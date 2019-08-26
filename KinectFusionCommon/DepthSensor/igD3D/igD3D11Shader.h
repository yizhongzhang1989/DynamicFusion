#pragma once

// Usage: wrapper for Shaders
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
		class CShader
		{
		public:
			CShader() = default;
			~CShader() = default;
			CShader(CShader&&) = default;
			CShader& operator=(CShader&&) = default;
			CShader(const CShader&) = delete;
			CShader& operator=(const CShader&) = delete;

			// Create from existing resource.
			HRESULT Create(ShaderType *pShader, const void *shaderByteCode, SIZE_T byteCodeLength);

			// General creation method
			HRESULT Create(ID3D11Device* pDevice, const void *shaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage = nullptr);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device*			DevicePtr() const;
			ShaderType*				Ptr() const;
			const void*				ByteCodePtr() const;
			SIZE_T					ByteCodeLength() const;
			bool					IsValid() const;

			// Convert to ComPtr
			operator ShaderType*() const;
	
		private:
			static LPCSTR		GenDefaultProfile();
			ComPtr<ShaderType>	CreateShader(ID3D11Device* pDevice, const void *shaderByteCode, SIZE_T byteCodeLength, ID3D11ClassLinkage* pLinkage);

			DeviceComPtr		m_pDevice;
			ComPtr<ShaderType>	m_pShader;
			std::vector<BYTE>	m_BufByteCode;
		};

		typedef CShader<ID3D11VertexShader>		CVertexShader;
		typedef CShader<ID3D11HullShader>		CHullShader;
		typedef CShader<ID3D11DomainShader>		CDomainShader;
		typedef CShader<ID3D11GeometryShader>	CGeometryShader;
		typedef CShader<ID3D11PixelShader>		CPixelShader;
		typedef CShader<ID3D11ComputeShader>	CComputeShader;
	}

}

#include "inl\igD3D11Shader.inl"
