#pragma once

// Usage: D3D12 Texture2D
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// Version: 0.1.180929.2215

#include "igD3D12Resource.h"

namespace ig
{
	namespace D3D12
	{
		
		class CTexture2D : public CResource
		{
		public:
			CTexture2D(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

			CTexture2D(CDevice::sPtr spDevice,
				const D3D12_HEAP_PROPERTIES *pHeapProperties,
				D3D12_HEAP_FLAGS heapFlags,
				const D3D12_RESOURCE_DESC *pResourceDesc,
				D3D12_RESOURCE_STATES initialResourceState,
				const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr);

			// Commonly-used non-msaa constructor
			CTexture2D(
				CDevice::sPtr spDevice,
				UINT w, UINT h,
				UINT mipLevels,
				UINT arraySize,
				DXGI_FORMAT format,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_COMMON,
				const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		public:
			typedef std::shared_ptr<CTexture2D> sPtr;
			typedef std::unique_ptr<CTexture2D> uPtr;

			static uPtr Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

			static uPtr Create(CDevice::sPtr spDevice,
				const D3D12_HEAP_PROPERTIES *pHeapProperties,
				D3D12_HEAP_FLAGS heapFlags,
				const D3D12_RESOURCE_DESC *pResourceDesc,
				D3D12_RESOURCE_STATES initialResourceState);

			// Commonly-used non-msaa creation method
			static uPtr Create(CDevice::sPtr spDevice,
				UINT w, UINT h,
				UINT mipLevels,
				UINT arraySize,
				DXGI_FORMAT format,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_COMMON,
				const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void UploadSync(
				UINT arrayIdx, UINT level,
				const void* pSrcData, UINT srcRowPitch,
				D3D12_RESOURCE_STATES afterState);
													
			void ReadbackSync(
				UINT arrayIdx, UINT level,
				std::vector<uint8_t> &dstData,
				UINT &width, UINT &height,
				UINT &rowPitch,
				D3D12_RESOURCE_STATES afterState);
		};		
	}
}

#include "inl\igD3D12Texture2D.inl"


