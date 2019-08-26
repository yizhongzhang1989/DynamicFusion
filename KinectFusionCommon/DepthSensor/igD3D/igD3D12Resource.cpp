#include "igD3D12Resource.h"

//#include "CommandQueue.h"
using namespace ig::D3D12;
// TODO: cache descriptors
//// from boost
//template<typename T> static void hash_combine(size_t& seed, T const& v)
//{
//	std::hash<T> hasher;
//	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//}
//
//template<typename It> static void hash_range(std::size_t& seed, It first, It last)
//{
//	for (; first != last; ++first)
//	{
//		hash_combine(seed, *first);
//	}
//}
void CResource::DoInitialization(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state)
{
	IG_D3D_ASSERT(spDevice != nullptr && pResource != nullptr);

	DeviceComPtr pDevice;
	IG_D3D_ASSERT_SUCCEEDED(pResource->GetDevice(__uuidof(ID3D12Device), (void**)pDevice.GetAddressOf()));
	IG_D3D_ASSERT(spDevice->RawPtr() == pDevice.Get());

	m_spDevice = spDevice;
	m_pResource = pResource;
	m_Desc = pResource->GetDesc();
	m_ResourceState = state;

	bool bIsTypelessFormat = ig::DXGI::IsTypeless(m_Desc.Format);
	bool bIsDepthFormat = ig::DXGI::IsDepthFormat(m_Desc.Format);
	bool bIsSRGB = ig::DXGI::IsSRGB(m_Desc.Format); // UAV && DSV don't support SRGB

	// Create views
	if (!bIsTypelessFormat && m_Desc.Format != DXGI_FORMAT_UNKNOWN)
	{
		if (m_Desc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER &&
			!(m_Desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE) &&
			!bIsDepthFormat)
		{
			m_hSRV = RetriveSRV(nullptr);
		}
		if (m_Desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			m_hRTV = RetriveRTV(nullptr);
		}
		if ((m_Desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) && !bIsSRGB)
		{
			m_hDSV = RetriveDSV(nullptr);
		}
		if ((m_Desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) && !bIsSRGB)
		{
			m_hUAV = RetriveUAV(nullptr);
		}
	}
}

CResource::CResource(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state)
{
	DoInitialization(spDevice, pResource, state);
}

CResource::CResource(
	CDevice::sPtr spDevice,
	const D3D12_HEAP_PROPERTIES *pHeapProperties,
	D3D12_HEAP_FLAGS heapFlags,
	const D3D12_RESOURCE_DESC *pResourceDesc,
	D3D12_RESOURCE_STATES initialResourceState,
	const D3D12_CLEAR_VALUE *pOptimizedClearValue)
{
	ResourceComPtr pResource;
	D3D12_RESOURCE_DESC desc = *pResourceDesc;
	// Allow UAV to enable MipMapGen. Since our MipMapGenerator uses CS.
	if (desc.MipLevels > 1 || desc.MipLevels == 0)
	{
		desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}
	// Set default optimized clear value.
	if (pOptimizedClearValue == nullptr)
	{
		if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			D3D12_CLEAR_VALUE defaultClearVal;
			defaultClearVal.Format = ig::DXGI::TypelessToFloatFormat(desc.Format);

			defaultClearVal.Color[0] = 0; defaultClearVal.Color[1] = 0;
			defaultClearVal.Color[2] = 0; defaultClearVal.Color[3] = 0;
			pOptimizedClearValue = &defaultClearVal;

		}
		else if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
		{
			D3D12_CLEAR_VALUE defaultClearVal;
			defaultClearVal.Format = ig::DXGI::TypelessToDepthFormat(desc.Format);

			defaultClearVal.DepthStencil.Depth = 1.f;
			defaultClearVal.DepthStencil.Stencil = 0;
			pOptimizedClearValue = &defaultClearVal;
		}
	}

	IG_D3D_ASSERT_SUCCEEDED(spDevice->RawPtr()->CreateCommittedResource(pHeapProperties, heapFlags,
		&desc, initialResourceState, pOptimizedClearValue, IID_PPV_ARGS(&pResource)));

	DoInitialization(spDevice, pResource.Get(), initialResourceState);
}


void CResource::GenMip(CComputeCommandContext *pContext)
{
	pContext->GenerateMips(this);
}


void CResource::GenMipSync()
{
	auto pContext = m_spDevice->GraphicsQueue()->RequestComputeContext();
	pContext->GenerateMips(this);
	m_spDevice->GraphicsQueue()->Execute(pContext);
	m_spDevice->GraphicsQueue()->WaitForIdle();
}

//void Resource::GenerateMipmap(ComputeCommandContext *pContext, Resource &resource, D3D12_RESOURCE_STATES afterState)
//{
//	auto pDevice = resource.DevicePtr();
//	IG_D3D_ASSERT(pDevice != nullptr);
//	std::map<ID3D12Device*, MipMapGenerator>::iterator it;
//	{
//		std::lock_guard<std::mutex> LockGuard(m_MipMutex);
//		it = sm_MipMapGenerators.find(pDevice);
//		if (it == sm_MipMapGenerators.end())
//		{
//			IG_D3D_ASSERT_SUCCEEDED(sm_MipMapGenerators[pDevice].Init(pDevice));
//			it = sm_MipMapGenerators.find(pDevice);
//		}
//	}
//	it->second.Run(pContext, resource, afterState);
//}

D3D12_CPU_DESCRIPTOR_HANDLE CResource::RetriveSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc)
{
	IG_D3D_ASSERT(m_pResource != nullptr);
	if (pDesc == nullptr && m_hSRV.ptr != 0) return m_hSRV;

	//std::lock_guard<std::mutex> LockGuard(m_RetriveMutex);
	D3D12_CPU_DESCRIPTOR_HANDLE hRet = m_spDevice->AllocCPUDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	CreateSRV(pDesc, hRet);
	return hRet;
}

D3D12_CPU_DESCRIPTOR_HANDLE CResource::RetriveRTV(const D3D12_RENDER_TARGET_VIEW_DESC *pDesc)
{
	IG_D3D_ASSERT(m_pResource != nullptr);
	if (pDesc == nullptr && m_hRTV.ptr != 0) return m_hRTV;

	//std::lock_guard<std::mutex> LockGuard(m_RetriveMutex);
	D3D12_CPU_DESCRIPTOR_HANDLE hRet = m_spDevice->AllocCPUDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1);	
	CreateRTV(pDesc, hRet);
	return hRet;
}

D3D12_CPU_DESCRIPTOR_HANDLE CResource::RetriveDSV(const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc)
{
	IG_D3D_ASSERT(m_pResource != nullptr);
	if (pDesc == nullptr && m_hDSV.ptr != 0) return m_hDSV;

	//std::lock_guard<std::mutex> LockGuard(m_RetriveMutex);
	D3D12_CPU_DESCRIPTOR_HANDLE hRet = m_spDevice->AllocCPUDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
	CreateDSV(pDesc, hRet);
	return hRet;
}

D3D12_CPU_DESCRIPTOR_HANDLE CResource::RetriveUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D12Resource* pCounterResource)
{
	IG_D3D_ASSERT(m_pResource != nullptr);
	if (pDesc == nullptr && m_hUAV.ptr != 0) return m_hUAV;

	//std::lock_guard<std::mutex> LockGuard(m_RetriveMutex);
	D3D12_CPU_DESCRIPTOR_HANDLE hRet = m_spDevice->AllocCPUDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	CreateUAV(pDesc, hRet);
	return hRet;
}


