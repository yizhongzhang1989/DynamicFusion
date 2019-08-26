#pragma once

// Usage: D3D12 TextureCube
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// Version: 0.1.180929.2215

#include "igD3D12Defs.h"
#include "igD3D12Resource.h"

namespace ig
{
	namespace D3D12
	{	
		class CTextureCube : public CResource
		{
		public:
			CTextureCube(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

			CTextureCube(
				CDevice::sPtr spDevice,
				UINT size,
				UINT mipLevels,
				DXGI_FORMAT format,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_COMMON,
				const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);		

			typedef std::shared_ptr<CTextureCube> sPtr;
			typedef std::unique_ptr<CTextureCube> uPtr;

			static uPtr Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

			// Commonly-used non-msaa creation method
			static uPtr Create(
				CDevice::sPtr spDevice,
				UINT size,
				UINT mipLevels,
				DXGI_FORMAT format,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_COMMON,
				const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);
			
			void UploadSync(
				UINT mipLevel,
				const void* pSrcData[6], UINT srcRowPitch[6],
				D3D12_RESOURCE_STATES afterState);

			void ReadbackSync(
				UINT mipLevel,
				std::vector<uint8_t> &dstData,
				UINT &resolution, UINT &rowPitch,
				UINT &facePitch,
				D3D12_RESOURCE_STATES afterState);
		};
		
	}
}

#include "inl\igD3D12TextureCube.inl"


