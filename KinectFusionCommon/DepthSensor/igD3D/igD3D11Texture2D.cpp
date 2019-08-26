#include "igD3D11Texture2D.h"
#include <algorithm>
using namespace ig::D3D11;

ID3D11ShaderResourceView* CTexture2D::RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc)
{
	return RetriveD3DSRV(pDesc);
}
ID3D11RenderTargetView* CTexture2D::RetriveRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc)
{
	return RetriveD3DRTV(pDesc);
}
ID3D11UnorderedAccessView* CTexture2D::RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc)
{
	return RetriveD3DUAV(pDesc);
}
ID3D11DepthStencilView* CTexture2D::RetriveDSV(const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc)
{
	return RetriveD3DDSV(pDesc);
}

ID3D11ShaderResourceView* CTexture2D::RetriveSRV(UINT firstArraySlice, UINT arraySize, UINT firstMipLevel, UINT numMipLevels)
{
	CD3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = ig::DXGI::TypelessToFloatFormat(Desc().Format);
	if (Desc().ArraySize > 1)
	{
		if (Desc().SampleDesc.Count > 1)
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
			desc.Texture2DMSArray.ArraySize = arraySize;
			desc.Texture2DMSArray.FirstArraySlice = firstArraySlice;
		}
		else
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.ArraySize = arraySize;
			desc.Texture2DArray.FirstArraySlice = firstArraySlice;
			desc.Texture2DArray.MipLevels = numMipLevels;
			desc.Texture2DArray.MostDetailedMip = firstMipLevel;
		}
	}
	else
	{
		if (Desc().SampleDesc.Count > 1)
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = numMipLevels;
			desc.Texture2D.MostDetailedMip = firstMipLevel;
		}
	}
	return RetriveD3DSRV(&desc);


}

ID3D11RenderTargetView* CTexture2D::RetriveRTV(UINT firstArraySlice, UINT arraySize, UINT mipLevel)
{

	CD3D11_RENDER_TARGET_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = ig::DXGI::TypelessToFloatFormat(Desc().Format);
	if (Desc().ArraySize > 1)
	{
		if (Desc().SampleDesc.Count > 1)
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
			desc.Texture2DMSArray.ArraySize = arraySize;
			desc.Texture2DMSArray.FirstArraySlice = firstArraySlice;
		}
		else
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.ArraySize = arraySize;
			desc.Texture2DArray.FirstArraySlice = firstArraySlice;
			desc.Texture2DArray.MipSlice = mipLevel;
		}
	}
	else
	{
		if (Desc().SampleDesc.Count > 1)
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = mipLevel;
		}
	}
	return RetriveD3DRTV(&desc);
}

ID3D11UnorderedAccessView* CTexture2D::RetriveUAV(UINT firstArraySlice, UINT arraySize, UINT mipLevel)
{
	CD3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = ig::DXGI::TypelessToFloatFormat(Desc().Format);
	if (Desc().ArraySize > 1)
	{
		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = arraySize;
		desc.Texture2DArray.FirstArraySlice = firstArraySlice;
		desc.Texture2DArray.MipSlice = mipLevel;
	}
	else
	{

		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = mipLevel;

	}
	return RetriveD3DUAV(&desc);

}

ID3D11DepthStencilView* CTexture2D::RetriveDSV(UINT firstArraySlice, UINT arraySize, UINT mipLevel)
{
	CD3D11_DEPTH_STENCIL_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = ig::DXGI::TypelessToDepthFormat(Desc().Format);
	if (Desc().ArraySize > 1)
	{
		if (Desc().SampleDesc.Count > 1)
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
			desc.Texture2DMSArray.ArraySize = arraySize;
			desc.Texture2DMSArray.FirstArraySlice = firstArraySlice;
		}
		else
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.ArraySize = arraySize;
			desc.Texture2DArray.FirstArraySlice = firstArraySlice;
			desc.Texture2DArray.MipSlice = mipLevel;
		}
	}
	else
	{
		if (Desc().SampleDesc.Count > 1)
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = mipLevel;
		}
	}
	return RetriveD3DDSV(&desc);
}

