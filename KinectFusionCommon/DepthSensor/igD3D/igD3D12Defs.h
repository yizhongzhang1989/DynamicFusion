#pragma once

// Usage: D3D12 Common Header
// Created: 2018.9.28
// 2018.9.28: Initial Version.
// 2018.10.8: Added SamplerDesc.
// 2019.6.3: Added DXR related stuffs.
// Version: 0.1.190603.1800

// TCHAR
#include <tchar.h>
#include <sdkddkver.h>
// D3D
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#include <d3d12sdklayers.h>
//#pragma comment(lib, "d3d12SDKLayers.lib")
#endif

// WRL
#include <wrl.h>

// shared_ptr & unique ptr
#include <memory>
#include <vector>

namespace ig
{
	namespace D3D12
	{
		// ComPtr Defs

		//using namespace DirectX;
		using Microsoft::WRL::ComPtr;

		//**************** ComPtr Typedefs ****************//
		typedef ComPtr<ID3D12Device>                DeviceComPtr;
#ifdef NTDDI_WIN10_RS5
		typedef ComPtr<ID3D12Device1>               Device1ComPtr;
		typedef ComPtr<ID3D12Device2>               Device2ComPtr;
		typedef ComPtr<ID3D12Device3>               Device3ComPtr;
		typedef ComPtr<ID3D12Device4>               Device4ComPtr;
		typedef ComPtr<ID3D12Device5>               Device5ComPtr;
		typedef ComPtr<ID3D12StateObject>			StateObjectComPtr;
		typedef ComPtr<ID3D12StateObjectProperties>	StateObjectPropertiesComPtr;
#endif
		typedef ComPtr<ID3D12CommandQueue>          CommandQueueComPtr;
		typedef ComPtr<ID3D12CommandAllocator>      CommandAllocatorComPtr;
		typedef ComPtr<ID3D12DescriptorHeap>        DescriptorHeapComPtr;
		typedef ComPtr<ID3D12Resource>              ResourceComPtr;
		typedef ComPtr<ID3DBlob>                    BlobComPtr;
		typedef ComPtr<ID3D12Fence>                 FenceComPtr;
		typedef ComPtr<ID3D12RootSignature>         RootSignatureComPtr;
		typedef ComPtr<ID3D12CommandList>           CommandListComPtr;
		typedef ComPtr<ID3D12GraphicsCommandList>   GraphicsCommandListComPtr;
		typedef ComPtr<ID3D12PipelineState>         PipelineStateComPtr;

		// Simple Desc structure Wrappers.
		struct CommandQueueDesc : public D3D12_COMMAND_QUEUE_DESC
		{
			CommandQueueDesc& AsDirect(
				INT priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
				D3D12_COMMAND_QUEUE_FLAGS flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
				UINT nodeMask = 0);

			CommandQueueDesc& AsBundle(
				INT priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
				D3D12_COMMAND_QUEUE_FLAGS flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
				UINT nodeMask = 0);

			CommandQueueDesc& AsCompute(
				INT priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
				D3D12_COMMAND_QUEUE_FLAGS flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
				UINT nodeMask = 0);

			CommandQueueDesc& AsCopy(
				INT priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
				D3D12_COMMAND_QUEUE_FLAGS flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
				UINT nodeMask = 0);

			operator const D3D12_COMMAND_QUEUE_DESC*() const;
		};

		struct DescriptorHeapDesc : public D3D12_DESCRIPTOR_HEAP_DESC
		{
			DescriptorHeapDesc& AsCBVSRVUAV(
				UINT numDescriptors,
				D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
				UINT nodeMask = 0);

			DescriptorHeapDesc& AsSampler(
				UINT numDescriptors,
				D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
				UINT nodeMask = 0);

			DescriptorHeapDesc& AsRTV(
				UINT numDescriptors,
				D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
				UINT nodeMask = 0);

			DescriptorHeapDesc& AsDSV(
				UINT numDescriptors,
				D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
				UINT nodeMask = 0);

			operator const D3D12_DESCRIPTOR_HEAP_DESC*() const;
		};

