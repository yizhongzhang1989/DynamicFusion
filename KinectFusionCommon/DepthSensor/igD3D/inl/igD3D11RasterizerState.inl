#pragma once

namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Device* CRasterizerState::DevicePtr() const { return m_pDevice.Get(); }
		inline ID3D11RasterizerState* CRasterizerState::Ptr() const { return m_pState.Get(); }
		inline const D3D11_RASTERIZER_DESC& CRasterizerState::Desc() const { return m_Desc; }
		inline bool CRasterizerState::IsValid() const { return m_pState != nullptr; }

		// Convert to ComPtr
		inline CRasterizerState::operator ID3D11RasterizerState*() const { return m_pState.Get(); }

	}

}
