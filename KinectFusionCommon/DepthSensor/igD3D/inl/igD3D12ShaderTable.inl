#pragma once

namespace ig
{
	namespace D3D12
	{	
		inline D3D12_GPU_VIRTUAL_ADDRESS CShaderTable::GPUVirtualAddress(UINT recordIdx) const
		{
			auto baseAddress = m_upBuffer->GPUVirtualAddress();
			return baseAddress + recordIdx * m_nShaderRecordSize;
		}

		inline UINT CShaderTable::NumShaderRecords() const
		{
			return m_nShaderRecords;
		}

		inline UINT CShaderTable::ShaderRecordSize() const
		{
			return m_nShaderRecordSize;
		}

		inline HRESULT CShaderTable::SetName(LPCWSTR name)
		{
			return m_upBuffer->SetName(name);
		}

		inline CBuffer* CShaderTable::BufferPtr() const 
		{ 
			return m_upBuffer.get(); 
		}

		inline CDevice* CShaderTable::DevicePtr() const 
		{ 
			return m_upBuffer->DevicePtr(); 
		}

	}
}
