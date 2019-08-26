#pragma once


namespace ig
{
	namespace D3D11
	{
			// Getters
		template<typename ShaderType>
		inline ID3D11Device* CShader<ShaderType>::DevicePtr() const { return m_pDevice.Get(); }
		template<typename ShaderType>
		inline ShaderType* CShader<ShaderType>::Ptr() const { return m_pShader.Get(); }
		template<typename ShaderType>
		inline const void* CShader<ShaderType>::ByteCodePtr() const { return (void*)m_BufByteCode.data(); }
		template<typename ShaderType>
		inline SIZE_T CShader<ShaderType>::ByteCodeLength() const { return m_BufByteCode.size(); }
		template<typename ShaderType>
		inline bool CShader<ShaderType>::IsValid() const { return m_pShader != nullptr; }

			// Convert to ComPtr
		template<typename ShaderType>
		inline CShader<ShaderType>::operator ShaderType*() const { return m_pShader.Get(); }

	}

}