		struct HeapDesc : public D3D12_HEAP_DESC
		{
			HeapDesc& AsDefault(
				UINT64 size,
				UINT64 alignment = 0,
				D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE,
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			HeapDesc& AsUpload(
				UINT64 size,
				UINT64 alignment = 0,
				D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE,
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			HeapDesc& AsReadback(
				UINT64 size,
				UINT64 alignment = 0,
				D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE,
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			HeapDesc& AsCustom(
				UINT64 size,
				D3D12_CPU_PAGE_PROPERTY pageProperty,
				D3D12_MEMORY_POOL memPool,
				UINT64 alignment = 0,
				D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE,
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			operator const D3D12_HEAP_DESC*() const;
		};

		struct HeapProperties : public D3D12_HEAP_PROPERTIES
		{
			HeapProperties& As(
				D3D12_HEAP_TYPE type,
				D3D12_CPU_PAGE_PROPERTY cpuPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
				D3D12_MEMORY_POOL memoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			HeapProperties& AsDefault(
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			HeapProperties& AsUpload(
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			HeapProperties& AsReadback(
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			HeapProperties& AsCustom(
				D3D12_CPU_PAGE_PROPERTY pageProperty,
				D3D12_MEMORY_POOL memPool,
				UINT creationNodeMask = 1,
				UINT visibleNodeMask = 1);

			operator const D3D12_HEAP_PROPERTIES*() const;
		};

		struct ResourceDesc : public D3D12_RESOURCE_DESC
		{
			ResourceDesc& AsBuffer(
				UINT64 size,
				D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
				UINT64 alignment = 0);
			ResourceDesc& AsTex1D(
				DXGI_FORMAT format,
				UINT64 width,
				UINT16 mipLevels = 1,
				UINT16 arraySize = 1,
				D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
				UINT64 alignment = 0);
			ResourceDesc& AsTex2D(
				DXGI_FORMAT format,
				UINT64 width,
				UINT height,
				UINT16 mipLevels = 1,
				UINT16 arraySize = 1,
				UINT sampleCount = 1,
				UINT sampleQuality = 0,
				D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
				UINT64 alignment = 0);
			ResourceDesc& AsTex3D(
				DXGI_FORMAT format,
				UINT64 width,
				UINT height,
				UINT16 depth,
				UINT16 mipLevels = 1,
				D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
				UINT64 alignment = 0);

			operator const D3D12_RESOURCE_DESC*() const;
		};

		struct SRVDesc : public D3D12_SHADER_RESOURCE_VIEW_DESC
		{
			SRVDesc& AsBuffer(
				DXGI_FORMAT format,
				UINT64 firstElement,
				UINT numElements,
				UINT structureByteStride = 0,
				D3D12_BUFFER_SRV_FLAGS flags = D3D12_BUFFER_SRV_FLAG_NONE,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTex1D(
				DXGI_FORMAT format,
				UINT mostDetailedMip,
				UINT mipLevels,
				FLOAT resourceMinLODClamp = 0.f,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTex1DArray(
				DXGI_FORMAT format,
				UINT mostDetailedMip,
				UINT mipLevels,
				UINT firstArraySlice,
				UINT arraySize,
				UINT planeSlice = 0,
				FLOAT resourceMinLODClamp = 0.f,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTex2D(
				DXGI_FORMAT format,
				UINT mostDetailedMip,
				UINT mipLevels,
				UINT planeSlice = 0,
				FLOAT resourceMinLODClamp = 0.f,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTex2DArray(
				DXGI_FORMAT format,
				UINT mostDetailedMip,
				UINT mipLevels,
				UINT firstArraySlice,
				UINT arraySize,
				UINT planeSlice = 0,
				FLOAT resourceMinLODClamp = 0.f,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTex2DMS(
				DXGI_FORMAT format,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTex2DMSArray(
				DXGI_FORMAT format,
				UINT firstArraySlice,
				UINT arraySize,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTex3D(
				DXGI_FORMAT format,
				UINT mostDetailedMip,
				UINT mipLevels,
				FLOAT resourceMinLODClamp = 0.f,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTexCube(
				DXGI_FORMAT format,
				UINT  mostDetailedMip,
				UINT  mipLevels,
				FLOAT resourceMinLODClamp = 0.f,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			SRVDesc& AsTexCubeArray(
				DXGI_FORMAT format,
				UINT mostDetailedMip,
				UINT mipLevels,
				UINT first2DArrayFace,
				UINT numCubes,
				FLOAT resourceMinLODClamp = 0.f,
				UINT shaderComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING);

			operator const D3D12_SHADER_RESOURCE_VIEW_DESC*() const;
		};

		struct RTVDesc : public D3D12_RENDER_TARGET_VIEW_DESC
		{
			RTVDesc& AsBuffer(
				DXGI_FORMAT format,
				UINT64 firstElement,
				UINT numElements);

			RTVDesc& AsTex1D(
				DXGI_FORMAT format,
				UINT mipSlice);

			RTVDesc& AsTex1DArray(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstArraySlice,
				UINT arraySize);

			RTVDesc& AsTex2D(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT planeSlice = 0);

			RTVDesc& AsTex2DArray(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstArraySlice,
				UINT arraySize,
				UINT planeSlice = 0);

			RTVDesc& AsTex2DMS(DXGI_FORMAT format);

			RTVDesc& AsTex2DMSArray(
				DXGI_FORMAT format,
				UINT firstArraySlice,
				UINT arraySize);

			RTVDesc& AsTex3D(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstWSlice,
				UINT wSize);

			operator const D3D12_RENDER_TARGET_VIEW_DESC*() const;
		};

		struct DSVDesc : public D3D12_DEPTH_STENCIL_VIEW_DESC
		{
			DSVDesc& AsTex1D(
				DXGI_FORMAT format,
				UINT mipSlice,
				D3D12_DSV_FLAGS flags = D3D12_DSV_FLAG_NONE);

			DSVDesc& AsTex1DArray(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstArraySlice,
				UINT arraySize,
				D3D12_DSV_FLAGS flags = D3D12_DSV_FLAG_NONE);

			DSVDesc& AsTex2D(
				DXGI_FORMAT format,
				UINT mipSlice,
				D3D12_DSV_FLAGS flags = D3D12_DSV_FLAG_NONE);

			DSVDesc& AsTex2DArray(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstArraySlice,
				UINT arraySize,
				D3D12_DSV_FLAGS flags = D3D12_DSV_FLAG_NONE);

			DSVDesc& AsTex2DMS(
				DXGI_FORMAT format,
				D3D12_DSV_FLAGS flags = D3D12_DSV_FLAG_NONE);

			DSVDesc& AsTex2DMSArray(
				DXGI_FORMAT format,
				UINT firstArraySlice,
				UINT arraySize,
				D3D12_DSV_FLAGS flags = D3D12_DSV_FLAG_NONE);

			operator const D3D12_DEPTH_STENCIL_VIEW_DESC*() const;
		};

		struct UAVDesc : public D3D12_UNORDERED_ACCESS_VIEW_DESC
		{
			UAVDesc& AsBuffer(
				DXGI_FORMAT format,
				UINT64 firstElement,
				UINT numElements,
				UINT structureByteStride = 0,
				UINT64 counterOffsetInBytes = 0,
				D3D12_BUFFER_UAV_FLAGS flags = D3D12_BUFFER_UAV_FLAG_NONE);

			UAVDesc& AsTex1D(
				DXGI_FORMAT format,
				UINT mipSlice);

			UAVDesc& AsTex1DArray(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstArraySlice,
				UINT arraySize);

			UAVDesc& AsTex2D(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT planeSlice = 0);

			UAVDesc& AsTex2DArray(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstArraySlice,
				UINT arraySize,
				UINT planeSlice = 0);

			UAVDesc& AsTex3D(
				DXGI_FORMAT format,
				UINT mipSlice,
				UINT firstWSlice,
				UINT wSize);

			operator const D3D12_UNORDERED_ACCESS_VIEW_DESC*() const;
		};

		struct RasterizerDesc : public D3D12_RASTERIZER_DESC
		{
			RasterizerDesc& As(
				D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID,
				D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK,
				BOOL frontCounterClockwise = FALSE,
				INT depthBias = D3D12_DEFAULT_DEPTH_BIAS,
				FLOAT depthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
				FLOAT slopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
				BOOL depthClipEnable = TRUE,
				BOOL multisampleEnable = FALSE,
				BOOL antialiasedLineEnable = FALSE,
				UINT forcedSampleCount = 0,
				D3D12_CONSERVATIVE_RASTERIZATION_MODE conservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);

			/*operator D3D12_RASTERIZER_DESC() const
			{
			return *(D3D12_RASTERIZER_DESC*)this;
			}*/
		};

		struct DepthStencilDesc : public D3D12_DEPTH_STENCIL_DESC
		{
			DepthStencilDesc& As(
				BOOL depthEnable = FALSE,
				D3D12_DEPTH_WRITE_MASK depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL,
				D3D12_COMPARISON_FUNC depthFunc = D3D12_COMPARISON_FUNC_LESS,
				BOOL stencilEnable = FALSE,
				UINT8 stencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK,
				UINT8 stencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK,
				D3D12_STENCIL_OP frontFaceStencilFailOp = D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP frontFaceStencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP frontFaceStencilPassOp = D3D12_STENCIL_OP_KEEP,
				D3D12_COMPARISON_FUNC frontFaceStencilFunc = D3D12_COMPARISON_FUNC_ALWAYS,
				D3D12_STENCIL_OP backFaceStencilFailOp = D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP backFaceStencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
				D3D12_STENCIL_OP backFaceStencilPassOp = D3D12_STENCIL_OP_KEEP,
				D3D12_COMPARISON_FUNC backFaceStencilFunc = D3D12_COMPARISON_FUNC_ALWAYS);

			/*operator const D3D12_DEPTH_STENCIL_DESC&() const
			{
			return *this;
			}*/
		};

		struct RenderTargetBlendDesc : public D3D12_RENDER_TARGET_BLEND_DESC
		{
			RenderTargetBlendDesc& As(
				BOOL blendEnable = FALSE,
				BOOL logicOpEnable = FALSE,
				D3D12_BLEND srcBlend = D3D12_BLEND_ONE,
				D3D12_BLEND destBlend = D3D12_BLEND_ZERO,
				D3D12_BLEND_OP blendOp = D3D12_BLEND_OP_ADD,
				D3D12_BLEND srcBlendAlpha = D3D12_BLEND_ONE,
				D3D12_BLEND destBlendAlpha = D3D12_BLEND_ZERO,
				D3D12_BLEND_OP blendOpAlpha = D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP logicOp = D3D12_LOGIC_OP_NOOP,
				UINT8 renderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL);

			/*operator const D3D12_RENDER_TARGET_BLEND_DESC&() const
			{
			return *this;
			}*/
		};

		struct BlendDesc : public D3D12_BLEND_DESC
		{
			BlendDesc& As(
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget,
				BOOL alphaToCoverageEnable = FALSE);

			BlendDesc& AsIndependent(
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget0,
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget1,
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget2,
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget3,
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget4,
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget5,
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget6,
				const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget7,
				BOOL alphaToCoverageEnable = FALSE);

			/*operator const D3D12_BLEND_DESC&() const
			{
			return *this;
			}*/
		};

		struct SamplerDesc : public D3D12_SAMPLER_DESC
		{
			SamplerDesc& As(
				D3D12_FILTER Filter,
				D3D12_TEXTURE_ADDRESS_MODE AddressU,
				D3D12_TEXTURE_ADDRESS_MODE AddressV,
				D3D12_TEXTURE_ADDRESS_MODE AddressW,
				FLOAT MipLODBias,
				UINT MaxAnisotropy,
				D3D12_COMPARISON_FUNC ComparisonFunc,
				FLOAT borderColorR,
				FLOAT borderColorG,
				FLOAT borderColorB,
				FLOAT borderColorA,
				FLOAT MinLOD,
				FLOAT MaxLOD);

			SamplerDesc& AsPointClamp();

			SamplerDesc& AsPointWrap();

			SamplerDesc& AsPointMirror();

			SamplerDesc& AsLinearClamp();

			SamplerDesc& AsLinearWrap();

			SamplerDesc& AsLinearMirror();

			SamplerDesc& AsAnisoClamp(UINT maxAnisotropy = 16);

			SamplerDesc& AsAnisoWrap(UINT maxAnisotropy = 16);

			SamplerDesc& AsAnisoMirror(UINT maxAnisotropy = 16);

			operator const D3D12_SAMPLER_DESC*() const;
		};

		// Complex Desc structure Wrappers.
		class RootSignatureDesc
		{
		private:

			// Memory layout should be SAME as D3D12_ROOT_PARAMETER
			// So no data member is allowed.
			class RootParameter : public D3D12_ROOT_PARAMETER
			{
			public:
				RootParameter();

				~RootParameter();

				RootParameter(const RootParameter&) = delete;

				RootParameter& operator=(const RootParameter&) = delete;

				void Clear();

				void AsConstants(UINT shaderRegister, UINT numDWords, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsConstantBuffer(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsBufferSRV(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsBufferUAV(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

				RootParameter& AsDescriptorTable(UINT rangeCount, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

				void SetTableRange(UINT rangeIdx, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT count, UINT space = 0, UINT OffsetFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);
			};

			// Memory layout should be SAME as D3D12_STATIC_SAMPLER_DESC
			// So no data member is allowed.
			struct StaticSampler : public D3D12_STATIC_SAMPLER_DESC
			{
			public:
				void As(
					D3D12_FILTER filter,
					D3D12_TEXTURE_ADDRESS_MODE addressU,
					D3D12_TEXTURE_ADDRESS_MODE addressV,
					D3D12_TEXTURE_ADDRESS_MODE addressW,
					FLOAT mipLODBias,
					UINT maxAnisotropy,
					D3D12_COMPARISON_FUNC comparisonFunc,
					D3D12_STATIC_BORDER_COLOR borderColor,
					FLOAT minLOD,
					FLOAT maxLOD,
					UINT shaderRegister,
					UINT registerSpace,
					D3D12_SHADER_VISIBILITY shaderVisibility);
				
				// Frequently used samplers
				void AsPointClamp(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsPointWrap(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsPointMirror(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsLinearClamp(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsLinearWrap(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsLinearMirror(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsAnisoClamp(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsAnisoWrap(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				void AsAnisoMirror(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

				// Init from D3D12_SAMPLER_DESC
				void As(
					const D3D12_SAMPLER_DESC *pDesc,
					UINT shaderRegister,
					UINT registerSpace = 0,
					D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

			};

		public:
			RootSignatureDesc(UINT numRootParameters = 0, UINT numStaticSamplers = 0, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);
			void Reset(UINT numRootParameters = 0, UINT numStaticSamplers = 0, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

			RootParameter& Parameter(UINT i);
			const RootParameter& Parameter(UINT i) const;

			StaticSampler& Sampler(UINT i);
			const StaticSampler& Sampler(UINT i) const;

			D3D12_ROOT_SIGNATURE_FLAGS Flags() const;

			UINT NumRootParameters() const;
			UINT NumStaticSamplers() const;

			operator const D3D12_ROOT_SIGNATURE_DESC*() const;

		private:
			D3D12_ROOT_SIGNATURE_DESC m_Desc;
			std::unique_ptr<RootParameter[]> m_RootParams;
			std::unique_ptr<StaticSampler[]> m_StaticSamplers;
		};

		class GraphicsPipelineStateDesc
		{
		private:
			class InputAssemblerStage
			{
			public:
				InputAssemblerStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc);
				void SetInputLayout(UINT numElements = 0, const D3D12_INPUT_ELEMENT_DESC* pDescs = nullptr);
				void SetIBStripCutValue(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE val = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED);
				void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
			private:
				D3D12_GRAPHICS_PIPELINE_STATE_DESC * m_pDesc;
				//std::shared_ptr<D3D12_INPUT_ELEMENT_DESC> m_InputLayouts;
			};
			class RasterizerStage
			{
			public:
				RasterizerStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc);
				void SetRasterizerState(const D3D12_RASTERIZER_DESC& desc = RasterizerDesc().As());
			private:
				D3D12_GRAPHICS_PIPELINE_STATE_DESC * m_pDesc;
			};
			class OutputMergerStage
			{
			public:
				OutputMergerStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc);
				void SetBlendState(const D3D12_BLEND_DESC& desc = BlendDesc().As(RenderTargetBlendDesc().As()));
				void SetBlendSampleMask(UINT mask = UINT_MAX);
				void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& desc = DepthStencilDesc().As());
				void SetRenderTargetFormat(DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT, UINT msaaCount = 1, UINT msaaQuality = 0);
				void SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* pRtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
			private:
				D3D12_GRAPHICS_PIPELINE_STATE_DESC * m_pDesc;
			};
			class StreamOutStage
			{
			public:
				StreamOutStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc);
				void SetStreamOut(const D3D12_STREAM_OUTPUT_DESC &desc);
			private:
				D3D12_GRAPHICS_PIPELINE_STATE_DESC * m_pDesc;
			};
			class ShaderStage
			{
			public:
				ShaderStage(D3D12_SHADER_BYTECODE *pShader);
				void SetShader(const void *pShaderBytecode = nullptr, SIZE_T bytecodeLength = 0);

			protected:
				D3D12_SHADER_BYTECODE * m_pShader;
			};

		public:
			GraphicsPipelineStateDesc();

			void SetNodeMask(UINT mask = 0);
			void SetRootSignature(ID3D12RootSignature *pSig = nullptr);
			void SetFlag(D3D12_PIPELINE_STATE_FLAGS flags = D3D12_PIPELINE_STATE_FLAG_NONE);
			void SetCachedPSO(const void *pBlob = nullptr, SIZE_T nSize = 0);

			operator const D3D12_GRAPHICS_PIPELINE_STATE_DESC*() const;

		public: // stages
			InputAssemblerStage IA;
			RasterizerStage RS;
			OutputMergerStage OM;
			StreamOutStage SO;
			ShaderStage VS;
			ShaderStage HS;
			ShaderStage DS;
			ShaderStage GS;
			ShaderStage PS;

		private:
			D3D12_GRAPHICS_PIPELINE_STATE_DESC m_Desc;

		};

		class ComputePipelineStateDesc
		{
		public:
			ComputePipelineStateDesc();

			void SetNodeMask(UINT mask = 0);
			void SetRootSignature(ID3D12RootSignature *pSig = nullptr);
			void SetFlag(D3D12_PIPELINE_STATE_FLAGS flags = D3D12_PIPELINE_STATE_FLAG_NONE);
			void SetCachedPSO(const void *pBlob = nullptr, SIZE_T nSize = 0);
			void SetComputeShader(const void *pShaderBytecode = nullptr, SIZE_T bytecodeLength = 0);
			D3D12_COMPUTE_PIPELINE_STATE_DESC& GetDesc();
			const D3D12_COMPUTE_PIPELINE_STATE_DESC& GetDesc() const;

			operator const D3D12_COMPUTE_PIPELINE_STATE_DESC*() const;
		private:
			D3D12_COMPUTE_PIPELINE_STATE_DESC m_Desc;
		};

		struct Viewport : public D3D12_VIEWPORT
		{
			Viewport(
				float topLeftX, float topLeftY,
				float w, float h,
				float minDepth = D3D12_MIN_DEPTH,
				float maxDepth = D3D12_MAX_DEPTH);
			
			operator const D3D12_VIEWPORT*() const;
		};
#ifdef NTDDI_WIN10_RS5
		struct DispatchRaysDesc : D3D12_DISPATCH_RAYS_DESC
		{
			DispatchRaysDesc(UINT width, UINT height, UINT depth = 1);
			
			void SetRayGenerationShaderRecord(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes);
			
			void SetHitGroupTable(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes, UINT64 strideInBytes);
			
			void SetMissShaderTable(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes, UINT64 strideInBytes);
			
			void SetCallableShaderTable(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes, UINT64 strideInBytes);
			
		};

		class StateObjectDesc
		{
		public:
			struct SubObjectBase
			{
				SubObjectBase(D3D12_STATE_SUBOBJECT_TYPE subObjectType, const void *pSubObjectDesc);
				D3D12_STATE_SUBOBJECT_TYPE Type() const
				{
					return m_Type;
				}
				operator const D3D12_STATE_SUBOBJECT*() const
				{
					return &m_SubObject;
				}
			private:
				D3D12_STATE_SUBOBJECT_TYPE m_Type;
				D3D12_STATE_SUBOBJECT m_SubObject;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType, typename T = void>
			struct SubObject : SubObjectBase
			{
				SubObject();
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY>::type> : SubObjectBase
			{
				SubObject();

				void SetLibrary(D3D12_SHADER_BYTECODE* pCode);
				
				void SetLibrary(const void *pShaderBytecode, SIZE_T BytecodeLength);
				
				void AddExport(
					LPCWSTR Name,
					LPCWSTR ExportToRename = nullptr,
					D3D12_EXPORT_FLAGS Flags = D3D12_EXPORT_FLAG_NONE);
				
				D3D12_DXIL_LIBRARY_DESC m_Desc;
				std::vector<D3D12_EXPORT_DESC> m_Exports;
				std::vector<std::wstring> m_ExportStrings;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION>::type> : SubObjectBase
			{
				SubObject();
				
				void SetCollection(ID3D12StateObject* pExistingCollection);

				void AddExport(
					LPCWSTR Name,
					LPCWSTR ExportToRename = nullptr,
					D3D12_EXPORT_FLAGS Flags = D3D12_EXPORT_FLAG_NONE);

				D3D12_EXISTING_COLLECTION_DESC m_Desc;
				StateObjectComPtr m_pExistingCollection;
				std::vector<D3D12_EXPORT_DESC> m_Exports;
				std::vector<std::wstring> m_ExportStrings;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type> : SubObjectBase
			{
				SubObject();

				void SetSubobject(const D3D12_STATE_SUBOBJECT *pSubobjectToAssociate);

				void AddExport(LPCWSTR Export);
				
				D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION m_Desc;
				std::vector<LPCWSTR> m_Exports;
				std::vector<std::wstring> m_ExportStrings;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type> : SubObjectBase
			{
				SubObject();

				void SetSubobject(LPCWSTR subobjectName);
				
				void AddExport(LPCWSTR Export);
				
				D3D12_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION m_Desc;
				std::wstring m_sSubObjectName;
				std::vector<LPCWSTR> m_Exports;
				std::vector<std::wstring> m_ExportStrings;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP>::type> : SubObjectBase
			{
				SubObject();

				void SetHitGroup(
					D3D12_HIT_GROUP_TYPE type,
					LPCWSTR strExportName,
					LPCWSTR strAnyHitShaderImport,
					LPCWSTR strClosestHitShaderImport,
					LPCWSTR strIntersectionShaderImport);

				D3D12_HIT_GROUP_DESC m_Desc;
				std::wstring m_sAnyHitShaderImport;
				std::wstring m_sClosestHitShaderImport;
				std::wstring m_sIntersectionShaderImport;
				std::wstring m_sHitGroupExport;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG>::type> : SubObjectBase
			{
				SubObject();

				void Config(UINT MaxPayloadSizeInBytes, UINT MaxAttributeSizeInBytes);
				
				D3D12_RAYTRACING_SHADER_CONFIG m_Desc;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG>::type> : SubObjectBase
			{
				SubObject();

				void Config(UINT MaxTraceRecursionDepth);
				
				D3D12_RAYTRACING_PIPELINE_CONFIG m_Desc;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE>::type> : SubObjectBase
			{
				SubObject();

				void SetRootSignature(ID3D12RootSignature* pRootSig);
				
				D3D12_GLOBAL_ROOT_SIGNATURE m_Desc;
				RootSignatureComPtr m_pRS;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE>::type> : SubObjectBase
			{
				SubObject();

				void SetRootSignature(ID3D12RootSignature* pRootSig);
				
				D3D12_LOCAL_ROOT_SIGNATURE m_Desc;
				RootSignatureComPtr m_pRS;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG>::type> : SubObjectBase
			{
				SubObject();

				void SetFlags(D3D12_STATE_OBJECT_FLAGS Flags);
				
				D3D12_STATE_OBJECT_CONFIG m_Desc;
			};

			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			struct SubObject<SubObjectType,
				typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK>::type> : SubObjectBase
			{
				SubObject();

				void SetNodeMask(UINT nodeMask);
				
				D3D12_NODE_MASK m_Desc;
			};

		public:
			StateObjectDesc(D3D12_STATE_OBJECT_TYPE type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE);
			
			template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
			SubObject<SubObjectType> *CreateSubObject();
			
			operator const D3D12_STATE_OBJECT_DESC*() const;
			
			operator const D3D12_STATE_OBJECT_DESC&() const;

		private:
			D3D12_STATE_OBJECT_DESC m_Desc;
			std::vector<D3D12_STATE_SUBOBJECT> m_SubObjectData;
			std::vector<std::unique_ptr<SubObjectBase>> m_SubObjects;
		};
#endif
	}
}

#include "igD3DCommon.h"
#include "igDXGI.h"
#include "inl\igD3D12Defs.inl"

