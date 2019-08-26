#pragma once

// Usage: D3D12 Resource
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// 2019.5.28: Skipped default view creation on DXGI_FORMAT_UNKNOWN format resources.
// 2019.5.29: Added SetName method.
// Version: 0.1.190529.1600

#include "igD3D12Defs.h"
#include "igD3D12Device.h"

namespace ig
{
	namespace D3D12
	{
		
		class CComputeCommandContext;
		class CCommandContext;

		class CResource
		{
			friend class CCommandContext;

		private:
			void DoInitialization(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

		public:
			CResource(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);
			CResource(CDevice::sPtr spDevice,
				const D3D12_HEAP_PROPERTIES *pHeapProperties,
				D3D12_HEAP_FLAGS heapFlags,
				const D3D12_RESOURCE_DESC *pResourceDesc,
				D3D12_RESOURCE_STATES initialResourceState,
				const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr);

		public:
			typedef std::shared_ptr<CResource> sPtr;
			typedef std::unique_ptr<CResource> uPtr;

			static uPtr Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

			static uPtr Create(CDevice::sPtr spDevice,
				const D3D12_HEAP_PROPERTIES *pHeapProperties,
				D3D12_HEAP_FLAGS heapFlags,
				const D3D12_RESOURCE_DESC *pResourceDesc,
				D3D12_RESOURCE_STATES initialResourceState,
				const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr);
			

			HRESULT Map(UINT subresource, const D3D12_RANGE *pRange, void **ppData);
			

			void Unmap(UINT subresource, const D3D12_RANGE *pRange);
			

			// SRV/RTV/DSV/CBV
			void CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle);

			void CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle);

			void CreateDSV(const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle);

			void CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle, ID3D12Resource* pCounterResource = nullptr);

			// Getters
			ID3D12Resource* RawPtr() const;
			CDevice* DevicePtr() const;
			const D3D12_RESOURCE_DESC& Desc() const;
			D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() const;
			D3D12_RESOURCE_STATES CurrentState() const;

		public:
			D3D12_CPU_DESCRIPTOR_HANDLE SRV();
			
			D3D12_CPU_DESCRIPTOR_HANDLE RTV();
			
			D3D12_CPU_DESCRIPTOR_HANDLE DSV();
			
			D3D12_CPU_DESCRIPTOR_HANDLE UAV();
			
			D3D12_CPU_DESCRIPTOR_HANDLE RetriveSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc);
			D3D12_CPU_DESCRIPTOR_HANDLE RetriveRTV(const D3D12_RENDER_TARGET_VIEW_DESC *pDesc);
			D3D12_CPU_DESCRIPTOR_HANDLE RetriveDSV(const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc);
			D3D12_CPU_DESCRIPTOR_HANDLE RetriveUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D12Resource* pCounterResource = nullptr);

			void GenMipSync();
			void GenMip(CComputeCommandContext *pContext);

			HRESULT SetName(LPCWSTR strName);

		protected:
			CDevice::sPtr m_spDevice = nullptr;
			ResourceComPtr m_pResource = nullptr;
			D3D12_RESOURCE_DESC m_Desc;
			D3D12_CPU_DESCRIPTOR_HANDLE m_hSRV = { 0 };
			D3D12_CPU_DESCRIPTOR_HANDLE m_hRTV = { 0 };
			D3D12_CPU_DESCRIPTOR_HANDLE m_hDSV = { 0 };
			D3D12_CPU_DESCRIPTOR_HANDLE m_hUAV = { 0 };
			
			// Resource state can only be modified by CommandQueue::GraphicsCommandList
			// Currently not thread-safe
			D3D12_RESOURCE_STATES m_ResourceState = D3D12_RESOURCE_STATE_COMMON;
		};
	}
}

#include "inl\igD3D12Resource.inl"


