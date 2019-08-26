#pragma once

namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Device* CSamplerState::DevicePtr() const { return m_pDevice.Get(); }
		inline ID3D11SamplerState* CSamplerState::Ptr() const { return m_pState.Get(); }
		inline const D3D11_SAMPLER_DESC& CSamplerState::Desc() const { return m_Desc; }
		inline bool CSamplerState::IsValid() const { return m_pState != nullptr; }

		// Convert to ComPtr
		inline CSamplerState::operator ID3D11SamplerState*() const { return m_pState.Get(); }

	}
}


