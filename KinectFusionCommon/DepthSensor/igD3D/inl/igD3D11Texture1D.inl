#pragma once


namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Texture1D* CTexture1D::Ptr() const { return m_pTex.Get(); }
		// Get Default SRV
		inline ID3D11ShaderResourceView* CTexture1D::SRV() const { return m_pSRV.Get(); }
		// Get Default RTV
		inline ID3D11RenderTargetView* CTexture1D::RTV() const { return m_pRTV.Get(); }
		// Get Default UAV
		inline ID3D11UnorderedAccessView* CTexture1D::UAV() const { return m_pUAV.Get(); }
		// Get Default DSV
		inline ID3D11DepthStencilView* CTexture1D::DSV() const { return m_pDSV.Get(); }

		inline const D3D11_TEXTURE1D_DESC& CTexture1D::Desc() const { return m_Desc; }

		// Convert to ComPtr
		inline CTexture1D::operator ID3D11Texture1D*() const { return m_pTex.Get(); }


	}
}
