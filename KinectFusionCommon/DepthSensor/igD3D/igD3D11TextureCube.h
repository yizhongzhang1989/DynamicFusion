#pragma once

// Usage: CTextureCube class
// Created: 2018.4.28
// Last Modified: 2019.5.15
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// 2019.5.15: Added supports for DSV and UAV.
// Version: 1.0.190515.2015

#include "igD3D11Resource.h"

namespace ig
{
	namespace D3D11
	{
		// Currently DO-NOT support cube array.
		class CTextureCube : public CResource
		{
		public:
			CTextureCube() = default;
			~CTextureCube() = default;
			CTextureCube(CTextureCube&&) = default;
			CTextureCube& operator=(CTextureCube&&) = default;
			CTextureCube(const CTextureCube&) = delete;
			CTextureCube& operator=(const CTextureCube&) = delete;

			// Create from existing resource.
			HRESULT Create(ID3D11Texture2D *pTex);

			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, UINT size, UINT mipLevels, DXGI_FORMAT format,
				UINT bindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_SUBRESOURCE_DATA *pInitData = nullptr);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Texture2D*				Ptr() const;
			ID3D11ShaderResourceView*		SRV() const;
			ID3D11RenderTargetView*			RTV() const;
			ID3D11UnorderedAccessView*		UAV() const;
			ID3D11DepthStencilView*			DSV() const;
			const D3D11_TEXTURE2D_DESC&		Desc() const;

			// Convert to ComPtr
			operator ID3D11Texture2D*() const;

			// More general cases.
			ID3D11ShaderResourceView*	RetriveSRV(UINT firstMipLevel, UINT numMipLevels);
			ID3D11RenderTargetView*		RetriveRTV(UINT mipLevel);
			ID3D11RenderTargetView*		RetriveRTV(UINT faceIdx, UINT mipLevel);
			ID3D11UnorderedAccessView*	RetriveUAV(UINT mipLevel);
			ID3D11DepthStencilView*		RetriveDSV(UINT mipLevel);


			ID3D11ShaderResourceView*	RetriveSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc);
			ID3D11RenderTargetView*		RetriveRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc);
			ID3D11UnorderedAccessView*	RetriveUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc);
			ID3D11DepthStencilView*		RetriveDSV(const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc);
			
			// Operations.
			void BuildMips(ID3D11DeviceContext *pOptionalDC = nullptr);

			HRESULT Map(UINT faceIdx, UINT level, D3D11_MAP mapType, void **ppData, UINT *pRowPitch, ID3D11DeviceContext *pOptionalDC = nullptr);

			void Unmap(UINT faceIdx, UINT level, ID3D11DeviceContext *pOptionalDC = nullptr);

			void Upload(UINT faceIdx, UINT level, UINT xOffset, UINT yOffset, UINT w, UINT h, const void* pSrcData, UINT srcRowPitch, ID3D11DeviceContext *pOptionalDC = nullptr);


		private:
			Texture2DComPtr				m_pTex;
			ShaderResourceViewComPtr	m_pSRV;
			RenderTargetViewComPtr		m_pRTV;
			UnorderedAccessViewComPtr	m_pUAV;
			DepthStencilViewComPtr		m_pDSV;
			D3D11_TEXTURE2D_DESC		m_Desc = {}; // Zero initialization. 
		};
	}
}

#include "inl\igD3D11TextureCube.inl"
