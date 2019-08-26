#include "inl\igD3D12CommandContext.inl"
#include "igD3D12Resource.h"
#include "igD3D12PipelineState.h"
#include "igD3D12RootSignature.h"
#ifdef NTDDI_WIN10_RS5
#include "igD3D12StateObject.h"
#endif
using namespace ig::D3D12;

Core::CUploadMemoryAllocator::MemoryBlock CCommandContext::AllocUploadMemoryBlocks(size_t size, const void *pInitialData, size_t alignment)
{
	auto cb = m_upUploadMemoryAllocator->Alloc(size, pInitialData, alignment);
	m_uploadMemBlocksNeedFree.push_back(cb);
	return cb;
}

void CCommandContext::FreeUploadMemoryBlocks(ID3D12Fence *pFence, uint64_t completionFenceVal) // Called by queue.
{
	while (m_uploadMemBlocksNeedFree.size() > 0)
	{
		m_upUploadMemoryAllocator->Free(m_uploadMemBlocksNeedFree.back(), pFence, completionFenceVal);
		m_uploadMemBlocksNeedFree.pop_back();
	}
}

// Resources for dynamic descriptors
void CCommandContext::SetDynamicDescriptorsToCache(DynDescType type, UINT rootIdx, UINT nOffset, UINT nDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pDescriptors)
{
	for (UINT i = 0; i < nDescriptors; ++i)
	{
		m_DescCache.push_back(pDescriptors[i]);
	}
	DynDescInfo info;
	info.nDescriptors = nDescriptors;
	info.nOffsetInTable = nOffset;
	info.nCacheOffset = (UINT)m_DescCache.size() - nDescriptors;// &m_DescCache[m_DescCache.size() - nDescriptors];
	m_DynDescs[type][rootIdx].push_back(info);
}

