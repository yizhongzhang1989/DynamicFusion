#include "igD3D11Shader.h"

using namespace ig::D3D11;

template<typename ShaderType>
ComPtr<ShaderType> CShader<ShaderType>::CreateShader(ID3D11Device* pDevice, const void *shaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	IG_D3D_ASSERT(false, L"Unsupported shader type.");
}

template<typename ShaderType>
HRESULT CShader<ShaderType>::Create(ShaderType *pShader, const void *shaderByteCode, SIZE_T byteCodeLength)
{
	// Clear
	Destroy();
	// Validate ptr.
	if (!pShader || !shaderByteCode || byteCodeLength == 0)
	{
		IG_D3D_ERROR_DUMP(L"CShader<ShaderType>::Create: Incorrect input parameters.");
		return E_INVALIDARG;
	}

	// Fill data member.
	m_pShader = pShader;
	m_pShader->GetDevice(m_pDevice.ReleaseAndGetAddressOf());
	m_BufByteCode.resize(byteCodeLength);
	memcpy_s(m_BufByteCode.data(), byteCodeLength, shaderByteCode, byteCodeLength);
	return S_OK;
}

template<typename ShaderType>
HRESULT CShader<ShaderType>::Create(ID3D11Device* pDevice, const void *shaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	ComPtr<ShaderType> pShader = CreateShader(pDevice, shaderByteCode, byteCodeLength, pLinkage);
	if (!pShader)
	{
		IG_D3D_ERROR_DUMP(L"CShader<ShaderType>::Create: Failed to create shader.");
		return E_FAIL;
	}
	return Create(pShader.Get(), shaderByteCode, byteCodeLength);
}


template<typename ShaderType>
void CShader<ShaderType>::Destroy()
{
	m_pDevice = nullptr;
	m_pShader = nullptr;
	m_BufByteCode.clear();
}

template<>
LPCSTR CShader<ID3D11VertexShader>::GenDefaultProfile()
{
	return "vs_5_0";
}

template<>
LPCSTR CShader<ID3D11HullShader>::GenDefaultProfile()
{
	return "hs_5_0";
}

template<>
LPCSTR CShader<ID3D11DomainShader>::GenDefaultProfile()
{
	return "ds_5_0";
}

template<>
LPCSTR CShader<ID3D11GeometryShader>::GenDefaultProfile()
{
	return "gs_5_0";
}

template<>
LPCSTR CShader<ID3D11PixelShader>::GenDefaultProfile()
{
	return "ps_5_0";
}

template<>
LPCSTR CShader<ID3D11ComputeShader>::GenDefaultProfile()
{
	return "cs_5_0";
}

template<>
inline ComPtr<ID3D11VertexShader> CShader<ID3D11VertexShader>::CreateShader(ID3D11Device* pDevice, const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	ComPtr<ID3D11VertexShader> ret;
	if (FAILED(pDevice->CreateVertexShader(pShaderByteCode, byteCodeLength, pLinkage, ret.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CShader<ID3D11VertexShader>::CreateShader: Failed to create vertex shader.");
		return nullptr;
	}
	return ret;
}

template<>
inline ComPtr<ID3D11HullShader> CShader<ID3D11HullShader>::CreateShader(ID3D11Device* pDevice, const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	ComPtr<ID3D11HullShader> ret;
	if (FAILED(pDevice->CreateHullShader(pShaderByteCode, byteCodeLength, pLinkage, ret.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CShader<ID3D11HullShader>::CreateShader: Failed to create hull shader.");
		return nullptr;
	}
	return ret;
}

template<>
inline ComPtr<ID3D11DomainShader> CShader<ID3D11DomainShader>::CreateShader(ID3D11Device* pDevice, const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	ComPtr<ID3D11DomainShader> ret;
	if (FAILED(pDevice->CreateDomainShader(pShaderByteCode, byteCodeLength, pLinkage, ret.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CShader<ID3D11HullShader>::CreateShader: Failed to create domain shader.");
		return nullptr;
	}
	return ret;
}

template<>
inline ComPtr<ID3D11GeometryShader> CShader<ID3D11GeometryShader>::CreateShader(ID3D11Device* pDevice, const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	ComPtr<ID3D11GeometryShader> ret;
	if (FAILED(pDevice->CreateGeometryShader(pShaderByteCode, byteCodeLength, pLinkage, ret.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CShader<ID3D11HullShader>::CreateShader: Failed to create geometry shader.");
		return nullptr;
	}
	return ret;
}

template<>
inline ComPtr<ID3D11PixelShader> CShader<ID3D11PixelShader>::CreateShader(ID3D11Device* pDevice, const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	ComPtr<ID3D11PixelShader> ret;
	if (FAILED(pDevice->CreatePixelShader(pShaderByteCode, byteCodeLength, pLinkage, ret.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CShader<ID3D11HullShader>::CreateShader: Failed to create pixel shader.");
		return nullptr;
	}
	return ret;
}

template<>
inline ComPtr<ID3D11ComputeShader> CShader<ID3D11ComputeShader>::CreateShader(ID3D11Device* pDevice, const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage)
{
	ComPtr<ID3D11ComputeShader> ret;
	if (FAILED(pDevice->CreateComputeShader(pShaderByteCode, byteCodeLength, pLinkage, ret.GetAddressOf())))
	{
		IG_D3D_ERROR_DUMP(L"CShader<ID3D11HullShader>::CreateShader: Failed to create compute shader.");
		return nullptr;
	}
	return ret;
}


template class CShader<ID3D11VertexShader>;
template class CShader<ID3D11HullShader>;
template class CShader<ID3D11DomainShader>;
template class CShader<ID3D11GeometryShader>;
template class CShader<ID3D11PixelShader>;
template class CShader<ID3D11ComputeShader>;