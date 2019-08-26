#pragma once

// Usage: D3D12 CommandContext
// Created: 2018.9.27
// 2018.9.27: Initial Version.
// 2019.5.29: Added DXR stuffs to ComputeCommandContext.
// 2019.5.30: Supports UAV clear.
// Version: 0.1.190530.1600

#include "igD3D12Defs.h"
#include <mutex>
#include <vector>
#include <queue>
#include <stdint.h>

#include "igD3D12CoreUploadMemoryAllocator.h"
#include "igD3D12CoreGPUDescriptorAllocator.h"
#include "igD3D12MipMapGen.inl"

namespace ig
{
	namespace D3D12
	{
		class CCommandQueue;
		class CResource;
		class CPipelineState;
		class CRootSignature;
		class CStateObject;

		class CCommandContext
		{
			friend class CCommandQueue;
		public:
			enum ContextType
			{
				Graphics,
				Compute,
			};

			// Getters
			ID3D12GraphicsCommandList* CommandListRawPtr() const { return m_pCommandList.Get(); }
			ID3D12CommandAllocator* AssociatedAllocator() const { return m_pAssociatedAllocator; }
			ContextType Type() const { return m_ContextType; }

			// Copy operations
			void CopyResource(CResource *pDstResource, CResource *pSrcResource);
			
			void CopyBufferRegion(CResource *pDstBuffer, UINT64 dstOffset, CResource *pSrcBuffer, UINT64 srcOffset, UINT64 nBytes);
			
			void CopyTextureRegion(D3D12_TEXTURE_COPY_LOCATION *pDst, UINT DstX, UINT DstY, UINT DstZ, const D3D12_TEXTURE_COPY_LOCATION *pSrc, const D3D12_BOX *pSrcBox);			

			void WriteBuffer(CResource *pDstBuf, size_t nDstOffset, const void *pData, size_t nSizeInBytes);
			
			// Barrier operations
			void TransitionResource(CResource* pResource, D3D12_RESOURCE_STATES dstState);
			void UAVBarrier(CResource* pResource, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);

			// Clear operations
			void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE hRTV, const FLOAT *colorRGBA, UINT numRects = 0, const D3D12_RECT *pRect = nullptr);

			void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE hRTV, FLOAT R, FLOAT G, FLOAT B, FLOAT A, UINT numRects = 0, const D3D12_RECT *pRects = nullptr);