void CTexture2D::BuildMips(ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	IG_D3D_ASSERT(Desc().MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS);
	pDC->GenerateMips(SRV());
}

// Map and unmap
HRESULT CTexture2D::Map(UINT arrayIdx, UINT level, D3D11_MAP mapType, void **ppData, UINT *pRowPitch, ID3D11DeviceContext* pOptionalDC)
{
	HRESULT hr = S_OK;
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		D3D11_MAPPED_SUBRESOURCE res;
		IG_D3D_FAILED_RETURN(pDC->Map(m_pRes.Get(), m_Desc.MipLevels * arrayIdx + level, mapType, 0, &res), L"Failed to map texture2D.");
		*ppData = res.pData;
		*pRowPitch = res.RowPitch;
		return S_OK;
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTexture2D::Map: Only dynamic/staging resource can be mapped.");
		return E_FAIL;
	}
}

void CTexture2D::Unmap(UINT arrayIdx, UINT level, ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->Unmap(m_pRes.Get(), m_Desc.MipLevels * arrayIdx + level);
}

// Update subresource
void CTexture2D::Upload(UINT arrayIdx, UINT level, UINT xOffset, UINT yOffset, UINT w, UINT h, const void* pSrcData, UINT srcRowPitch, ID3D11DeviceContext* pOptionalDC)
{
	UINT pixelSize = DXGI::GetDXGIFormatBytes(Desc().Format);
	if (pixelSize == 0) return;
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	CD3D11_BOX box(xOffset, yOffset, 0, xOffset + w, yOffset + h, 1);

	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		// Use map.
		BYTE *pData = nullptr;
		UINT rowPitch = 0;
		D3D11_MAP mapType = D3D11_USAGE_DYNAMIC == Desc().Usage ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE;
		Map(arrayIdx, level, mapType, (void**)&pData, &rowPitch, pDC);
		for (UINT j = 0; j < h; ++j)
		{
			memcpy(pData + xOffset * pixelSize + (yOffset + j) * rowPitch, (BYTE*)pSrcData + j * srcRowPitch, w * pixelSize);
		}
		Unmap(arrayIdx, level, pDC);
	}
	else if (D3D11_USAGE_IMMUTABLE != Desc().Usage)
	{
		pDC->UpdateSubresource(m_pRes.Get(), Desc().MipLevels * arrayIdx + level, &box, pSrcData, srcRowPitch, 0);
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTexture2D::Upload: Such resource does not support upload operation.");
	}
}

HRESULT CTexture2D::Create(ID3D11Texture2D *pTex, ID2D1DeviceContext *pD2D1DC, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties)
{
	// Destroy
	Destroy();	
	
	// Call base-class creation.
	IG_D3D_FAILED_RETURN(CResource::Create(pTex), L"CTexture2D::Create: CResource::Create() failed.");

	// Resource
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
	
	DXGI::SurfaceComPtr pDXGISurface;
	//IG_D3D_FAILED_RETURN(pTex->QueryInterface(pDXGISurface.GetAddressOf()), L"CTexture2D::Create: Failed to query IDXGISurface interface.");
	ComPtr<ID2D1Bitmap1> pBitmap;
	if (pD2D1DC)
	{
		IG_D3D_FAILED_RETURN(pTex->QueryInterface(pDXGISurface.GetAddressOf()), L"CTexture2D::Create: Failed to query IDXGISurface interface.");
		IG_D3D_FAILED_RETURN(pD2D1DC->CreateBitmapFromDxgiSurface(pDXGISurface.Get(), pD2D1BitmapProperties, pBitmap.GetAddressOf()), 
			L"CTexture2D::Create: Failed to create d2d1bitmap.");
	}	

	// Fill data members.	
	m_pSRV = pSRV;
	m_pRTV = pRTV;
	m_pDSV = pDSV;
	m_pUAV = pUAV;

	//m_pDXGISurface = pDXGISurface;
	m_pD2D1DC = pD2D1DC;
	m_pD2D1Bitmap1 = pBitmap;

	return S_OK;
}

