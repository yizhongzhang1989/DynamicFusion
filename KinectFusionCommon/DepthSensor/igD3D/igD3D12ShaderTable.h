#pragma once
// Usage: D3D12 ShaderTable for DXR
// Created: 2019.6.3
// 2019.6.3: Initial Version.
// Version: 0.1.190603.1800

#include <d3d12.h>
#include <vector>
#include <DirectXMath.h>
#include "igD3D12Defs.h"
#include "igD3D12Device.h"
#include "igD3D12Buffer.h"

namespace ig
{
	namespace D3D12
	{
		class CBuffer;

		class CShaderTable
		{
		public:
			CShaderTable(
				CDevice::sPtr spDevice,
				UINT nShaderRecords,
				UINT nShaderRecordSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
				UINT nShaderRecordAlignment = D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);

		public:
			typedef std::shared_ptr<CShaderTable> sPtr;
			typedef std::unique_ptr<CShaderTable> uPtr;

			static uPtr Create(
				CDevice::sPtr spDevice,
				UINT nShaderRecords,
				UINT nShaderRecordSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
				UINT nShaderRecordAlignment = D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);

			void SetShaderRecord(
				UINT recordIdx,
				const void *pShaderIdentifier,
				const void *pLocalRootArguments = nullptr,
				UINT localRootArgumentsSize = 0);

			D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress(UINT recordIdx = 0) const;
			
			UINT NumShaderRecords() const;
			
			UINT ShaderRecordSize() const;
			
			HRESULT SetName(LPCWSTR name);
			
			CBuffer* BufferPtr() const; 
			CDevice* DevicePtr() const;

		private:
			ig::D3D12::CBuffer::uPtr m_upBuffer;
			UINT m_nShaderRecords;
			UINT m_nShaderRecordSize;
		};
	}
}

#include "inl\igD3D12ShaderTable.inl"