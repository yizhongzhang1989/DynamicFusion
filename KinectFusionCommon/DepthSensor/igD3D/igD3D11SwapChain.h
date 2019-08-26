#pragma once

// Usage: wrapper for SwapChain
// Created: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// 2018.5.4: Improved D2D1 support.
// 2019.3.11: Added initial support for Flip model.
// Version: 1.0.190311.1600

#include "igD3D11Defs.h"
#include <d2d1_1.h>

namespace ig
{
	namespace D3D11
	{
		class CSwapChain
		{
		public:
			CSwapChain() = default;
			~CSwapChain() = default;
			CSwapChain(CSwapChain&&) = default;
			CSwapChain& operator=(CSwapChain&&) = default;
			CSwapChain(const CSwapChain&) = delete;
			CSwapChain& operator=(const CSwapChain&) = delete;
			
			// Create from existing resource.
			HRESULT Create(IDXGISwapChain *pSwapChain, ID2D1DeviceContext *pD2D1DC = nullptr, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties = nullptr);

			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, ID2D1DeviceContext *pD2D1DC = nullptr, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties = nullptr);

			// Specialized creation method for Non-MSAA swap chain.
			HRESULT Create(ID3D11Device* pDevice, HWND hWnd, UINT w, UINT h, bool bIsWindowed = true, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_USAGE usage = DXGI_USAGE_RENDER_TARGET_OUTPUT, UINT refreshRate = 60, UINT bufferCount = 2);

			// Specialized creation method for MSAA swap chain.
			HRESULT CreateMS(ID3D11Device* pDevice, HWND hWnd, UINT w, UINT h, UINT sampleCount, UINT sampleQuality, bool bIsWindowed = true, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_USAGE usage = DXGI_USAGE_RENDER_TARGET_OUTPUT, UINT refreshRate = 60, UINT bufferCount = 2);

			// Specialized creation method for Non-MSAA swap chain with D2D1 support. default format = BGRA32
			HRESULT CreateWithD2D1Support(ID3D11Device* pDevice, ID2D1DeviceContext *pD2D1DC, HWND hWnd, UINT w, UINT h, bool bIsWindowed = true, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM,
				DXGI_USAGE usage = DXGI_USAGE_RENDER_TARGET_OUTPUT, UINT refreshRate = 60, UINT bufferCount = 2, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties = nullptr);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device*					DevicePtr() const;
			IDXGISwapChain*					Ptr() const;
			ID3D11Texture2D*				BackBufferPtr() const;
			ID3D11RenderTargetView*			RTV() const;
			IDXGISurface*					DXGISurfacePtr() const;
			ID2D1Bitmap1*					D2D1Bitmap1Ptr() const; // Only available when create with valid D2D1DeviceContext.
			ID2D1DeviceContext*				D2D1DeviceContextPtr() const;
			//ID3D11RenderTargetView*			RTV() const;
			const DXGI_SWAP_CHAIN_DESC&		Desc() const;
			bool							IsValid() const;
			
			// Convert to ComPtr
			operator IDXGISwapChain*() const;

			// Operations.
			HRESULT Resize(UINT w, UINT h, DXGI_FORMAT newFormat = DXGI_FORMAT_UNKNOWN);
			void Present(UINT syncInterval = 0, UINT flags = 0);
			

		private:
			DeviceComPtr			m_pDevice;
			DXGI::SwapChainComPtr	m_pSwapChain;
			Texture2DComPtr			m_pTex;
			RenderTargetViewComPtr	m_pRTV;
			DXGI::SurfaceComPtr		m_pDXGISurface;
			ComPtr<ID2D1Bitmap1>		m_pD2D1Bitmap1;
			ComPtr<ID2D1DeviceContext> m_pD2D1DC;
			DXGI_SWAP_CHAIN_DESC	m_Desc = {}; // Zero initialization. 
			//D2D1_BITMAP_PROPERTIES1 m_D2D1BitmapProp;
		};

	}
}

#include "inl\igD3D11SwapChain.inl"
