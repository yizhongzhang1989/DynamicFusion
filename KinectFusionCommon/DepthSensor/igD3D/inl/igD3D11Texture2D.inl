#pragma once


namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Texture2D* CTexture2D::Ptr() const { return m_pTex.Get(); }
		// Get Default SRV
		inline ID3D11ShaderResourceView* CTexture2D::SRV() const { return m_pSRV.Get(); }
		// Get Default RTV
		inline ID3D11RenderTargetView* CTexture2D::RTV() const { return m_pRTV.Get(); }
		// Get Default UAV
		inline ID3D11UnorderedAccessView* CTexture2D::UAV() const { return m_pUAV.Get(); }
		// Get Default DSV
		inline ID3D11DepthStencilView* CTexture2D::DSV() const { return m_pDSV.Get(); }

		//inline IDXGISurface* CTexture2D::DXGISurfacePtr() const { return m_pDXGISurface.Get(); }

		inline ID2D1Bitmap1* CTexture2D::D2D1Bitmap1Ptr() const { return m_pD2D1Bitmap1.Get(); }

		inline ID2D1DeviceContext* CTexture2D::D2D1DeviceContextPtr() const { return m_pD2D1DC.Get(); }

		inline const D3D11_TEXTURE2D_DESC& CTexture2D::Desc() const { return m_Desc; }

		// Convert to ComPtr
		inline CTexture2D::operator ID3D11Texture2D*() const { return m_pTex.Get(); }
	}
}