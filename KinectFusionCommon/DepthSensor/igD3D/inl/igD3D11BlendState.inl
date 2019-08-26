#pragma once

namespace ig
{
	namespace D3D11
	{
			// Getters
		inline ID3D11Device* CBlendState::DevicePtr() const { return m_pDevice.Get(); }
		inline ID3D11BlendState* CBlendState::Ptr() const { return m_pState.Get(); }
		inline const D3D11_BLEND_DESC& CBlendState::Desc() const { return m_Desc; }
		inline bool CBlendState::IsValid() const { return m_pState != nullptr; }

			// Convert to ComPtr
		inline CBlendState::operator ID3D11BlendState*() const { return m_pState.Get(); }
	}
}