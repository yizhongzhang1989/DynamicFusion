#pragma once

#include "igD3D12Resource.h"

namespace ig
{
	namespace D3D12
	{		
		inline CBuffer::CBuffer(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state) :
				CResource(spDevice, pResource, state) 
		{
		}

		inline CBuffer::CBuffer(CDevice::sPtr spDevice,
				const D3D12_HEAP_PROPERTIES *pHeapProperties,
				D3D12_HEAP_FLAGS heapFlags,
				const D3D12_RESOURCE_DESC *pResourceDesc,
				D3D12_RESOURCE_STATES initialResourceState) :
				CResource(spDevice, pHeapProperties, heapFlags, pResourceDesc, initialResourceState, nullptr) 
		{
		}

		inline CBuffer::CBuffer(
			CDevice::sPtr spDevice,
			UINT64 nSizeInBytes,
			D3D12_HEAP_TYPE heapType,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_RESOURCE_STATES initialResourceState,
			D3D12_HEAP_FLAGS heapFlags) : CBuffer(spDevice,
				HeapProperties().As(heapType, D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
					D3D12_MEMORY_POOL_UNKNOWN),//, heapCreationNodeMask, heapVisibleNodeMask),
				heapFlags, ResourceDesc().AsBuffer(nSizeInBytes, resourceFlags),
				initialResourceState)
		{
		}
		
		inline CBuffer::uPtr CBuffer::Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state)
		{
			return std::make_unique<CBuffer>(spDevice, pResource, state);
		}

		inline CBuffer::uPtr CBuffer::Create(CDevice::sPtr spDevice,
			const D3D12_HEAP_PROPERTIES *pHeapProperties,
			D3D12_HEAP_FLAGS heapFlags,
			const D3D12_RESOURCE_DESC *pResourceDesc,
			D3D12_RESOURCE_STATES initialResourceState)
		{
			return std::make_unique<CBuffer>(spDevice, pHeapProperties, heapFlags, pResourceDesc, initialResourceState);
		}

		inline CBuffer::uPtr CBuffer::Create(CDevice::sPtr spDevice,
			UINT64 nSizeInBytes,
			D3D12_HEAP_TYPE heapType,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_RESOURCE_STATES initialResourceState,
			D3D12_HEAP_FLAGS heapFlags)
		{
			return std::make_unique<CBuffer>(spDevice, nSizeInBytes, heapType, resourceFlags, initialResourceState, heapFlags);
		}

		inline CBuffer::uPtr CBuffer::CreateAsUpload(
			CDevice::sPtr spDevice,
			UINT64 nSizeInBytes,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_HEAP_FLAGS heapFlags)
		{
			return Create(spDevice, nSizeInBytes, D3D12_HEAP_TYPE_UPLOAD, resourceFlags, D3D12_RESOURCE_STATE_GENERIC_READ, heapFlags);
		}

		inline CBuffer::uPtr CBuffer::CreateAsReadback(
			CDevice::sPtr spDevice,
			UINT64 nSizeInBytes,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_HEAP_FLAGS heapFlags)
		{
			return Create(spDevice, nSizeInBytes, D3D12_HEAP_TYPE_READBACK, resourceFlags, D3D12_RESOURCE_STATE_COPY_DEST, heapFlags);
		}
	}
}


