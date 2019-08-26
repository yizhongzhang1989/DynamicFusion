#pragma once

// Usage: CTexture1D class
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Resource.h"
#include <string>

namespace ig
{
	namespace D3D11
	{

		class CTexture1D : public CResource
		{
		public:
			CTexture1D() = default;
			~CTexture1D() = default;
			CTexture1D(CTexture1D&&) = default;
			CTexture1D& operator=(CTexture1D&&) = default;
			CTexture1D(const CTexture1D&) = delete;
			CTexture1D& operator=(const CTexture1D&) = delete;
			
			// Create from existing resource.
			HRESULT Create(ID3D11Texture1D *pTex);

			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitData = nullptr);

			HRESULT Create(ID3D11Device* pDevice, UINT w, UINT mipLevels, UINT arraySize, DXGI_FORMAT format,
				UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_SUBRESOURCE_DATA *pInitData = nullptr);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Texture1D*				Ptr() const;
			ID3D11ShaderResourceView*		SRV() const;
			ID3D11RenderTargetView*			RTV() const;
			ID3D11UnorderedAccessView*		UAV() const;
			ID3D11DepthStencilView*			DSV() const;
			const D3D11_TEXTURE1D_DESC&		Desc() const;
			
			// Convert to ComPtr
			operator ID3D11Texture1D*() const;

			// For general views.
			ID3D11ShaderResourceView*	RetriveSRV(UINT firstArraySlice, UINT arraySize, UINT firstMipLevel, UINT numMipLevels);
			ID3D11RenderTargetView*		RetriveRTV(UINT firstArraySlice, UINT arraySize, UINT mipLevel);
			ID3D11UnorderedAccessView*	RetriveUAV(UINT firstArraySlice, UINT arraySize, UINT mipLevel);
			ID3D11DepthStencilView*		RetriveDSV(UINT firstArraySlice, UINT arraySize, UINT mipLevel);

			ID3D11ShaderResourceView*	RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc);
			ID3D11RenderTargetView*		RetriveRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc);
			ID3D11UnorderedAccessView*	RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc);
			ID3D11DepthStencilView*		RetriveDSV(const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc);

			// Operations
			void BuildMips(ID3D11DeviceContext* pOptionalDC = nullptr);

			HRESULT Map(UINT arrayIdx, UINT level, D3D11_MAP mapType, void **ppData, ID3D11DeviceContext* pOptionalDC = nullptr);
			
			void Unmap(UINT arrayIdx, UINT level, ID3D11DeviceContext* pOptionalDC = nullptr);

			void Upload(UINT arrayIdx, UINT level, UINT xOffset, UINT w, const void* pSrcData, ID3D11DeviceContext* pOptionalDC = nullptr);

		private:
			Texture1DComPtr				m_pTex;
			ShaderResourceViewComPtr	m_pSRV;
			RenderTargetViewComPtr		m_pRTV;
			UnorderedAccessViewComPtr	m_pUAV;
			DepthStencilViewComPtr		m_pDSV;
			D3D11_TEXTURE1D_DESC		m_Desc = {}; // Zero initialization. 


		};

	}
}

#include "inl\igD3D11Texture1D.inl"