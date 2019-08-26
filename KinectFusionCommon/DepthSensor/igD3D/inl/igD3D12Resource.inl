#pragma once

namespace ig
{
	namespace D3D12
	{
		inline CResource::uPtr CResource::Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state)
		{
			return std::make_unique<CResource>(spDevice, pResource, state);
		}

		inline CResource::uPtr CResource::Create(CDevice::sPtr spDevice,
			const D3D12_HEAP_PROPERTIES *pHeapProperties,
			D3D12_HEAP_FLAGS heapFlags,
			const D3D12_RESOURCE_DESC *pResourceDesc,
			D3D12_RESOURCE_STATES initialResourceState,
			const D3D12_CLEAR_VALUE *pOptimizedClearValue)
		{
			return std::make_unique<CResource>(spDevice, pHeapProperties, heapFlags, pResourceDesc, initialResourceState, pOptimizedClearValue);
		}

		// Map
		inline HRESULT CResource::Map(UINT subresource, const D3D12_RANGE *pRange, void **ppData)
		{
			return m_pResource->Map(subresource, pRange, ppData);
		}

		inline void CResource::Unmap(UINT subresource, const D3D12_RANGE *pRange)
		{
			return m_pResource->Unmap(subresource, pRange);
		}

		// SRV/RTV/DSV/CBV
		inline void CResource::CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle)
		{
			m_spDevice->RawPtr()->CreateShaderResourceView(m_pResource.Get(), pDesc, handle);
		}

		inline void CResource::CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle)
		{
			m_spDevice->RawPtr()->CreateRenderTargetView(m_pResource.Get(), pDesc, handle);
		}

		inline void CResource::CreateDSV(const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle)
		{
			m_spDevice->RawPtr()->CreateDepthStencilView(m_pResource.Get(), pDesc, handle);
		}

		inline void CResource::CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc, D3D12_CPU_DESCRIPTOR_HANDLE handle, ID3D12Resource* pCounterResource)
		{
			m_spDevice->RawPtr()->CreateUnorderedAccessView(m_pResource.Get(), pCounterResource, pDesc, handle);
		}

		// Getters
		inline ID3D12Resource* CResource::RawPtr() const { return m_pResource.Get(); }
		inline CDevice* CResource::DevicePtr() const { return m_spDevice.get(); }
		inline const D3D12_RESOURCE_DESC& CResource::Desc() const { return m_Desc; }
		inline D3D12_GPU_VIRTUAL_ADDRESS CResource::GPUVirtualAddress() const { return m_pResource->GetGPUVirtualAddress(); }
		inline D3D12_RESOURCE_STATES CResource::CurrentState() const { return m_ResourceState; }

		inline D3D12_CPU_DESCRIPTOR_HANDLE CResource::SRV()
		{
			IG_D3D_ASSERT(m_hSRV.ptr != 0, L"This resource doesn't support default SRV.");
			return m_hSRV;
		}
		inline D3D12_CPU_DESCRIPTOR_HANDLE CResource::RTV()
		{
			IG_D3D_ASSERT(m_hRTV.ptr != 0, L"This resource doesn't support default RTV.");
			return m_hRTV;
		}
		inline D3D12_CPU_DESCRIPTOR_HANDLE CResource::DSV()
		{
			IG_D3D_ASSERT(m_hDSV.ptr != 0, L"This resource doesn't support default DSV.");
			return m_hDSV;
		}
		inline D3D12_CPU_DESCRIPTOR_HANDLE CResource::UAV()
		{
			IG_D3D_ASSERT(m_hUAV.ptr != 0, L"This resource doesn't support default UAV.");
			return m_hUAV;
		}

		inline HRESULT CResource::SetName(LPCWSTR strName)
		{
			return m_pResource->SetName(strName);
		}
	}
}



