#pragma once

// Usage: CTexture3D class
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

		class CTexture3D : public CResource
		{
		public:
			// Default construct/copy/assign behaviors.
			CTexture3D() = default;
			~CTexture3D() = default;
			CTexture3D(CTexture3D&&) = default;
			CTexture3D& operator=(CTexture3D&&) = default;
			CTexture3D(const CTexture3D&) = delete;
			CTexture3D& operator=(const CTexture3D&) = delete;

			// Create from existing resource.
			HRESULT Create(ID3D11Texture3D *pTex);

			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitData = nullptr);

			HRESULT Create(ID3D11Device* pDevice, UINT w, UINT h, UINT d, UINT mipLevels, DXGI_FORMAT format,
				UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_SUBRESOURCE_DATA *pInitData = nullptr);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Texture3D*				Ptr() const;
			ID3D11ShaderResourceView*		SRV() const;
			ID3D11RenderTargetView*			RTV() const;
			ID3D11UnorderedAccessView*		UAV() const;
			const D3D11_TEXTURE3D_DESC&		Desc() const;

			// Convert to ComPtr
			operator ID3D11Texture3D*() const;

			ID3D11ShaderResourceView*	RetriveSRV(UINT firstMipLevel, UINT numMipLevels);
			ID3D11RenderTargetView*		RetriveRTV(UINT firstDepthSlice, UINT nSlices, UINT mipLevel);
			ID3D11UnorderedAccessView*	RetriveUAV(UINT firstDepthSlice, UINT nSlices, UINT mipLevel);

			ID3D11ShaderResourceView*	RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc);
			ID3D11RenderTargetView*		RetriveRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc);
			ID3D11UnorderedAccessView*	RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc);

			
			// Operations
			void BuildMips(ID3D11DeviceContext* pOptionalDC = nullptr);

			HRESULT Map(UINT level, D3D11_MAP mapType, void **ppData, UINT *pRowPitch, UINT *pDepthPitch, ID3D11DeviceContext* pOptionalDC = nullptr);

			void Unmap(UINT level, ID3D11DeviceContext* pOptionalDC = nullptr);

			void Upload(UINT level, UINT xOffset, UINT yOffset, UINT zOffset, UINT w, UINT h, UINT d, const void* pSrcData, UINT srcRowPitch, UINT srcDepthPitch, ID3D11DeviceContext* pOptionalDC = nullptr);

		private:
			Texture3DComPtr				m_pTex;
			ShaderResourceViewComPtr	m_pSRV;
			RenderTargetViewComPtr		m_pRTV;
			UnorderedAccessViewComPtr	m_pUAV;
			D3D11_TEXTURE3D_DESC		m_Desc = {}; // Zero initialization. 
		};

	}
}

#include "inl\igD3D11Texture3D.inl"
