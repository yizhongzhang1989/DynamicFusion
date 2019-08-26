#include "igD3D11InputLayout.h"
#include "igD3D11Shader.h"

using namespace ig::D3D11;

HRESULT CInputLayout::Create(ID3D11InputLayout *pIA, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT numElements)
{
	// Clear
	Destroy();
	
	// Validate ptr.
	if (!pIA || !pInputElementDescs || numElements == 0)
	{
		IG_D3D_ERROR_DUMP(L"CInputLayout::Create: Incorrect input parameters.");
		return E_INVALIDARG;
	}

	// Fill data member.
	m_pIA = pIA;
	m_pIA->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_Desc.resize(numElements);
	for (std::vector<D3D11_INPUT_ELEMENT_DESC>::size_type i = 0; i != numElements; ++i)
	{
		m_Desc[i] = pInputElementDescs[i];
	}
	return S_OK;
}

HRESULT CInputLayout::Create(ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT numElements, const void* pShaderByteCode, SIZE_T codeLength)
{
	InputLayoutComPtr pIA;
	IG_D3D_FAILED_RETURN(pDevice->CreateInputLayout(pInputElementDescs, numElements, pShaderByteCode, codeLength, pIA.GetAddressOf()), L"CInputLayout::Create: Failed to create input layout.");
	return Create(pIA.Get(), pInputElementDescs, numElements);
}

HRESULT CInputLayout::Create(ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT numElements, const CShader<ID3D11VertexShader>& VS)
{
	return Create(pDevice, pInputElementDescs, numElements, VS.ByteCodePtr(), VS.ByteCodeLength());
}

void CInputLayout::Destroy()
{
	m_pDevice = nullptr;
	m_pIA = nullptr;
	m_Desc.clear();
}


