#include "igD3D11Texture3D.h"
#include <algorithm>
using namespace ig::D3D11;

ID3D11ShaderResourceView* CTexture3D::RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc)
{
	return RetriveD3DSRV(pDesc);
}
ID3D11RenderTargetView* CTexture3D::RetriveRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc)
{
	return RetriveD3DRTV(pDesc);
}
ID3D11UnorderedAccessView* CTexture3D::RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc)
{
	return RetriveD3DUAV(pDesc);
}

ID3D11ShaderResourceView* CTexture3D::RetriveSRV(UINT firstMipLevel, UINT nMipLevels)
{
	CD3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = Desc().Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	desc.Texture3D.MostDetailedMip = firstMipLevel;
	desc.Texture3D.MipLevels = nMipLevels;
	return RetriveD3DSRV(&desc);


}

ID3D11RenderTargetView* CTexture3D::RetriveRTV(UINT firstDepthSlice, UINT nSlices, UINT mipLevel)
{

	CD3D11_RENDER_TARGET_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = Desc().Format;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
	desc.Texture3D.FirstWSlice = firstDepthSlice;
	desc.Texture3D.WSize = nSlices;
	desc.Texture3D.MipSlice = mipLevel;
	return RetriveD3DRTV(&desc);
}

ID3D11UnorderedAccessView* CTexture3D::RetriveUAV(UINT firstDepthSlice, UINT nSlices, UINT mipLevel)
{
	CD3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = Desc().Format;
	desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
	desc.Texture3D.FirstWSlice = firstDepthSlice;
	desc.Texture3D.WSize = nSlices;
	desc.Texture3D.MipSlice = mipLevel;
	return RetriveD3DUAV(&desc);

}

void CTexture3D::BuildMips(ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->GenerateMips(SRV());
}

// Map and unmap
HRESULT CTexture3D::Map(UINT level, D3D11_MAP mapType, void **ppData, UINT *pRowPitch, UINT *pDepthPitch, ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		D3D11_MAPPED_SUBRESOURCE res;
		IG_D3D_FAILED_RETURN(pDC->Map(m_pRes.Get(), level, mapType, 0, &res), L"CTexture3D::Map: Failed to map texture3D.");
		*ppData = res.pData;
		*pRowPitch = res.RowPitch;
		*pDepthPitch = res.DepthPitch;
		return S_OK;
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTexture3D::Map: Only dynamic/staging resource can be mapped.");
		return E_FAIL;
	}
}

void CTexture3D::Unmap(UINT level, ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->Unmap(m_pRes.Get(), level);
}

// Update subresource
void CTexture3D::Upload(UINT level, UINT xOffset, UINT yOffset, UINT zOffset, UINT w, UINT h, UINT d, const void* pSrcData, UINT srcRowPitch, UINT srcDepthPitch, ID3D11DeviceContext* pOptionalDC)
{
	UINT pixelSize = DXGI::GetDXGIFormatBytes(Desc().Format);
	if (pixelSize == 0) return;
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	CD3D11_BOX box(xOffset, yOffset, zOffset, xOffset + w, yOffset + h, zOffset + d);

	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		// Use map.
		BYTE *pData = nullptr;
		UINT rowPitch = 0;
		UINT depthPitch = 0;
		D3D11_MAP mapType = D3D11_USAGE_DYNAMIC == Desc().Usage ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE;
		Map(level, mapType, (void**)&pData, &rowPitch, &depthPitch, pDC);
		for (UINT k = 0; k < d; ++k)
			for (UINT j = 0; j < h; ++j)
			{
				memcpy(pData + xOffset * pixelSize + (yOffset + j) * rowPitch + (zOffset + k) * depthPitch, (BYTE*)pSrcData + j * srcRowPitch + k * srcDepthPitch, w * pixelSize);
			}
		Unmap(level, pDC);
	}
	else if (D3D11_USAGE_IMMUTABLE != Desc().Usage)
	{
		pDC->UpdateSubresource(m_pRes.Get(), level, &box, pSrcData, srcRowPitch, srcDepthPitch);
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTexture3D::Upload: Such resource does not support upload operation.");
	}
}

HRESULT CTexture3D::Create(ID3D11Texture3D *pTex)
{
	// Destroy
	Destroy();

	// Call base-class creation.
	IG_D3D_FAILED_RETURN(CResource::Create(pTex), L"CTexture3D::Create: CResource::Create() failed.");

	// Resource
	m_pTex = pTex;
	m_pTex->GetDesc(&m_Desc);

	// Get desired views.
	ShaderResourceViewComPtr pSRV;
	RenderTargetViewComPtr pRTV;
	DepthStencilViewComPtr pDSV;
	UnorderedAccessViewComPtr pUAV;

	// Get views.
	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		pSRV = RetriveSRV(0, m_Desc.MipLevels);
	}
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		pRTV = RetriveRTV(0, m_Desc.Depth, 0);
	}
	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		pUAV = RetriveUAV(0, m_Desc.Depth, 0);
	}
	
	// Fill data members.
	m_pSRV = pSRV;
	m_pRTV = pRTV;
	m_pUAV = pUAV;

	return S_OK;
}

HRESULT CTexture3D::Create(ID3D11Device* pDevice, const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitData)
{
	Texture3DComPtr pTex;
	IG_D3D_FAILED_RETURN(pDevice->CreateTexture3D(pDesc, pInitData, pTex.GetAddressOf()), L"CTexture3D::Create: Failed to create texture3D.");

	return Create(pTex.Get());
}

HRESULT CTexture3D::Create(ID3D11Device* pDevice, UINT w, UINT h, UINT d, UINT mipLevels, DXGI_FORMAT format,
	UINT bindFlags, D3D11_USAGE usage, D3D11_SUBRESOURCE_DATA *pInitData)
{
	UINT cpuAccessFlags = 0;
	if (usage == D3D11_USAGE_DYNAMIC) cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) cpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	UINT miscFlags = 0;
	if (mipLevels != 1 && (bindFlags &D3D11_BIND_RENDER_TARGET) && (bindFlags &D3D11_BIND_SHADER_RESOURCE))
		miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

	mipLevels = mipLevels == 0 ? std::max<UINT>(CalMipLevels(w), CalMipLevels(h)) : mipLevels;

	CD3D11_TEXTURE3D_DESC texDesc(format, w, h, d, mipLevels, bindFlags, usage, cpuAccessFlags, miscFlags);

	return Create(pDevice, &texDesc, pInitData);
}

void CTexture3D::Destroy()
{
	CResource::Destroy();
	m_pTex = nullptr;
	m_pSRV = nullptr;
	m_pRTV = nullptr;
	m_pUAV = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}