#pragma once

// Usage: simple wrapper for buffers
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"
#include "igD3D11Resource.h"
namespace ig
{
	namespace D3D11
	{
		// General Buffer.
		class CBuffer : public CResource
		{
		public:
			CBuffer() = default;
			~CBuffer() = default;
			CBuffer(CBuffer&&) = default;
			CBuffer& operator=(CBuffer&&) = default;
			CBuffer(const CBuffer&) = delete;
			CBuffer& operator=(const CBuffer&) = delete;

			// Create from existing resource.
			// defaultViewFormat: the format used to create default views (e.g. srv, uav)
			HRESULT Create(ID3D11Buffer *pBuf, DXGI_FORMAT defaultViewFormat = DXGI_FORMAT_UNKNOWN);

			// General Creation Method.
			// defaultViewFormat: the format used to create default views (e.g. srv, uav)
			HRESULT Create(ID3D11Device* pDevice, const D3D11_BUFFER_DESC *pDesc, DXGI_FORMAT defaultViewFormat = DXGI_FORMAT_UNKNOWN,
				const void *pData = nullptr);

			// Typical creator for vertex buffer/index buffer/constant buffer/typed buffer/raw buffer.
			// defaultViewFormat: the format used to create default views (e.g. srv, uav)
			HRESULT Create(ID3D11Device* pDevice, UINT bytesWidth,
				DXGI_FORMAT defaultViewFormat = DXGI_FORMAT_UNKNOWN,
				UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT, void *pData = nullptr, bool bAllowRawView = false);

			// For structured buffer only.
			// defaultViewFormat: the format used to create default views (e.g. srv, uav)
			HRESULT CreateStructured(ID3D11Device *pDevice, UINT numElements, UINT bytesPerElement,
				UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT, void *pData = nullptr);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Buffer*					Ptr() const;
			ID3D11ShaderResourceView*		SRV() const;
			ID3D11UnorderedAccessView*		UAV() const;
			ID3D11ShaderResourceView*		RawSRV() const;
			ID3D11UnorderedAccessView*		RawUAV() const;
			const D3D11_BUFFER_DESC&		Desc() const;

			// Convert to ComPtr
			operator ID3D11Buffer*() const;

			// For general views.
			ID3D11ShaderResourceView*		RetriveSRV(DXGI_FORMAT viewFormat, UINT firstElement, UINT numElements);
			ID3D11UnorderedAccessView*		RetriveUAV(DXGI_FORMAT viewFormat, UINT firstElement, UINT numElements, UINT flags = 0);


			ID3D11ShaderResourceView*		RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc);
			ID3D11UnorderedAccessView*		RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc);

			
			// Operations
			HRESULT Map(D3D11_MAP mapType, void **ppData, ID3D11DeviceContext *pOptionalDC = nullptr);

			void Unmap(ID3D11DeviceContext *pOptionalDC = nullptr);

			void Upload(UINT offsetInBytes, UINT sizeInBytes, const void* pSrcData, ID3D11DeviceContext *pOptionalDC = nullptr);

			void Download(UINT offsetInBytes, UINT sizeInBytes, void* pDstData, ID3D11DeviceContext *pOptionalDC = nullptr);

		private:
			BufferComPtr				m_pBuf;
			ShaderResourceViewComPtr	m_pSRV;
			UnorderedAccessViewComPtr	m_pUAV;
			ShaderResourceViewComPtr	m_pRawSRV; // for raw buffer.
			UnorderedAccessViewComPtr	m_pRawUAV;
			D3D11_BUFFER_DESC			m_Desc = {}; // Zero initialization.

		};

	}

}

#include "inl\igD3D11Buffer.inl"
