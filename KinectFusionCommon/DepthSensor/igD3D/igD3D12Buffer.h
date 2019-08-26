#pragma once

#include "igD3D12Resource.h"

namespace ig
{
	namespace D3D12
	{
		
		class CBuffer : public CResource
		{
		public:
			CBuffer(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

			CBuffer(CDevice::sPtr spDevice,
				const D3D12_HEAP_PROPERTIES *pHeapProperties,
				D3D12_HEAP_FLAGS heapFlags,
				const D3D12_RESOURCE_DESC *pResourceDesc,
				D3D12_RESOURCE_STATES initialResourceState);

			CBuffer(CDevice::sPtr spDevice,
				UINT64 nSizeInBytes,
				D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);
		public:

			typedef std::shared_ptr<CBuffer> sPtr;
			typedef std::unique_ptr<CBuffer> uPtr;

			static uPtr Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state);

			static uPtr Create(CDevice::sPtr spDevice,
				const D3D12_HEAP_PROPERTIES *pHeapProperties,
				D3D12_HEAP_FLAGS heapFlags,
				const D3D12_RESOURCE_DESC *pResourceDesc,
				D3D12_RESOURCE_STATES initialResourceState);
			
			static uPtr Create(CDevice::sPtr spDevice,
				UINT64 nSizeInBytes,
				D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			static uPtr CreateAsUpload(
				CDevice::sPtr spDevice,
				UINT64 nSizeInBytes,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			static uPtr CreateAsReadback(
				CDevice::sPtr spDevice,
				UINT64 nSizeInBytes,
				D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE,
				D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			// pQueue and afterState only applies to resource in default heap.
			void UploadSync(
				UINT64 offsetInBytes, UINT64 sizeInBytes,
				const void* pSrcData,
				D3D12_RESOURCE_STATES afterStateForResourceInDefaultHeap = D3D12_RESOURCE_STATE_GENERIC_READ);

			// pQueue and afterState only applies to resource in default heap.
			void ReadbackSync(
				UINT64 offsetInBytes, UINT64 sizeInBytes,
				std::vector<uint8_t> &dstData,
				D3D12_RESOURCE_STATES afterStateForResourceInDefaultHeap = D3D12_RESOURCE_STATE_COPY_SOURCE);
		};
		
	}
}

#include "inl\igD3D12Buffer.inl"

