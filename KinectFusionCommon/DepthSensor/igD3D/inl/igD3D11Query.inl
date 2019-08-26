#pragma once

namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Device* CQuery::DevicePtr() const { return m_pDevice.Get(); }
		inline ID3D11Query* CQuery::Ptr() const { return m_pQuery.Get(); }
		inline const D3D11_QUERY_DESC& CQuery::Desc() const { return m_Desc; }
		inline bool CQuery::IsValid() const { return m_pQuery != nullptr; }

		// Convert to ComPtr
		inline CQuery::operator ID3D11Query*() const { return m_pQuery.Get(); }

	}

}
