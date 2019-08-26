#include "igD3D11BlendState.h"

using namespace ig::D3D11;

HRESULT CBlendState::Create(ID3D11BlendState *pState)
{
	// Clear
	Destroy();	
	// Validate ptr.
	if (!pState)
	{
		IG_D3D_ERROR_DUMP(L"CBlendState::Create: pState must not be NULL.");
		return E_INVALIDARG;
	}

	// Fill members.
	m_pState = pState;
	m_pState->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_pState->GetDesc(&m_Desc);
	return S_OK;
}


HRESULT CBlendState::Create(ID3D11Device* pDevice, const D3D11_BLEND_DESC *pDesc)
{
	BlendStateComPtr pState;
	IG_D3D_FAILED_RETURN(pDevice->CreateBlendState(pDesc, pState.GetAddressOf()), L"CBlendState::Create: Failed to create blend state.");
	
	return Create(pState.Get());
}


// simple blend
HRESULT CBlendState::Create(ID3D11Device* pDevice, bool bEnableBlend, bool bEnableAlphaToCoverage,
	D3D11_BLEND srcBlend, D3D11_BLEND dstBlend, D3D11_BLEND_OP blendOp,
	D3D11_BLEND srcBlendAlpha, D3D11_BLEND dstBlendAlpha, D3D11_BLEND_OP blendOpAlpha, UINT8 RTWriteMask)
{
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = bEnableAlphaToCoverage;
	desc.IndependentBlendEnable = false;
	for (UINT i = 0; i < 8; ++i)
	{
		desc.RenderTarget[i].BlendEnable = bEnableBlend ? TRUE : FALSE;
		desc.RenderTarget[i].SrcBlend = srcBlend;
		desc.RenderTarget[i].DestBlend = dstBlend;
		desc.RenderTarget[i].BlendOp = blendOp;
		desc.RenderTarget[i].SrcBlendAlpha = srcBlendAlpha;
		desc.RenderTarget[i].DestBlendAlpha = dstBlendAlpha;
		desc.RenderTarget[i].BlendOpAlpha = blendOpAlpha;
		desc.RenderTarget[i].RenderTargetWriteMask = RTWriteMask;
	}
	return Create(pDevice, &desc);
}

void CBlendState::Destroy()
{
	m_pDevice = nullptr;
	m_pState = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}


