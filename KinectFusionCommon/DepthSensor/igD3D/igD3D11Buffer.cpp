#include "igD3D11Buffer.h"

using namespace ig::D3D11;

ID3D11ShaderResourceView* CBuffer::RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc)
{
	return RetriveD3DSRV(pDesc);
}

ID3D11UnorderedAccessView* CBuffer::RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc)
{
	return RetriveD3DUAV(pDesc);
}

ID3D11ShaderResourceView* CBuffer::RetriveSRV(DXGI_FORMAT viewFormat, UINT firstElement, UINT numElements)
{
	CD3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = viewFormat;
	
	if (viewFormat == DXGI_FORMAT_R32_TYPELESS)
	{
		if (!(Desc().MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS))
		{
			IG_D3D_ERROR_DUMP(L"Raw view needs D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS to be set.");
		}
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		desc.BufferEx.FirstElement = firstElement;
		desc.BufferEx.NumElements = numElements;
		desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	}
	else
	{
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = firstElement;
		desc.Buffer.NumElements = numElements;
	}
	return RetriveD3DSRV(&desc);
}

ID3D11UnorderedAccessView* CBuffer::RetriveUAV(DXGI_FORMAT viewFormat, UINT firstElement, UINT numElements, UINT flags)
{
	CD3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = firstElement;
	desc.Buffer.NumElements = numElements;
	desc.Buffer.Flags = flags;
	desc.Format = viewFormat;

	return RetriveD3DUAV(&desc);
}

// Create from existing resource.
HRESULT CBuffer::Create(ID3D11Buffer *pBuf, DXGI_FORMAT defaultViewFormat)
{
	// Destroy
	Destroy();

	// Call base-class creation.
	IG_D3D_FAILED_RETURN(CResource::Create(pBuf), L"CBuffer::Create: CResource::Create() failed.");

	// Resource
	m_pBuf = pBuf;
	m_pBuf->GetDesc(&m_Desc);

	// Get desired views.
	ShaderResourceViewComPtr pSRV;
	UnorderedAccessViewComPtr pUAV;
	ShaderResourceViewComPtr pRawSRV;
	UnorderedAccessViewComPtr pRawUAV;

	UINT bAllowRaw = m_Desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	UINT bIsStructured = m_Desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		if (!bAllowRaw && defaultViewFormat == DXGI_FORMAT_R32_TYPELESS)
		{
			IG_D3D_ERROR_DUMP(L"CBuffer::Create: DXGI_FORMAT_R32_TYPELESS can only be appled to raw SRV.");
		}
		if (bAllowRaw)
		{
			pRawSRV = RetriveSRV(DXGI_FORMAT_R32_TYPELESS, 0, m_Desc.ByteWidth / 4);
			pSRV = pRawSRV;
		}
		if (defaultViewFormat != DXGI_FORMAT_R32_TYPELESS)
		{
			UINT bytesPerElement = DXGI::GetDXGIFormatBytes(defaultViewFormat);
			if (bIsStructured)
			{
				bytesPerElement = m_Desc.StructureByteStride;
			}
			if (bytesPerElement > 0)
			{
				pSRV = RetriveSRV(defaultViewFormat, 0, m_Desc.ByteWidth / bytesPerElement);
			}
		}
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		if (!bAllowRaw && defaultViewFormat == DXGI_FORMAT_R32_TYPELESS)
		{
			IG_D3D_ERROR_DUMP(L"CBuffer::Create: DXGI_FORMAT_R32_TYPELESS can only be appled to raw UAV.");
		}
		if (bAllowRaw)
		{
			pRawUAV = RetriveUAV(DXGI_FORMAT_R32_TYPELESS, 0, m_Desc.ByteWidth / 4, D3D11_BUFFER_UAV_FLAG_RAW);
			pUAV = pRawUAV;
		}
		if (defaultViewFormat != DXGI_FORMAT_R32_TYPELESS)
		{
			UINT bytesPerElement = DXGI::GetDXGIFormatBytes(defaultViewFormat);
			if (bIsStructured)
			{
				bytesPerElement = m_Desc.StructureByteStride;
			}
			if (bytesPerElement > 0)
			{
				pUAV = RetriveUAV(defaultViewFormat, 0, m_Desc.ByteWidth / bytesPerElement, 0);
			}
		}
	}

	
	m_pSRV = pSRV;
	m_pUAV = pUAV;
	m_pRawSRV = pRawSRV;
	m_pRawUAV = pRawUAV;
	return S_OK;
}

HRESULT CBuffer::Create(ID3D11Device* pDevice, const D3D11_BUFFER_DESC *pDesc, DXGI_FORMAT defaultViewFormat, const void *pData)
{
	BufferComPtr pBuf;
	if (pData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = pData;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;
		IG_D3D_FAILED_RETURN(pDevice->CreateBuffer(pDesc, &data, pBuf.GetAddressOf()), L"CBuffer::Create: Failed to create buffer.");
	}
	else
	{
		IG_D3D_FAILED_RETURN(pDevice->CreateBuffer(pDesc, NULL, pBuf.GetAddressOf()), L"CBuffer::Create: Failed to create buffer.");
	}
	return Create(pBuf.Get(), defaultViewFormat);
}

