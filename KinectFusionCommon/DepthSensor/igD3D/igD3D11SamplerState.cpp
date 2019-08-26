#include "igD3D11SamplerState.h"

using namespace ig::D3D11;

HRESULT CSamplerState::Create(ID3D11SamplerState *pState)
{
	// Clear
	Destroy();
	// Validate ptr.
	if (!pState)
	{
		IG_D3D_ERROR_DUMP(L"CSamplerState::Create: pState must not be NULL.");
		return E_INVALIDARG;
	}
	// Fill members.
	m_pState = pState;
	m_pState->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_pState->GetDesc(&m_Desc);
	return S_OK;
}

HRESULT CSamplerState::Create(ID3D11Device* pDevice, const D3D11_SAMPLER_DESC *pDesc)
{
	SamplerStateComPtr pState;
	IG_D3D_FAILED_RETURN(pDevice->CreateSamplerState(pDesc, pState.GetAddressOf()), L"CSamplerState::Create: Failed to create sampler state.");
	
	return Create(pState.Get());
}

HRESULT CSamplerState::Create(ID3D11Device* pDevice, D3D11_FILTER filter,
	D3D11_TEXTURE_ADDRESS_MODE addressU,
	D3D11_TEXTURE_ADDRESS_MODE addressV,
	D3D11_TEXTURE_ADDRESS_MODE addressW,
	float mipLODBias, UINT maxAnisotropy,
	D3D11_COMPARISON_FUNC compFunc,
	float borderColorR, float borderColorG, float borderColorB, float borderColorA,
	float minLOD, float maxLOD)
{
	float borderColor[4] = { borderColorR, borderColorG, borderColorB, borderColorA };
	CD3D11_SAMPLER_DESC desc(filter, addressU, addressV, addressW, mipLODBias, maxAnisotropy, compFunc, borderColor, minLOD, maxLOD);
	return Create(pDevice, &desc);
}

void CSamplerState::Destroy()
{
	m_pDevice = nullptr;
	m_pState = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}




