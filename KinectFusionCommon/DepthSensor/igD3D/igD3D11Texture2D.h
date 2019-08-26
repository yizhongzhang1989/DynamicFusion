#pragma once

// Usage: CTexture2D class
// Created: 2018.4.28
// Last Modified: 2018.5.4
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// 2018.5.4: Added initial support for D2D1
// Version: 1.0.180504.1500

#include "igD3D11Resource.h"
#include <string>
#include <d2d1_1.h>

namespace ig
{
	namespace D3D11
	{

		class CTexture2D : public CResource
		{	
		public:
			CTexture2D() = default;
			~CTexture2D() = default;
			CTexture2D(CTexture2D&&) = default;
			CTexture2D& operator=(CTexture2D&&) = default;
			CTexture2D(const CTexture2D&) = delete;
			CTexture2D& operator=(const CTexture2D&) = delete;
			
			// Create from existing resource.
			HRESULT Create(ID3D11Texture2D *pTex, ID2D1DeviceContext *pD2D1DC = nullptr, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties = nullptr);

			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitData = nullptr, 
				ID2D1DeviceContext *pD2D1DC = nullptr, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties = nullptr);

			// Specialized creation method for Non-MSAA texture.
			HRESULT Create(ID3D11Device* pDevice, UINT w, UINT h, UINT mipLevels, UINT arraySize, DXGI_FORMAT format,
				UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_SUBRESOURCE_DATA *pInitData = nullptr);

			// Specialized creation method for MSAA texture.
			HRESULT CreateMS(ID3D11Device* pDevice, UINT w, UINT h, UINT arraySize, DXGI_FORMAT format,
				UINT sampleCount, UINT sampleQuality,
				UINT bindFlags = D3D11_BIND_RENDER_TARGET, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

			// Specialized creation method for Non-MSAA texture with D2D1 support. Only supports texture with arraySize = 1 and mipLevel = 1.
			HRESULT CreateWithD2D1Support(ID3D11Device* pDevice, ID2D1DeviceContext *pD2D1DC, UINT w, UINT h, DXGI_FORMAT format,
				UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE usage = D3D11_USAGE_DEFAULT, 
				D3D11_SUBRESOURCE_DATA *pInitData = nullptr, 
				D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties = nullptr);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Texture2D*				Ptr() const;
			ID3D11ShaderResourceView*		SRV() const;
			ID3D11RenderTargetView*			RTV() const;
			ID3D11UnorderedAccessView*		UAV() const;
			ID3D11DepthStencilView*			DSV() const;
			const D3D11_TEXTURE2D_DESC&		Desc() const;
			//IDXGISurface*					DXGISurfacePtr() const;
			ID2D1Bitmap1*					D2D1Bitmap1Ptr() const; // Only available when create with valid D2D1DeviceContext.
			ID2D1DeviceContext*				D2D1DeviceContextPtr() const;

			// Convert to ComPtr
			operator ID3D11Texture2D*() const;

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

			HRESULT Map(UINT arrayIdx, UINT level, D3D11_MAP mapType, void **ppData, UINT *pRowPitch, ID3D11DeviceContext* pOptionalDC = nullptr);

			void Unmap(UINT arrayIdx, UINT level, ID3D11DeviceContext* pOptionalDC = nullptr);

			void Upload(UINT arrayIdx, UINT level, UINT xOffset, UINT yOffset, UINT w, UINT h, const void* pSrcData, UINT srcRowPitch, ID3D11DeviceContext* pOptionalDC = nullptr);


		private:
			Texture2DComPtr				m_pTex;
			ShaderResourceViewComPtr	m_pSRV;
			RenderTargetViewComPtr		m_pRTV;
			UnorderedAccessViewComPtr	m_pUAV;
			DepthStencilViewComPtr		m_pDSV;
			//DXGI::SurfaceComPtr			m_pDXGISurface;
			ComPtr<ID2D1Bitmap1>		m_pD2D1Bitmap1;
			ComPtr<ID2D1DeviceContext>	m_pD2D1DC;
			D3D11_TEXTURE2D_DESC		m_Desc = {}; // Zero initialization. 
		};

	}
}

#include "inl\igD3D11Texture2D.inl"