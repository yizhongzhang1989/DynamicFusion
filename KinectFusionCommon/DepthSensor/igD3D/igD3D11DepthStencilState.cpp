#include "igD3D11DepthStencilState.h"

using namespace ig::D3D11;

HRESULT CDepthStencilState::Create(ID3D11DepthStencilState *pState)
{
	// Clear
	Destroy();

	if (!pState)
	{
		IG_D3D_ERROR_DUMP(L"CDepthStencilState::Create: pState must not be NULL.");
		return E_INVALIDARG;
	}
	// Fill data members.
	m_pState = pState;
	m_pState->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_pState->GetDesc(&m_Desc);
	return S_OK;
}

HRESULT CDepthStencilState::Create(ID3D11Device* pDevice, const D3D11_DEPTH_STENCIL_DESC *pDesc)
{
	DepthStencilStateComPtr pState;
	IG_D3D_FAILED_RETURN(pDevice->CreateDepthStencilState(pDesc, pState.GetAddressOf()), L"CDepthStencilState::Create: Failed to create depth-stencil state.");	
	return Create(pState.Get());
}

HRESULT CDepthStencilState::Create(ID3D11Device* pDevice, bool bEnableDepth, bool bEnableDepthWrite, D3D11_COMPARISON_FUNC depthFunc,
	bool bEnableStencil, UINT8 stencilReadMask, UINT8 stencilWriteMask,
	D3D11_STENCIL_OP frontStencilFailOp, D3D11_STENCIL_OP frontStencilDepthFailOp,
	D3D11_STENCIL_OP frontStencilPassOp, D3D11_COMPARISON_FUNC frontStencilFunc,
	D3D11_STENCIL_OP backStencilFailOp, D3D11_STENCIL_OP backStencilDepthFailOp,
	D3D11_STENCIL_OP backStencilPassOp, D3D11_COMPARISON_FUNC backStencilFunc)
{
	CD3D11_DEPTH_STENCIL_DESC desc((bEnableDepth ? TRUE : FALSE), (bEnableDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO), depthFunc,
		(bEnableStencil ? TRUE : FALSE), stencilReadMask, stencilWriteMask,
		frontStencilFailOp, frontStencilDepthFailOp, frontStencilPassOp, frontStencilFunc,
		backStencilFailOp, backStencilDepthFailOp, backStencilPassOp, backStencilFunc);

	return Create(pDevice, &desc);
}

void CDepthStencilState::Destroy()
{
	m_pDevice = nullptr;
	m_pState = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}





