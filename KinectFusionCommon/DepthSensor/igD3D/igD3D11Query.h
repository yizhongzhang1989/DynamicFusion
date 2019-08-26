#pragma once

// Usage: simple wrapper for Query
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"

namespace ig
{
	namespace D3D11
	{
		class CQuery
		{
		public:
			CQuery() = default;
			~CQuery() = default;
			CQuery(CQuery&&) = default;
			CQuery& operator=(CQuery&&) = default;
			CQuery(const CQuery&) = delete;
			CQuery& operator=(const CQuery&) = delete;

			// Create from existing pDevice ptr.
			HRESULT Create(ID3D11Query *pQuery);

			// General creation method.
			HRESULT Create(ID3D11Device *pDevice, const D3D11_QUERY_DESC *pDesc);

			HRESULT Create(ID3D11Device *pDevice, D3D11_QUERY query, UINT miscFlags = 0);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device* DevicePtr()	const;
			ID3D11Query* Ptr()	const;
			bool IsValid() const;
			const D3D11_QUERY_DESC&		Desc() const;

			// Convert to ComPtr
			operator ID3D11Query*() const;

		private:
			DeviceComPtr		m_pDevice;
			QueryComPtr			m_pQuery;
			D3D11_QUERY_DESC	m_Desc = {}; // Zero initialization. 
		};

	}

}

#include "inl\igD3D11Query.inl"
