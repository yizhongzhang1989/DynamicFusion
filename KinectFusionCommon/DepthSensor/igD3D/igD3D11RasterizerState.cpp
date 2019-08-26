#include "igD3D11RasterizerState.h"

using namespace ig::D3D11;

HRESULT CRasterizerState::Create(ID3D11RasterizerState *pState)
{
	// Clear
	Destroy();
	// Validate ptr.
	if (!pState)
	{
		IG_D3D_ERROR_DUMP(L"CRasterizerState::Create: pState must not be NULL.");
		return E_INVALIDARG;
	}
	// Fill members.
	m_pState = pState;
	m_pState->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_pState->GetDesc(&m_Desc);
	return S_OK;
}

HRESULT CRasterizerState::Create(ID3D11Device* pDevice, const D3D11_RASTERIZER_DESC *pDesc)
{
	RasterizerStateComPtr pState;
	IG_D3D_FAILED_RETURN(pDevice->CreateRasterizerState(pDesc, pState.GetAddressOf()),
		L"CRasterizerState::Create: Failed to create rasterizer state.");

	return Create(pState.Get());
}

HRESULT CRasterizerState::Create(ID3D11Device* pDevice, D3D11_FILL_MODE fillMode,
	D3D11_CULL_MODE cullMode, bool bIsFrontCounterClockwise,
	int depthBias, float depthBiasClamp, float slopScaledDepthBias,
	bool bEnableDepthClip, bool bEnableScissor, bool bEnableMultisample,
	bool bEnableAntialiasedLine)
{
	CD3D11_RASTERIZER_DESC desc(fillMode, cullMode, (bIsFrontCounterClockwise ? TRUE : FALSE),
		depthBias, depthBiasClamp, slopScaledDepthBias,
		(bEnableDepthClip ? TRUE : FALSE),
		(bEnableScissor ? TRUE : FALSE),
		(bEnableMultisample ? TRUE : FALSE),
		(bEnableAntialiasedLine ? TRUE : FALSE));

	return Create(pDevice, &desc);
}

void CRasterizerState::Destroy()
{
	m_pDevice = nullptr;
	m_pState = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