HRESULT CTexture2D::Create(ID3D11Device* pDevice, const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitData, 
	ID2D1DeviceContext *pD2D1DC, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties)
{
	Texture2DComPtr pTex;
	IG_D3D_FAILED_RETURN(pDevice->CreateTexture2D(pDesc, pInitData, pTex.GetAddressOf()), L"CTexture2D::Create: Failed to create texture2D.");

	return Create(pTex.Get(), pD2D1DC, pD2D1BitmapProperties);
}

HRESULT CTexture2D::Create(ID3D11Device* pDevice, UINT w, UINT h, UINT mipLevels, UINT arraySize, DXGI_FORMAT format,
	UINT bindFlags, D3D11_USAGE usage, D3D11_SUBRESOURCE_DATA *pInitData)
{
	UINT cpuAccessFlags = 0;
	if (usage == D3D11_USAGE_DYNAMIC) cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) cpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	UINT miscFlags = 0;
	if (mipLevels != 1 && (bindFlags &D3D11_BIND_RENDER_TARGET) && (bindFlags &D3D11_BIND_SHADER_RESOURCE))
		miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

	mipLevels = mipLevels == 0 ? std::max<UINT>(CalMipLevels(w), CalMipLevels(h)) : mipLevels;

	CD3D11_TEXTURE2D_DESC texDesc(format, w, h, arraySize, mipLevels, bindFlags, usage, cpuAccessFlags, 1, 0, miscFlags);

	return Create(pDevice, &texDesc, pInitData);
}

// Create multisample texture2D (arraySize = 1)
HRESULT CTexture2D::CreateMS(ID3D11Device* pDevice, UINT w, UINT h, UINT arraySize, DXGI_FORMAT format,
	UINT sampleCount, UINT sampleQuality, UINT bindFlags, D3D11_USAGE usage)
{
	UINT cpuAccessFlags = 0;
	if (usage == D3D11_USAGE_DYNAMIC) cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) cpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	CD3D11_TEXTURE2D_DESC texDesc(format, w, h, arraySize, 1, bindFlags, usage, cpuAccessFlags, sampleCount, sampleQuality, 0);
	return Create(pDevice, &texDesc, nullptr);
}

HRESULT CTexture2D::CreateWithD2D1Support(ID3D11Device* pDevice, ID2D1DeviceContext *pD2D1DC, UINT w, UINT h, DXGI_FORMAT format,
	UINT bindFlags, D3D11_USAGE usage, D3D11_SUBRESOURCE_DATA *pInitData, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties)
{
	if (!pD2D1DC)
	{
		IG_D3D_ERROR_DUMP(L"CTexture2D::CreateWithD2D1Support: pD2D1DC must not be NULL.");
		return E_INVALIDARG;
	}

	UINT cpuAccessFlags = 0;
	if (usage == D3D11_USAGE_DYNAMIC) cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) cpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	CD3D11_TEXTURE2D_DESC texDesc(format, w, h, 1, 1, bindFlags, usage, cpuAccessFlags, 1, 0, 0);

	return Create(pDevice, &texDesc, pInitData, pD2D1DC, pD2D1BitmapProperties);
}

void CTexture2D::Destroy()
{
	CResource::Destroy();
	m_pTex = nullptr;
	m_pSRV = nullptr;
	m_pRTV = nullptr;
	m_pUAV = nullptr;
	m_pDSV = nullptr;
	//m_pDXGISurface = nullptr;
	m_pD2D1Bitmap1 = nullptr;
	m_pD2D1DC = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}