			void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE hDSV, D3D12_CLEAR_FLAGS clearFlags = D3D12_CLEAR_FLAG_DEPTH,
				FLOAT depth = 1.f, UINT8 stencil = 0, UINT numRects = 0, const D3D12_RECT *pRects = nullptr);

			void ClearUnorderedAccessViewFloat(D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, const FLOAT *pValues, UINT numRects = 0, const D3D12_RECT *pRects = nullptr);

			void ClearUnorderedAccessViewFloat(D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, FLOAT R, FLOAT G, FLOAT B, FLOAT A, UINT numRects = 0, const D3D12_RECT *pRects = nullptr);

			void ClearUnorderedAccessViewUint(D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, const UINT *pValues, UINT numRects, const D3D12_RECT *pRects);
			
			void ClearUnorderedAccessViewUint(D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, UINT R, UINT G, UINT B, UINT A, UINT numRects, const D3D12_RECT *pRects);		
		
			// Pipeline operations
			void SetPipelineState(ID3D12PipelineState *pPSO);

			void SetPipelineState(CPipelineState *pPSO);

		protected:
			CCommandContext(
				ContextType contextType,
				ID3D12Device *pDevice,
				D3D12_COMMAND_LIST_TYPE type,
				ID3D12CommandAllocator *pAllocator,
				ID3D12PipelineState *pState = nullptr,
				UINT nodeMask = 0);

			HRESULT Close(); // Called by CCommandQueue
			
			HRESULT Reset(ID3D12CommandAllocator *pAllocator, ID3D12PipelineState *pInitialState); // Called by CCommandQueue

			void TransitionBarrier(ID3D12Resource *pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter,
				UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);

			void UAVBarrier(ID3D12Resource *pResource, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);

			// Resources for upload memory
			Core::CUploadMemoryAllocator::MemoryBlock AllocUploadMemoryBlocks(size_t size, const void *pInitialData, size_t alignment = 256);
			void FreeUploadMemoryBlocks(ID3D12Fence *pFence, uint64_t completionFenceVal);
			std::unique_ptr<Core::CUploadMemoryAllocator> m_upUploadMemoryAllocator;
			std::vector<Core::CUploadMemoryAllocator::MemoryBlock> m_uploadMemBlocksNeedFree;

			// Resources for dynamic descriptors
			enum DynDescType
			{
				VIEW = 0, // CBV_SRV_UAV
				SAMPLER = 1,
				NUM_DESC_TYPES,
			};

			void SetDynamicDescriptorsToCache(DynDescType type, UINT rootIdx, UINT nOffset, UINT nDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors);
			void ResolveCachedDynamicViewDescriptors();
			void FreeDynamicDescriptorHeapBlocks(ID3D12Fence *pFence, uint64_t completionFenceVal);
			static const UINT sm_nMaxRoots = 16;
			std::unique_ptr<Core::CGPUDescriptorAllocator> m_upDescriptorAllocator[NUM_DESC_TYPES];
			std::vector<Core::CGPUDescriptorAllocator::HeapBlock> m_DescHeapBlocks[NUM_DESC_TYPES];
			
			struct DynDescInfo
			{
				UINT nOffsetInTable;
				UINT nDescriptors;
				UINT nCacheOffset;
			};
			std::vector<DynDescInfo> m_DynDescs[NUM_DESC_TYPES][sm_nMaxRoots];
			std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_DescCache;

		protected:
			DeviceComPtr m_pDevice;
			GraphicsCommandListComPtr m_pCommandList;
			ID3D12CommandAllocator* m_pAssociatedAllocator = nullptr;
			ContextType m_ContextType;

		};

		class CComputeCommandContext : public CCommandContext
		{
			friend class CCommandQueue;
		public:
			class CComputeRoot
			{
			public:
				CComputeRoot(CComputeCommandContext *pContext) : m_pContext(pContext) {}

				void SetSignature(ID3D12RootSignature *pSignature)
				{
					m_pContext->CommandListRawPtr()->SetComputeRootSignature(pSignature);
				}
				void SetSignature(CRootSignature *pSignature);

				void Set32BitConstant(UINT rootParamIdx, UINT srcData, UINT destOffsetIn32BitVals)
				{
					m_pContext->CommandListRawPtr()->SetComputeRoot32BitConstant(rootParamIdx, srcData, destOffsetIn32BitVals);
				}
				void Set32BitConstants(UINT rootParamIdx, UINT num32bitVals, void* pSrcData, UINT destOffsetIn32BitVals)
				{
					m_pContext->CommandListRawPtr()->SetComputeRoot32BitConstants(rootParamIdx, num32bitVals, pSrcData, destOffsetIn32BitVals);
				}

				void SetBufferShaderResourceView(UINT rootParamIdx, D3D12_GPU_VIRTUAL_ADDRESS bufLocation)
				{
					m_pContext->CommandListRawPtr()->SetComputeRootShaderResourceView(rootParamIdx, bufLocation);
				}
				void SetBufferUnorderedAccessView(UINT rootParamIdx, D3D12_GPU_VIRTUAL_ADDRESS bufLocation)
				{
					m_pContext->CommandListRawPtr()->SetComputeRootUnorderedAccessView(rootParamIdx, bufLocation);
				}

				void SetConstantBufferView(UINT rootParamIdx, D3D12_GPU_VIRTUAL_ADDRESS bufLocation)
				{
					m_pContext->CommandListRawPtr()->SetComputeRootConstantBufferView(rootParamIdx, bufLocation);
				}
				void SetDynamicConstantBuffer(UINT rootIdx, size_t nBufSize, const void* pData)
				{
					auto cb = m_pContext->AllocUploadMemoryBlocks(nBufSize, pData, 256); // constant buffer requires 256-byte-alignment
					SetConstantBufferView(0, cb.GPUAddress);
				}

				void SetDynamicViewDescriptor(UINT rootIdx, UINT nOffset, D3D12_CPU_DESCRIPTOR_HANDLE hDescriptor)
				{
					SetDynamicViewDescriptors(rootIdx, nOffset, 1, &hDescriptor);
				}

				void SetDynamicViewDescriptors(UINT rootIdx, UINT nOffset, UINT nDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors)
				{
					m_pContext->SetDynamicDescriptorsToCache(DynDescType::VIEW, rootIdx, nOffset, nDescriptors, pDescriptors);
				}

				void SetDynamicSamplerDescriptor(UINT rootIdx, UINT nOffset, D3D12_CPU_DESCRIPTOR_HANDLE hDescriptor)
				{
					SetDynamicSamplerDescriptors(rootIdx, nOffset, 1, &hDescriptor);
				}

				void SetDynamicSamplerDescriptors(UINT rootIdx, UINT nOffset, UINT nDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors)
				{
					m_pContext->SetDynamicDescriptorsToCache(DynDescType::SAMPLER, rootIdx, nOffset, nDescriptors, pDescriptors);
				}

			private:
				CComputeCommandContext * m_pContext;

			};

		public:
			void Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ)
			{
				ResolveCachedDynamicViewDescriptors();
				m_pCommandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
			}

			CComputeRoot ComputeRoot; // State setters
			void GenerateMips(CResource *pResource)
			{
				m_MipMapGen.Run(this, pResource);
			}
