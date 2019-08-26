#pragma once



namespace ig
{
	namespace D3D11
	{
			// Getters
		inline ID3D11Texture3D* CTexture3D::Ptr() const { return m_pTex.Get(); }
			// Get Default SRV
		inline ID3D11ShaderResourceView* CTexture3D::SRV() const { return m_pSRV.Get(); }
			// Get Default RTV
		inline ID3D11RenderTargetView* CTexture3D::RTV() const { return m_pRTV.Get(); }
			// Get Default UAV
		inline ID3D11UnorderedAccessView* CTexture3D::UAV() const { return m_pUAV.Get(); }

		inline const D3D11_TEXTURE3D_DESC& CTexture3D::Desc() const { return m_Desc; }

			// Convert to ComPtr
		inline CTexture3D::operator ID3D11Texture3D*() const { return m_pTex.Get(); }

	}
}