HRESULT CBuffer::Create(ID3D11Device* pDevice, UINT byteWidth, DXGI_FORMAT defaultViewFormat,
	UINT bindFlags, D3D11_USAGE usage, void *pData, bool bAllowRawView)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = bindFlags;
	desc.ByteWidth = byteWidth;
	if (usage == D3D11_USAGE_DYNAMIC) desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	if (bAllowRawView) desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	desc.Usage = usage;
	return Create(pDevice, &desc, defaultViewFormat, pData);
}

void CBuffer::Destroy()
{
	CResource::Destroy();
	m_pBuf = nullptr;
	m_pSRV = nullptr;
	m_pUAV = nullptr;
	m_pRawSRV = nullptr; // for raw buffer.
	m_pRawUAV = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

// For structured buffer.
HRESULT CBuffer::CreateStructured(ID3D11Device *pDevice, UINT numElements, UINT bytesPerElement,
	UINT bindFlags, D3D11_USAGE usage, void *pData)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = bindFlags;
	desc.ByteWidth = numElements * bytesPerElement;
	if (usage == D3D11_USAGE_DYNAMIC) desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (usage == D3D11_USAGE_STAGING) desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = bytesPerElement;
	desc.Usage = usage;
	return Create(pDevice, &desc, DXGI_FORMAT_UNKNOWN, pData);
}


HRESULT CBuffer::Map(D3D11_MAP mapType, void** ppData, ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	if (D3D11_USAGE_DYNAMIC == Desc().Usage || D3D11_USAGE_STAGING == Desc().Usage)
	{
		D3D11_MAPPED_SUBRESOURCE res;
		IG_D3D_FAILED_RETURN(pDC->Map(m_pBuf.Get(), 0, mapType, 0, &res), L"CBuffer::Map: Failed to map buffer.");
		*ppData = res.pData;
		return S_OK;
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CBuffer::Map: Only dynamic/staging resource can be mapped.");
		return E_FAIL;
	}
}

void CBuffer::Unmap(ID3D11DeviceContext* pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	pDC->Unmap(m_pBuf.Get(), 0);
}

void CBuffer::Upload(UINT offsetInBytes, UINT sizeInBytes, const void* pSrcData, ID3D11DeviceContext *pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	if (Desc().CPUAccessFlags & D3D11_CPU_ACCESS_WRITE)
	{
		// Use map.
		BYTE *pData = nullptr;
		D3D11_MAP mapType = D3D11_USAGE_DYNAMIC == Desc().Usage ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE;
		Map(mapType, (void**)&pData, pDC);
		if (pData)
		{
			memcpy(pData + offsetInBytes, pSrcData, sizeInBytes);
			Unmap(pDC);
		}
	}
	else if (D3D11_USAGE_IMMUTABLE != Desc().Usage && D3D11_USAGE_DYNAMIC != Desc().Usage)
	{
		if (Desc().BindFlags & D3D11_BIND_CONSTANT_BUFFER)
		{
			if (!(offsetInBytes == 0 && sizeInBytes == Desc().ByteWidth))
			{
				IG_D3D_ERROR_DUMP(L"CBuffer::Upload: Shader-constant buffer only supports full update.");
				return;
			}
			pDC->UpdateSubresource(m_pBuf.Get(), 0, nullptr, pSrcData, 0, 0);
		}
		else
		{
			CD3D11_BOX box(offsetInBytes, 0, 0, offsetInBytes + sizeInBytes, 1, 1);
			pDC->UpdateSubresource(m_pBuf.Get(), 0, &box, pSrcData, 0, 0);
		}
	}
	else
	{
		IG_D3D_ERROR_DUMP(L"CBuffer::Upload: Such resource does not support upload operation.");
	}
}

void CBuffer::Download(UINT offsetInBytes, UINT sizeInBytes, void* pDstData, ID3D11DeviceContext *pOptionalDC)
{
	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
	if (Desc().CPUAccessFlags & D3D11_CPU_ACCESS_READ)
	{
		// Use map.
		BYTE *pData = nullptr;
		D3D11_MAP mapType = D3D11_MAP_READ;
		Map(mapType, (void**)&pData, pDC);
		if (pData)
		{
			memcpy(pDstData, pData + offsetInBytes, sizeInBytes);
			Unmap(pDC);
		}
	}
	else
	{
		CBuffer tmpBuf;
		if (SUCCEEDED(tmpBuf.Create(DevicePtr(), Desc().ByteWidth, DXGI_FORMAT_UNKNOWN, 0, D3D11_USAGE_STAGING, nullptr, false)))
		{
			CopyTo(tmpBuf, pDC);
			BYTE *pData = nullptr;
			D3D11_MAP mapType = D3D11_MAP_READ;
			tmpBuf.Map(mapType, (void**)&pData, pDC);
			if (pData)
			{
				memcpy(pDstData, pData + offsetInBytes, sizeInBytes);
				tmpBuf.Unmap(pDC);
			}
		}
	}
}

//void CBuffer::CopyTo(ID3D11Buffer *pDstBuf, ID3D11DeviceContext *pOptionalDC)
//{
//	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
//	pDC->CopyResource(pDstBuf, Ptr());
//}
//
//void CBuffer::CopyFrom(ID3D11Buffer *pSrcBuf, ID3D11DeviceContext *pOptionalDC)
//{
//	ID3D11DeviceContext* pDC = pOptionalDC == nullptr ? m_pImmediateDC.Get() : pOptionalDC;
//	pDC->CopyResource(Ptr(), pSrcBuf);
//}