#ifdef NTDDI_WIN10_RS5
			// Extension for DXR, temporary solution
			void BuildRaytracingAccelerationStructure(
				const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC *pDesc,
				UINT nPostbuildInfoDescs = 0,
				const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC *pPostbuildInfoDescs = nullptr);
			void SetRayTracingStateObject(ID3D12StateObject *pSO);
			void SetRayTracingStateObject(CStateObject *pSO);
			void DispatchRays(const D3D12_DISPATCH_RAYS_DESC *pDesc)
			{
				// Temporary solution
				Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> pDXRCmdList;
				IG_D3D_ASSERT_SUCCEEDED(m_pCommandList->QueryInterface(IID_PPV_ARGS(&pDXRCmdList)));
				ResolveCachedDynamicViewDescriptors();
				pDXRCmdList->DispatchRays(pDesc);
			}
#endif
		private:
			CComputeCommandContext(
				ID3D12Device *pDevice,
				D3D12_COMMAND_LIST_TYPE type,
				ID3D12CommandAllocator *pAllocator,
				ID3D12PipelineState *pState = nullptr,
				UINT nodeMask = 0) :
				CCommandContext(Compute, pDevice, type, pAllocator, pState, nodeMask),
				ComputeRoot(this), m_MipMapGen(pDevice, 0)
			{
			}
			MipMapGen m_MipMapGen;
		};

		class CGraphicsCommandContext : public CCommandContext
		{
			friend class CCommandQueue;
		public:
			class CCommonBase
			{
			public:
				CCommonBase(CGraphicsCommandContext *pContext) : m_pContext(pContext) {}
				CGraphicsCommandContext* m_pContext = nullptr;
			};

			class CGraphicsRoot : public CCommonBase
			{
			public:
				CGraphicsRoot(CGraphicsCommandContext *pContext) : CCommonBase(pContext) {}

				void SetSignature(ID3D12RootSignature *pSignature)
				{
					m_pContext->CommandListRawPtr()->SetGraphicsRootSignature(pSignature);
				}

				void SetSignature(CRootSignature *pSignature);
				

				void Set32BitConstant(UINT rootParamIdx, UINT srcData, UINT destOffsetIn32BitVals)
				{
					m_pContext->CommandListRawPtr()->SetGraphicsRoot32BitConstant(rootParamIdx, srcData, destOffsetIn32BitVals);
				}
				void Set32BitConstants(UINT rootParamIdx, UINT num32bitVals, void* pSrcData, UINT destOffsetIn32BitVals)
				{
					m_pContext->CommandListRawPtr()->SetGraphicsRoot32BitConstants(rootParamIdx, num32bitVals, pSrcData, destOffsetIn32BitVals);
				}

				void SetBufferShaderResourceView(UINT rootParamIdx, D3D12_GPU_VIRTUAL_ADDRESS bufLocation)
				{
					m_pContext->CommandListRawPtr()->SetGraphicsRootShaderResourceView(rootParamIdx, bufLocation);
				}
				void SetBufferUnorderedAccessView(UINT rootParamIdx, D3D12_GPU_VIRTUAL_ADDRESS bufLocation)
				{
					m_pContext->CommandListRawPtr()->SetGraphicsRootUnorderedAccessView(rootParamIdx, bufLocation);
				}

				void SetConstantBufferView(UINT rootParamIdx, D3D12_GPU_VIRTUAL_ADDRESS bufLocation)
				{
					m_pContext->CommandListRawPtr()->SetGraphicsRootConstantBufferView(rootParamIdx, bufLocation);
				}
				void SetDynamicConstantBuffer(UINT rootIdx, size_t nBufSize, const void* pData)
				{
					auto cb = m_pContext->AllocUploadMemoryBlocks(nBufSize, pData, 256); // constant buffer requires 256-byte-alignment
					SetConstantBufferView(rootIdx, cb.GPUAddress);
				}
				void SetDynamicViewDescriptor(UINT rootIdx, UINT nOffset, D3D12_CPU_DESCRIPTOR_HANDLE hDescriptor)
				{
					SetDynamicViewDescriptors(rootIdx, nOffset, 1, &hDescriptor);
				}
				void SetDynamicViewDescriptors(UINT rootIdx, UINT nOffset, UINT nDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors)
				{
					m_pContext->SetDynamicDescriptorsToCache(DynDescType::VIEW, rootIdx, nOffset, nDescriptors, pDescriptors);
				}
				void SetDynamicSamplerDescriptor(UINT rootIdx, UINT nOffset, D3D12_CPU_DESCRIPTOR_HANDLE hDescriptor)
				{
					SetDynamicSamplerDescriptors(rootIdx, nOffset, 1, &hDescriptor);
				}
				void SetDynamicSamplerDescriptors(UINT rootIdx, UINT nOffset, UINT nDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors)
				{
					m_pContext->SetDynamicDescriptorsToCache(DynDescType::SAMPLER, rootIdx, nOffset, nDescriptors, pDescriptors);
				}
			};
			class CInputAssemberStage : public CCommonBase
			{
			public:
				CInputAssemberStage(CGraphicsCommandContext *pContext) : CCommonBase(pContext) {}
				void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
				{
					m_pContext->CommandListRawPtr()->IASetPrimitiveTopology(topology);
				}
				void SetVertexBuffers(UINT startSlot, UINT numViews, const D3D12_VERTEX_BUFFER_VIEW *pVBVs)
				{
					m_pContext->CommandListRawPtr()->IASetVertexBuffers(startSlot, numViews, pVBVs);
				}
				void SetVertexBuffer(UINT slot, const D3D12_VERTEX_BUFFER_VIEW *pVBV)
				{
					m_pContext->CommandListRawPtr()->IASetVertexBuffers(slot, 1, pVBV);
				}
				void SetVertexBuffer(UINT slot, D3D12_GPU_VIRTUAL_ADDRESS bufLocation, UINT sizeInBytes, UINT strideInBytes)
				{
					D3D12_VERTEX_BUFFER_VIEW vbv;
					vbv.BufferLocation = bufLocation;
					vbv.SizeInBytes = sizeInBytes;
					vbv.StrideInBytes = strideInBytes;
					m_pContext->CommandListRawPtr()->IASetVertexBuffers(slot, 1, &vbv);
				}
				void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW *pIBV)
				{
					m_pContext->CommandListRawPtr()->IASetIndexBuffer(pIBV);
				}
				void SetIndexBuffer(D3D12_GPU_VIRTUAL_ADDRESS bufLocation, UINT sizeInBytes, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT)
				{
					D3D12_INDEX_BUFFER_VIEW ibv;
					ibv.BufferLocation = bufLocation;
					ibv.SizeInBytes = sizeInBytes;
					ibv.Format = format;
					m_pContext->CommandListRawPtr()->IASetIndexBuffer(&ibv);
				}
			};
			class CRasterizerStage : public CCommonBase
			{
			public:
				CRasterizerStage(CGraphicsCommandContext *pContext) : CCommonBase(pContext) {}
				void SetViewports(UINT numVPs, const D3D12_VIEWPORT *pVPs)
				{
					m_pContext->CommandListRawPtr()->RSSetViewports(numVPs, pVPs);
				}
				void SetViewport(const D3D12_VIEWPORT *pVP)
				{
					m_pContext->CommandListRawPtr()->RSSetViewports(1, pVP);
				}
				void SetViewport(
					FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height,
					FLOAT minDepth = D3D12_MIN_DEPTH, FLOAT maxDepth = D3D12_MAX_DEPTH)
				{
					D3D12_VIEWPORT vp = { topLeftX, topLeftY, width, height, minDepth, maxDepth };
					m_pContext->CommandListRawPtr()->RSSetViewports(1, &vp);
				}
				void SetScissorRects(UINT numRects, const D3D12_RECT *pRects)
				{
					m_pContext->CommandListRawPtr()->RSSetScissorRects(numRects, pRects);
				}
				void SetScissorRect(const D3D12_RECT *pRect)
				{
					m_pContext->CommandListRawPtr()->RSSetScissorRects(1, pRect);
				}
				void SetScissorRect(LONG left, LONG top, LONG right, LONG bottom)
				{
					D3D12_RECT rect = { left, top, right, bottom };
					m_pContext->CommandListRawPtr()->RSSetScissorRects(1, &rect);
				}
			};
			class COutputMergerStage : public CCommonBase
			{
			public:
				COutputMergerStage(CGraphicsCommandContext *pContext) : CCommonBase(pContext) {}
				void SetRenderTargets(UINT numRTs, const D3D12_CPU_DESCRIPTOR_HANDLE *pRTDescriptors,
					BOOL RTsSingleHandleToDescriptorRange, const D3D12_CPU_DESCRIPTOR_HANDLE *pDSDescriptor)
				{
					m_pContext->CommandListRawPtr()->OMSetRenderTargets(numRTs, pRTDescriptors, RTsSingleHandleToDescriptorRange, pDSDescriptor);

				}
				void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtDescriptorHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsDescriptorHandle)
				{
					m_pContext->CommandListRawPtr()->OMSetRenderTargets(1, &rtDescriptorHandle, true, &dsDescriptorHandle);
				}
				void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtDescriptorHandle)
				{
					m_pContext->CommandListRawPtr()->OMSetRenderTargets(1, &rtDescriptorHandle, true, nullptr);
				}
				void SetBlendFactor(const FLOAT blendFactor[4])
				{
					m_pContext->CommandListRawPtr()->OMSetBlendFactor(blendFactor);
				}
				void SetBlendFactor(FLOAT factor0, FLOAT factor1, FLOAT factor2, FLOAT factor3)
				{
					FLOAT blendFactor[4] = { factor0, factor1, factor2, factor3 };
					m_pContext->CommandListRawPtr()->OMSetBlendFactor(blendFactor);
				}
				void SetStencilRef(UINT stencilRef)
				{
					m_pContext->CommandListRawPtr()->OMSetStencilRef(stencilRef);
				}
			};

		public:
			void DrawInstanced(UINT vtxCountPerInstance, UINT instanceCount, UINT startVtxLocation, UINT startInstanceLocation)
			{
				// Resolve dynamic table descriptors
				ResolveCachedDynamicViewDescriptors();
				m_pCommandList->DrawInstanced(vtxCountPerInstance, instanceCount, startVtxLocation, startInstanceLocation);
			}

			void DrawIndexedInstanced(UINT idxCountPerInstance, UINT instanceCount, UINT startIdxLocation, UINT baseVtxLocation, UINT startInstanceLocation)
			{
				// Resolve dynamic table descriptors
				ResolveCachedDynamicViewDescriptors();
				m_pCommandList->DrawIndexedInstanced(idxCountPerInstance, instanceCount, startIdxLocation, baseVtxLocation, startInstanceLocation);

			}

			CGraphicsRoot GraphicsRoot;
			CInputAssemberStage IA;
			CRasterizerStage RS;
			COutputMergerStage OM;

			private:
				CGraphicsCommandContext(ID3D12Device *pDevice,
					D3D12_COMMAND_LIST_TYPE type,
					ID3D12CommandAllocator *pAllocator,
					ID3D12PipelineState *pState = nullptr,
					UINT nodeMask = 0) :
					CCommandContext(Graphics, pDevice, type, pAllocator, pState, nodeMask),
					GraphicsRoot(this), IA(this), RS(this), OM(this)
				{
				}
		};
		
	}
}

