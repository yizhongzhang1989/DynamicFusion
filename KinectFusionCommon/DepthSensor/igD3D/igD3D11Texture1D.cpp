#include "igD3D11Texture1D.h"
#include <algorithm>
using namespace ig::D3D11;

ID3D11ShaderResourceView* CTexture1D::RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc)
{
	return RetriveD3DSRV(pDesc);
}
ID3D11RenderTargetView* CTexture1D::RetriveRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc)
{
	return RetriveD3DRTV(pDesc);
}
ID3D11UnorderedAccessView* CTexture1D::RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc)
{
	return RetriveD3DUAV(pDesc);
}
ID3D11DepthStencilView* CTexture1D::RetriveDSV(const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc)
{
	return RetriveD3DDSV(pDesc);
}

ID3D11ShaderResourceView* CTexture1D::RetriveSRV(UINT firstArraySlice, UINT arraySize, UINT firstMipLevel, UINT numMipLevels)
{
	CD3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = Desc().Format;
	if (Desc().ArraySize > 1)
	{
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
		desc.Texture1DArray.ArraySize = arraySize;
		desc.Texture1DArray.FirstArraySlice = firstArraySlice;
		desc.Texture1DArray.MipLevels = numMipLevels;
		desc.Texture1DArray.MostDetailedMip = firstMipLevel;
	}
	else
	{
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		desc.Texture1D.MipLevels = numMipLevels;
		desc.Texture1D.MostDetailedMip = firstMipLevel;
	}
	return RetriveD3DSRV(&desc);


}

ID3D11RenderTargetView* CTexture1D::RetriveRTV(UINT firstArraySlice, UINT arraySize, UINT mipLevel)
{

	CD3D11_RENDER_TARGET_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = Desc().Format;
	if (Desc().ArraySize > 1)
	{
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
		desc.Texture1DArray.ArraySize = arraySize;
		desc.Texture1DArray.FirstArraySlice = firstArraySlice;
		desc.Texture1DArray.MipSlice = mipLevel;

	}
	else
	{
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
		desc.Texture2D.MipSlice = mipLevel;
	}
	return RetriveD3DRTV(&desc);
}

ID3D11UnorderedAccessView* CTexture1D::RetriveUAV(UINT firstArraySlice, UINT arraySize, UINT mipLevel)
{
	CD3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = Desc().Format;
	if (Desc().ArraySize > 1)
	{
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
		desc.Texture1DArray.ArraySize = arraySize;
		desc.Texture1DArray.FirstArraySlice = firstArraySlice;
		desc.Texture1DArray.MipSlice = mipLevel;
	}
	else
	{

		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
		desc.Texture1D.MipSlice = mipLevel;

	}
	return RetriveD3DUAV(&desc);

}

ID3D11DepthStencilView* CTexture1D::RetriveDSV(UINT firstArraySlice, UINT arraySize, UINT mipLevel)
{
	CD3D11_DEPTH_STENCIL_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = Desc().Format;
	if (Desc().ArraySize > 1)
	{

		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
		desc.Texture1DArray.ArraySize = arraySize;
		desc.Texture1DArray.FirstArraySlice = firstArraySlice;
		desc.Texture1DArray.MipSlice = mipLevel;

	}
	else
	{
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
		desc.Texture1D.MipSlice = mipLevel;
	}
	return RetriveD3DDSV(&desc);
}

void CTexture1D::BuildMips(ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->GenerateMips(SRV());
}

// Map and unmap
HRESULT CTexture1D::Map(UINT arrayIdx, UINT level, D3D11_MAP mapType, void **ppData, ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		D3D11_MAPPED_SUBRESOURCE res;
		IG_D3D_FAILED_RETURN(pDC->Map(m_pRes.Get(), m_Desc.MipLevels * arrayIdx + level, mapType, 0, &res), L"CTexture1D::Map: Failed to map texture1D.");
		*ppData = res.pData;
		return S_OK;
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTexture1D::Map: Only dynamic/staging resource can be mapped.");
		return E_FAIL;
	}
}

