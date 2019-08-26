#include "igD3D11TextureCube.h"

using namespace ig::D3D11;

ID3D11ShaderResourceView* CTextureCube::RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc)
{
	return RetriveD3DSRV(pDesc);
}

ID3D11RenderTargetView* CTextureCube::RetriveRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc)
{
	return RetriveD3DRTV(pDesc);
}

ID3D11UnorderedAccessView* CTextureCube::RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc)
{
	return RetriveD3DUAV(pDesc);
}
ID3D11DepthStencilView* CTextureCube::RetriveDSV(const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc)
{
	return RetriveD3DDSV(pDesc);
}

ID3D11ShaderResourceView* CTextureCube::RetriveSRV(UINT firstMipLevel, UINT numMipLevels)
{
	CD3D11_SHADER_RESOURCE_VIEW_DESC desc(D3D11_SRV_DIMENSION_TEXTURECUBE, ig::DXGI::TypelessToFloatFormat(Desc().Format), firstMipLevel, numMipLevels, 0, 6, 0);
	return RetriveD3DSRV(&desc);
}

ID3D11RenderTargetView* CTextureCube::RetriveRTV(UINT mipLevel)
{
	CD3D11_RENDER_TARGET_VIEW_DESC desc(D3D11_RTV_DIMENSION_TEXTURE2DARRAY, ig::DXGI::TypelessToFloatFormat(Desc().Format), mipLevel, 0, 6);
	return RetriveD3DRTV(&desc);
}

ID3D11RenderTargetView*  CTextureCube::RetriveRTV(UINT faceIdx, UINT mipLevel)
{
	CD3D11_RENDER_TARGET_VIEW_DESC desc(D3D11_RTV_DIMENSION_TEXTURE2DARRAY, ig::DXGI::TypelessToFloatFormat(Desc().Format), mipLevel, faceIdx, 1);
	return RetriveD3DRTV(&desc);
}

ID3D11UnorderedAccessView* CTextureCube::RetriveUAV(UINT mipLevel)
{
	CD3D11_UNORDERED_ACCESS_VIEW_DESC desc(D3D11_UAV_DIMENSION_TEXTURE2DARRAY, ig::DXGI::TypelessToFloatFormat(Desc().Format), mipLevel, 0, 6);	
	return RetriveD3DUAV(&desc);

}

ID3D11DepthStencilView* CTextureCube::RetriveDSV(UINT mipLevel)
{
	CD3D11_DEPTH_STENCIL_VIEW_DESC desc(D3D11_DSV_DIMENSION_TEXTURE2DARRAY, ig::DXGI::TypelessToDepthFormat(Desc().Format), mipLevel, 0, 6);
	return RetriveD3DDSV(&desc);
}

void CTextureCube::BuildMips(ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->GenerateMips(SRV());
}

// Map and unmap
HRESULT CTextureCube::Map(UINT faceIdx, UINT level, D3D11_MAP mapType, void **ppData, UINT *pRowPitch, ID3D11DeviceContext *pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		D3D11_MAPPED_SUBRESOURCE res;
		IG_D3D_FAILED_RETURN(pDC->Map(m_pRes.Get(), m_Desc.MipLevels * faceIdx + level, mapType, 0, &res), L"CTextureCube::Map: Failed to map textureCube.");
		*ppData = res.pData;
		*pRowPitch = res.RowPitch;
		return S_OK;
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTextureCube::Map: Only dynamic/staging resource can be mapped.");
		return E_FAIL;
	}
}

void CTextureCube::Unmap(UINT faceIdx, UINT level, ID3D11DeviceContext *pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->Unmap(m_pRes.Get(), m_Desc.MipLevels * faceIdx + level);
}

// Update subresource
void CTextureCube::Upload(UINT faceIdx, UINT level, UINT xOffset, UINT yOffset, UINT w, UINT h, const void* pSrcData, UINT srcRowPitch, ID3D11DeviceContext *pOptionalDC)
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
		Map(faceIdx, level, mapType, (void**)&pData, &rowPitch, pDC);
		for (UINT j = 0; j < h; ++j)
		{
			memcpy(pData + xOffset * pixelSize + (yOffset + j) * rowPitch, (BYTE*)pSrcData + j * srcRowPitch, w * pixelSize);
		}
		Unmap(faceIdx, level, pDC);
	}
	else if (D3D11_USAGE_IMMUTABLE != Desc().Usage)
	{
		pDC->UpdateSubresource(m_pRes.Get(), Desc().MipLevels * faceIdx + level, &box, pSrcData, srcRowPitch, 0);
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CTextureCube::Upload: Such resource does not support upload operation.");
	}

	/*CD3D11_BOX box(xOffset, yOffset, 0, xOffset + w, yOffset + h, 1);
	if (D3D11_USAGE_DYNAMIC != Desc().Usage && D3D11_USAGE_IMMUTABLE != Desc().Usage)
	{
	pDC->UpdateSubresource(m_pRes.Get(), m_Desc.MipLevels * faceIdx + level, &box, pSrcData, srcRowPitch, 0);
	}*/
}

HRESULT CTextureCube::Create(ID3D11Texture2D *pTex)
{
	// Destroy
	Destroy();

	// Call base-class creation.
	IG_D3D_FAILED_RETURN(CResource::Create(pTex), L"CTextureCube::Create: CResource::Create() failed.");

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
		pSRV = RetriveSRV(0, m_Desc.MipLevels);
	}
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		pRTV = RetriveRTV((UINT)0);
	}
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		pDSV = RetriveDSV((UINT)0);
	}
	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		pUAV = RetriveUAV((UINT)0);
	}
	// Fill data members.
	m_pSRV = pSRV;
	m_pRTV = pRTV;
	m_pUAV = pUAV;
	m_pDSV = pDSV;
	return S_OK;
}

HRESULT CTextureCube::Create(ID3D11Device *pDevice, UINT size, UINT mipLevels, DXGI_FORMAT format,
	UINT bindFlags, D3D11_USAGE usage, D3D11_SUBRESOURCE_DATA *pInitData)
{
	UINT cpuAccessFlags = 0;
	if (usage == D3D11_USAGE_DYNAMIC) cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) cpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	UINT miscFlags = 0;
	if (mipLevels != 1 && (bindFlags &D3D11_BIND_RENDER_TARGET) && (bindFlags &D3D11_BIND_SHADER_RESOURCE))
		miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// for Cube
	miscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

	mipLevels = mipLevels == 0 ? CalMipLevels(size) : mipLevels;

	CD3D11_TEXTURE2D_DESC texDesc(format, size, size, 6, mipLevels, bindFlags, usage, cpuAccessFlags, 1, 0, miscFlags);

	TextureCubeComPtr pTex;

	IG_D3D_FAILED_RETURN(pDevice->CreateTexture2D(&texDesc, pInitData, pTex.GetAddressOf()), L"CTextureCube::Create: Failed to create textureCube.");

	return Create(pTex.Get());
}

void CTextureCube::Destroy()
{
	CResource::Destroy();
	m_pTex = nullptr;
	m_pSRV = nullptr;
	m_pRTV = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}