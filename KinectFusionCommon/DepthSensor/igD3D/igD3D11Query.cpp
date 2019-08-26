#include "igD3D11Query.h"

using namespace ig::D3D11;

HRESULT CQuery::Create(ID3D11Query *pQuery)
{
	// Clear
	Destroy();

	if (!pQuery)
	{
		IG_D3D_ERROR_DUMP(L"CQuery::Create: pQuery must not be NULL.");
		return E_INVALIDARG;
	}
	// Fill data members.
	m_pQuery = pQuery;
	m_pQuery->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_pQuery->GetDesc(&m_Desc);
	return S_OK;
}

HRESULT CQuery::Create(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pDesc)
{
	QueryComPtr pQuery;
	
	IG_D3D_FAILED_RETURN(pDevice->CreateQuery(pDesc, pQuery.GetAddressOf()), L"CQuery::Create: Failed to create Query.");
	return Create(pQuery.Get());
}

HRESULT CQuery::Create(ID3D11Device* pDevice, D3D11_QUERY query, UINT miscFlags)
{
	CD3D11_QUERY_DESC desc(query, miscFlags);
	return Create(pDevice, &desc);
}

void CQuery::Destroy()
{
	m_pDevice = nullptr;
	m_pQuery = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}





