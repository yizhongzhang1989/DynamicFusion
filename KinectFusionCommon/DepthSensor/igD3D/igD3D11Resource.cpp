#include "igD3D11Resource.h"
using namespace ig::D3D11;

// from boost
template<typename T> void hash_combine(size_t& seed, T const& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename It> void hash_range(std::size_t& seed, It first, It last)
{
	for (; first != last; ++first)
	{
		hash_combine(seed, *first);
	}
}


ID3D11ShaderResourceView* CResource::RetriveD3DSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc)
{
	const char* pData = reinterpret_cast<const char*>(pDesc);
	size_t hashVal = 0;
	hash_range(hashVal, pData, pData + sizeof(CD3D11_SHADER_RESOURCE_VIEW_DESC));

	auto iter = m_spSRVs->find(hashVal);
	if (iter != m_spSRVs->end())
	{
		return iter->second.Get();
	}

	ShaderResourceViewComPtr pSRV;

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(*pDesc);
	if (FAILED(m_pDevice->CreateShaderResourceView(ResourcePtr(), &srvDesc, pSRV.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CResource::RetriveD3DSRV: Failed to create SRV.");
		return nullptr;
	}
	auto ret = m_spSRVs->insert(std::make_pair(hashVal, pSRV));
	return ret.first->second.Get();
}

ID3D11RenderTargetView* CResource::RetriveD3DRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc)
{
	const char* pData = reinterpret_cast<const char*>(pDesc);
	size_t hashVal = 0;
	hash_range(hashVal, pData, pData + sizeof(CD3D11_RENDER_TARGET_VIEW_DESC));

	auto iter = m_spRTVs->find(hashVal);
	if (iter != m_spRTVs->end())
	{
		return iter->second.Get();
	}

	RenderTargetViewComPtr pRTV;
	CD3D11_RENDER_TARGET_VIEW_DESC rtvDesc(*pDesc);
	if (FAILED(m_pDevice->CreateRenderTargetView(ResourcePtr(), &rtvDesc, pRTV.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CResource::RetriveD3DRTV: Failed to create RTV.");
		return nullptr;
	}
	auto ret = m_spRTVs->insert(std::make_pair(hashVal, pRTV));
	return ret.first->second.Get();
}

ID3D11UnorderedAccessView* CResource::RetriveD3DUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc)
{
	const char* pData = reinterpret_cast<const char*>(pDesc);
	size_t hashVal = 0;
	hash_range(hashVal, pData, pData + sizeof(CD3D11_UNORDERED_ACCESS_VIEW_DESC));

	auto iter = m_spUAVs->find(hashVal);
	if (iter != m_spUAVs->end())
	{
		return iter->second.Get();
	}

	UnorderedAccessViewComPtr pUAV;
	CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc(*pDesc);
	if (FAILED(m_pDevice->CreateUnorderedAccessView(ResourcePtr(), &uavDesc, pUAV.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CResource::RetriveD3DUAV: Failed to create UAV.");
		return nullptr;
	}

	auto ret = m_spUAVs->insert(std::make_pair(hashVal, pUAV));
	return ret.first->second.Get();
}

ID3D11DepthStencilView* CResource::RetriveD3DDSV(const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc)
{
	const char* pData = reinterpret_cast<const char*>(pDesc);
	size_t hashVal = 0;
	hash_range(hashVal, pData, pData + sizeof(CD3D11_DEPTH_STENCIL_VIEW_DESC));

	auto iter = m_spDSVs->find(hashVal);
	if (iter != m_spDSVs->end())
	{
		return iter->second.Get();
	}
	DepthStencilViewComPtr pDSV;
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(*pDesc);
	if (FAILED(m_pDevice->CreateDepthStencilView(ResourcePtr(), &dsvDesc, pDSV.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CResource::RetriveD3DDSV: Failed to create DSV.");
		return nullptr;
	}

	auto ret = m_spDSVs->insert(std::make_pair(hashVal, pDSV));
	return ret.first->second.Get();
}

HRESULT CResource::Create(ID3D11Resource *pResource)
{
	// Clear
	Destroy();
	// Validate ptr.
	if (!pResource)
	{
		IG_D3D_ERROR_DUMP(L"CResource::Create: pResource must not be NULL.");
		return E_INVALIDARG;
	}

	m_pRes = pResource;
	m_spSRVs = std::make_shared<std::unordered_map<size_t, ShaderResourceViewComPtr>>();
	m_spRTVs = std::make_shared<std::unordered_map<size_t, RenderTargetViewComPtr>>();
	m_spUAVs = std::make_shared<std::unordered_map<size_t, UnorderedAccessViewComPtr>>();
	m_spDSVs = std::make_shared<std::unordered_map<size_t, DepthStencilViewComPtr>>();
	m_pRes->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_pDevice->GetImmediateContext(m_pImmediateDC.ReleaseAndGetAddressOf()); 
	return S_OK;
}
void CResource::Destroy()
{
	m_pDevice = nullptr;
	m_pImmediateDC = nullptr;
	m_pRes = nullptr;
	m_spSRVs = nullptr;
	m_spRTVs = nullptr;
	m_spUAVs = nullptr;
	m_spDSVs = nullptr;
}

void CResource::CopyTo(ID3D11Resource *pDstRes, ID3D11DeviceContext *pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->CopyResource(pDstRes, m_pRes.Get());
}

void CResource::CopyFrom(ID3D11Resource *pSrcRes, ID3D11DeviceContext *pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->CopyResource(m_pRes.Get(), pSrcRes);
}