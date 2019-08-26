#pragma once

namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Device* CInputLayout::DevicePtr() const { return m_pDevice.Get(); }
		inline ID3D11InputLayout* CInputLayout::Ptr() const { return m_pIA.Get(); }
		inline const std::vector<D3D11_INPUT_ELEMENT_DESC>& CInputLayout::Desc() const { return m_Desc; }
		inline bool CInputLayout::IsValid() const { return m_pIA != nullptr; }

		// Convert to ComPtr
		inline CInputLayout::operator ID3D11InputLayout*() const { return m_pIA.Get(); }

	}

}


