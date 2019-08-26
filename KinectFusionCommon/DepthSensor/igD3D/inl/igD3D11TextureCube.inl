#pragma once

namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Texture2D* CTextureCube::Ptr() const { return m_pTex.Get(); }
		// Get Default SRV
		inline ID3D11ShaderResourceView* CTextureCube::SRV() const { return m_pSRV.Get(); }
		// Get Default RTV
		inline ID3D11RenderTargetView* CTextureCube::RTV() const { return m_pRTV.Get(); }
		// Get Default UAV
		inline ID3D11UnorderedAccessView* CTextureCube::UAV() const { return m_pUAV.Get(); }
		// Get Default DSV
		inline ID3D11DepthStencilView* CTextureCube::DSV() const { return m_pDSV.Get(); }

		inline const D3D11_TEXTURE2D_DESC& CTextureCube::Desc() const { return m_Desc; }

		// Convert to ComPtr
		inline CTextureCube::operator ID3D11Texture2D*() const { return m_pTex.Get(); }

	}
}

