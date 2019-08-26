#pragma once

namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Device* CDepthStencilState::DevicePtr() const { return m_pDevice.Get(); }
		inline ID3D11DepthStencilState* CDepthStencilState::Ptr() const { return m_pState.Get(); }
		inline const D3D11_DEPTH_STENCIL_DESC& CDepthStencilState::Desc() const { return m_Desc; }
		inline bool CDepthStencilState::IsValid() const { return m_pState != nullptr; }

		// Convert to ComPtr
		inline CDepthStencilState::operator ID3D11DepthStencilState*() const { return m_pState.Get(); }

	}
}