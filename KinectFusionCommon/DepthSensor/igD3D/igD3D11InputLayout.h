#pragma once

// Usage: wrapper for InputLayout
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"
#include <vector>

namespace ig
{
	namespace D3D11
	{
		template<typename ShaderType>
		class CShader;

		class CInputLayout
		{
		public:
			CInputLayout() = default;
			~CInputLayout() = default;
			CInputLayout(CInputLayout&&) = default;
			CInputLayout& operator=(CInputLayout&&) = default;
			CInputLayout(const CInputLayout&) = delete;
			CInputLayout& operator=(const CInputLayout&) = delete;

			// Create from existing resource.
			HRESULT Create(ID3D11InputLayout *pIA, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT numElements);

			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT numElements, const void* pShaderByteCode, SIZE_T codeLength);

			// Create from VS
			HRESULT Create(ID3D11Device* pDevice, const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT numElements, const CShader<ID3D11VertexShader>& VS);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device*									DevicePtr() const;
			ID3D11InputLayout*								Ptr() const;
			const std::vector<D3D11_INPUT_ELEMENT_DESC>&	Desc() const;
			bool											IsValid() const;

			// Convert to ComPtr
			operator ID3D11InputLayout*() const;

		private:
			DeviceComPtr							m_pDevice;
			InputLayoutComPtr						m_pIA;
			std::vector<D3D11_INPUT_ELEMENT_DESC>	m_Desc;
		};

	}

}

#include "inl\igD3D11InputLayout.inl"