void CCommandContext::ResolveCachedDynamicViewDescriptors()
{
	for (UINT t = 0; t < DynDescType::NUM_DESC_TYPES; ++t)
	{
		// Parse dynamic descriptors
		UINT tableOffsets[sm_nMaxRoots];
		UINT nTableSizeSum = 0;
		for (UINT i = 0; i < sm_nMaxRoots; ++i)
		{
			tableOffsets[i] = nTableSizeSum;
			if (m_DynDescs[t][i].size() > 0)
			{
				UINT tableSize = 0;
				for (size_t s = 0; s < m_DynDescs[t][i].size(); ++s)
				{
					tableSize = std::max<UINT>(tableSize, m_DynDescs[t][i][s].nOffsetInTable + m_DynDescs[t][i][s].nDescriptors);
				}
				nTableSizeSum += tableSize;
			}
		}
	
		// Alloc dynamicDescriptorHeaps
		//m_DescHeapBlock[t] = m_upDescriptorAllocator[t]->Alloc(nTableSizeSum);
		auto heapBlock = m_upDescriptorAllocator[t]->Alloc(nTableSizeSum);
		// Set View and Sampler Heaps
		ID3D12DescriptorHeap* heaps[1] = { heapBlock.SrcHeapPtr() };
		m_pCommandList->SetDescriptorHeaps(1, heaps);

		// Copy descriptors, set tables, and clear caches
		for (UINT i = 0; i < sm_nMaxRoots; ++i)
		{
			if (m_DynDescs[t][i].size() > 0)
			{
				for (size_t s = 0; s < m_DynDescs[t][i].size(); ++s)
				{				
					for (UINT h = 0; h < m_DynDescs[t][i][s].nDescriptors; ++h)
					{
						if (t == DynDescType::VIEW)
						{
							m_pDevice->CopyDescriptorsSimple(1, heapBlock.CPUHandle(tableOffsets[i] + m_DynDescs[t][i][s].nOffsetInTable + h), m_DescCache[m_DynDescs[t][i][s].nCacheOffset + h], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
						}
						else if (t == DynDescType::SAMPLER)
						{
							m_pDevice->CopyDescriptorsSimple(1, heapBlock.CPUHandle(tableOffsets[i] + m_DynDescs[t][i][s].nOffsetInTable + h), m_DescCache[m_DynDescs[t][i][s].nCacheOffset + h], D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
						}
						else
						{
						}
					}
				}
				if (m_ContextType == Graphics)
				{
					m_pCommandList->SetGraphicsRootDescriptorTable(i, heapBlock.GPUHandle(tableOffsets[i]));
				}
				else if (m_ContextType == Compute)
				{
					m_pCommandList->SetComputeRootDescriptorTable(i, heapBlock.GPUHandle(tableOffsets[i]));
				}
				else
				{

				}
				m_DescHeapBlocks[t].push_back(heapBlock);
				m_DynDescs[t][i].clear();
			}
		}
	}
	m_DescCache.clear();

}

void CCommandContext::FreeDynamicDescriptorHeapBlocks(ID3D12Fence *pFence, uint64_t completionFenceVal) // Called by queue.
{
	for (UINT t = 0; t < DynDescType::NUM_DESC_TYPES; ++t)
	{
		for (size_t s = 0; s < m_DescHeapBlocks[t].size(); ++s)
		{
			m_upDescriptorAllocator[t]->Free(m_DescHeapBlocks[t][s], pFence, completionFenceVal);
		}
		m_DescHeapBlocks[t].clear();
	}
}

CCommandContext::CCommandContext(
	ContextType contextType,
	ID3D12Device *pDevice,
	D3D12_COMMAND_LIST_TYPE type,
	ID3D12CommandAllocator *pAllocator,
	ID3D12PipelineState *pState,
	UINT nodeMask) :
	m_ContextType(contextType),
	m_pDevice(pDevice),
	m_pAssociatedAllocator(pAllocator)
{
	IG_D3D_ASSERT_SUCCEEDED(pDevice->CreateCommandList(nodeMask, type, pAllocator, pState, IID_PPV_ARGS(&m_pCommandList)));
	m_upUploadMemoryAllocator = std::make_unique<Core::CUploadMemoryAllocator>(pDevice);
	m_upDescriptorAllocator[DynDescType::VIEW] = std::make_unique<Core::CGPUDescriptorAllocator>(pDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_upDescriptorAllocator[DynDescType::SAMPLER] = std::make_unique<Core::CGPUDescriptorAllocator>(pDevice, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
}
	
void CCommandContext::CopyResource(CResource *pDstResource, CResource *pSrcResource)
{
	m_pCommandList->CopyResource(pDstResource->RawPtr(), pSrcResource->RawPtr());
}

void CCommandContext::CopyBufferRegion(CResource *pDstBuffer, UINT64 dstOffset, CResource *pSrcBuffer, UINT64 srcOffset, UINT64 nBytes)
{
	m_pCommandList->CopyBufferRegion(pDstBuffer->RawPtr(), dstOffset, pSrcBuffer->RawPtr(), srcOffset, nBytes);
}

void CCommandContext::CopyTextureRegion(D3D12_TEXTURE_COPY_LOCATION *pDst, UINT DstX, UINT DstY, UINT DstZ, const D3D12_TEXTURE_COPY_LOCATION *pSrc, const D3D12_BOX *pSrcBox)
{
	m_pCommandList->CopyTextureRegion(pDst, DstX, DstY, DstZ, pSrc, pSrcBox);
}

void CCommandContext::WriteBuffer(CResource *pDstBuf, size_t nDstOffset, const void *pData, size_t nSizeInBytes)
{
	auto cb = AllocUploadMemoryBlocks(nSizeInBytes, pData);
	m_pCommandList->CopyBufferRegion(pDstBuf->RawPtr(), nDstOffset, cb.pResource, cb.nOffsetInResource, nSizeInBytes);
}

void CCommandContext::TransitionResource(CResource* pResource, D3D12_RESOURCE_STATES dstState)
{
	if (dstState != pResource->m_ResourceState)
	{
		TransitionBarrier(pResource->RawPtr(), pResource->m_ResourceState, dstState);
		pResource->m_ResourceState = dstState;
	}
	else if (dstState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		//Add UAV barrier
		UAVBarrier(pResource->RawPtr());
	}
}

void CCommandContext::UAVBarrier(CResource* pResource, D3D12_RESOURCE_BARRIER_FLAGS flags)
{
	UAVBarrier(pResource->RawPtr(), flags);
}

void CCommandContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE hRTV, const FLOAT *colorRGBA, UINT numRects, const D3D12_RECT *pRect)
{
	m_pCommandList->ClearRenderTargetView(hRTV, colorRGBA, numRects, pRect);
}

void CCommandContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE hRTV, FLOAT R, FLOAT G, FLOAT B, FLOAT A, UINT numRects, const D3D12_RECT *pRects)
{
	FLOAT colorRGBA[4] = { R, G, B, A };
	m_pCommandList->ClearRenderTargetView(hRTV, colorRGBA, numRects, pRects);
}

void CCommandContext::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE hDSV, D3D12_CLEAR_FLAGS clearFlags,
	FLOAT depth, UINT8 stencil, UINT numRects, const D3D12_RECT *pRects)
{
	m_pCommandList->ClearDepthStencilView(hDSV, clearFlags, depth, stencil, numRects, pRects);
}

void CCommandContext::ClearUnorderedAccessViewFloat(D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, const FLOAT *pValues, UINT numRects, const D3D12_RECT *pRects)
{
	// Alloc dynamicDescriptorHeaps
	auto heapBlock = m_upDescriptorAllocator[VIEW]->Alloc(1);

	// Set View and Sampler Heaps
	ID3D12DescriptorHeap* heaps[1] = { heapBlock.SrcHeapPtr() };
	m_pCommandList->SetDescriptorHeaps(1, heaps);
	m_pDevice->CopyDescriptorsSimple(1, heapBlock.CPUHandle(0), hUAVCPU, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_pCommandList->ClearUnorderedAccessViewFloat(heapBlock.GPUHandle(0), hUAVCPU, pResource, pValues, numRects, pRects);
	m_DescHeapBlocks[VIEW].push_back(heapBlock);
}

void CCommandContext::ClearUnorderedAccessViewFloat(D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, FLOAT R, FLOAT G, FLOAT B, FLOAT A, UINT numRects, const D3D12_RECT *pRects)
{
	FLOAT values[4] = { R, G, B, A };
	ClearUnorderedAccessViewFloat(hUAVCPU, pResource, values, numRects, pRects);
}

void CCommandContext::ClearUnorderedAccessViewUint( D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, const UINT *pValues, UINT numRects, const D3D12_RECT *pRects)
{
	// Alloc dynamicDescriptorHeaps
	auto heapBlock = m_upDescriptorAllocator[VIEW]->Alloc(1);

	// Set View and Sampler Heaps
	ID3D12DescriptorHeap* heaps[1] = { heapBlock.SrcHeapPtr() };
	m_pCommandList->SetDescriptorHeaps(1, heaps);
	m_pDevice->CopyDescriptorsSimple(1, heapBlock.CPUHandle(0), hUAVCPU, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_pCommandList->ClearUnorderedAccessViewUint(heapBlock.GPUHandle(0), heapBlock.CPUHandle(0), pResource, pValues, numRects, pRects);
	m_DescHeapBlocks[VIEW].push_back(heapBlock);
}

void CCommandContext::ClearUnorderedAccessViewUint(D3D12_CPU_DESCRIPTOR_HANDLE hUAVCPU, ID3D12Resource *pResource, UINT R, UINT G, UINT B, UINT A, UINT numRects, const D3D12_RECT *pRects)
{
	UINT values[4] = { R, G, B, A };
	ClearUnorderedAccessViewUint(hUAVCPU, pResource, values, numRects, pRects);
}

void CCommandContext::SetPipelineState(ID3D12PipelineState *pPSO)
{
	m_pCommandList->SetPipelineState(pPSO);
}

void CCommandContext::SetPipelineState(CPipelineState *pPSO)
{
	m_pCommandList->SetPipelineState(pPSO->RawPtr());
}


HRESULT CCommandContext::Close()
{
	IG_D3D_FAILED_RETURN(m_pCommandList->Close());
	return S_OK;
}

HRESULT CCommandContext::Reset(ID3D12CommandAllocator *pAllocator, ID3D12PipelineState *pInitialState)
{
	m_pAssociatedAllocator = pAllocator;
	IG_D3D_FAILED_RETURN(m_pCommandList->Reset(pAllocator, pInitialState));
	return S_OK;
}

void CCommandContext::TransitionBarrier(ID3D12Resource *pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter,
	UINT subresource, D3D12_RESOURCE_BARRIER_FLAGS flags)
{
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = flags;
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateBefore = stateBefore;
	barrier.Transition.StateAfter = stateAfter;
	barrier.Transition.Subresource = subresource;
	m_pCommandList->ResourceBarrier(1, &barrier);
}

void CCommandContext::UAVBarrier(ID3D12Resource *pResource, D3D12_RESOURCE_BARRIER_FLAGS flags)
{
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = flags;
	barrier.UAV.pResource = pResource;
	m_pCommandList->ResourceBarrier(1, &barrier);
}

void CGraphicsCommandContext::CGraphicsRoot::SetSignature(CRootSignature *pSignature)
{
	m_pContext->CommandListRawPtr()->SetGraphicsRootSignature(pSignature->RawPtr());
}

void CComputeCommandContext::CComputeRoot::SetSignature(CRootSignature *pSignature)
{
	m_pContext->CommandListRawPtr()->SetComputeRootSignature(pSignature->RawPtr());
}
#ifdef NTDDI_WIN10_RS5
void CComputeCommandContext::BuildRaytracingAccelerationStructure(
	const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC *pDesc,
	UINT nPostbuildInfoDescs,
	const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC *pPostbuildInfoDescs)
{
	// Temporary solution
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> pDXRCmdList;
	IG_D3D_ASSERT_SUCCEEDED(m_pCommandList->QueryInterface(IID_PPV_ARGS(&pDXRCmdList)));
	pDXRCmdList->BuildRaytracingAccelerationStructure(pDesc, nPostbuildInfoDescs, pPostbuildInfoDescs);
}

void CComputeCommandContext::SetRayTracingStateObject(CStateObject *pSO)
{
	// Temporary solution
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> pDXRCmdList;
	IG_D3D_ASSERT_SUCCEEDED(m_pCommandList->QueryInterface(IID_PPV_ARGS(&pDXRCmdList)));
	pDXRCmdList->SetPipelineState1(pSO->RawPtr());
}

void CComputeCommandContext::SetRayTracingStateObject(ID3D12StateObject *pSO)
{
	// Temporary solution
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> pDXRCmdList;
	IG_D3D_ASSERT_SUCCEEDED(m_pCommandList->QueryInterface(IID_PPV_ARGS(&pDXRCmdList)));
	pDXRCmdList->SetPipelineState1(pSO);
}
#endif		