void CTexture1D::Unmap(UINT arrayIdx, UINT level, ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->Unmap(m_pRes.Get(), m_Desc.MipLevels * arrayIdx + level);
}

// Update subresource
void CTexture1D::Upload(UINT arrayIdx, UINT level, UINT offset, UINT w, const void* pSrcData, ID3D11DeviceContext* pOptionalDC)
{
	UINT pixelSize = DXGI::GetDXGIFormatBytes(Desc().Format);
	if (pixelSize == 0) return;
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	CD3D11_BOX box(offset, 0, 0, offset + w, 1, 1);

	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		// Use map.
		BYTE *pData = nullptr;

		D3D11_MAP mapType = D3D11_USAGE_DYNAMIC == Desc().Usage ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE;
		Map(arrayIdx, level, mapType, (void**)&pData, pDC);
		memcpy(pData + offset * pixelSize, (BYTE*)pSrcData, w * pixelSize);
		Unmap(arrayIdx, level, pDC);
	}
	else if (D3D11_USAGE_IMMUTABLE != Desc().Usage)
	{
		pDC->UpdateSubresource(m_pRes.Get(), Desc().MipLevels * arrayIdx + level, &box, pSrcData, 0, 0);
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTexture1D::Upload: Such resource does not support upload operation.");
	}
}

HRESULT CTexture1D::Create(ID3D11Texture1D *pTex)
{
	// Destroy
	Destroy();
	
	// Call base-class creation.
	IG_D3D_FAILED_RETURN(CResource::Create(pTex), L"CTexture1D::Create: CResource::Create() failed.");

	// Resource.
	m_pTex = pTex;
	m_pTex->GetDesc(&m_Desc);

	// Get desired views.
	ShaderResourceViewComPtr pSRV;
	RenderTargetViewComPtr pRTV;
	DepthStencilViewComPtr pDSV;
	UnorderedAccessViewComPtr pUAV;
	
	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		pSRV = RetriveSRV(0, m_Desc.ArraySize, 0, m_Desc.MipLevels);
	}
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		pRTV = RetriveRTV(0, m_Desc.ArraySize, 0);
	}
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		pDSV = RetriveDSV(0, m_Desc.ArraySize, 0);
	}
	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		pUAV = RetriveUAV(0, m_Desc.ArraySize, 0);
	}
	
	// Fill data members.
	m_pSRV = pSRV;
	m_pRTV = pRTV;
	m_pDSV = pDSV;
	m_pUAV = pUAV;

	return S_OK;
}

HRESULT CTexture1D::Create(ID3D11Device* pDevice, const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitData)
{
	Texture1DComPtr pTex;
	IG_D3D_FAILED_RETURN(pDevice->CreateTexture1D(pDesc, pInitData, pTex.GetAddressOf()), L"CTexture1D::Create: Failed to create texture1D.");

	return Create(pTex.Get());
}

HRESULT CTexture1D::Create(ID3D11Device* pDevice, UINT w, UINT mipLevels, UINT arraySize, DXGI_FORMAT format,
	UINT bindFlags, D3D11_USAGE usage, D3D11_SUBRESOURCE_DATA *pInitData)
{
	UINT cpuAccessFlags = 0;
	if (usage == D3D11_USAGE_DYNAMIC) cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) cpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	UINT miscFlags = 0;
	if (mipLevels != 1 && (bindFlags &D3D11_BIND_RENDER_TARGET) && (bindFlags &D3D11_BIND_SHADER_RESOURCE))
		miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

	mipLevels = mipLevels == 0 ? CalMipLevels(w) : mipLevels;

	CD3D11_TEXTURE1D_DESC texDesc(format, w, arraySize, mipLevels, bindFlags, usage, cpuAccessFlags, miscFlags);

	return Create(pDevice, &texDesc, pInitData);
}

void CTexture1D::Destroy()
{
	CResource::Destroy();
	m_pTex = nullptr;
	m_pSRV = nullptr;
	m_pRTV = nullptr;
	m_pUAV = nullptr;
	m_pDSV = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}
