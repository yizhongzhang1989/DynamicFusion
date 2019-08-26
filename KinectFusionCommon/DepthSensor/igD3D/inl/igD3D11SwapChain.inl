#pragma once


namespace ig
{
	namespace D3D11
	{
			// Getters
		inline ID3D11Device* CSwapChain::DevicePtr() const { return m_pDevice.Get(); }
		inline ID3D11Texture2D* CSwapChain::BackBufferPtr() const { return m_pTex.Get(); }
		inline ID3D11RenderTargetView* CSwapChain::RTV() const { return m_pRTV.Get(); }
		inline IDXGISwapChain* CSwapChain::Ptr() const { return m_pSwapChain.Get(); }
		inline IDXGISurface* CSwapChain::DXGISurfacePtr() const { return m_pDXGISurface.Get(); }
		inline ID2D1Bitmap1* CSwapChain::D2D1Bitmap1Ptr() const { return m_pD2D1Bitmap1.Get(); }
		inline ID2D1DeviceContext*CSwapChain::D2D1DeviceContextPtr() const { return m_pD2D1DC.Get(); }
		inline const DXGI_SWAP_CHAIN_DESC& CSwapChain::Desc() const { return m_Desc; }
		inline bool CSwapChain::IsValid() const { return m_pSwapChain != nullptr; }

		inline void CSwapChain::Present(UINT syncInterval, UINT flags)
		{
			m_pSwapChain->Present(syncInterval, flags);
		}

		// Convert to ComPtr
		inline CSwapChain::operator IDXGISwapChain*() const { return m_pSwapChain.Get(); }

	}
}
