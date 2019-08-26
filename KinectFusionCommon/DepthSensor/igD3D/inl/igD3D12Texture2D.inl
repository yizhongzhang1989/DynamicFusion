#pragma once

#include "igD3D12Texture2D.h"

namespace ig
{
	namespace D3D12
	{
		inline CTexture2D::CTexture2D(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state) :
			CResource(spDevice, pResource, state)
		{
		}

		inline CTexture2D::CTexture2D(CDevice::sPtr spDevice,
			const D3D12_HEAP_PROPERTIES *pHeapProperties,
			D3D12_HEAP_FLAGS heapFlags,
			const D3D12_RESOURCE_DESC *pResourceDesc,
			D3D12_RESOURCE_STATES initialResourceState,
			const D3D12_CLEAR_VALUE *pOptimizedClearValue) :
			CResource(spDevice, pHeapProperties, heapFlags, pResourceDesc, initialResourceState, pOptimizedClearValue)
		{
		}

		inline CTexture2D::CTexture2D(
			CDevice::sPtr spDevice,
			UINT w, UINT h,
			UINT mipLevels,
			UINT arraySize,
			DXGI_FORMAT format,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_RESOURCE_STATES initialResourceState,
			const D3D12_CLEAR_VALUE *pOptimizedClearValue,
			D3D12_HEAP_FLAGS heapFlags) :
			CResource(spDevice, HeapProperties().AsDefault(), heapFlags,
				ResourceDesc().AsTex2D(format, w, h, mipLevels, arraySize, 1, 0, resourceFlags),
				initialResourceState, pOptimizedClearValue)
		{
		}

		inline CTexture2D::uPtr CTexture2D::Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state)
		{
			return std::make_unique<CTexture2D>(spDevice, pResource, state);
		}

		inline CTexture2D::uPtr CTexture2D::Create(CDevice::sPtr spDevice,
			const D3D12_HEAP_PROPERTIES *pHeapProperties,
			D3D12_HEAP_FLAGS heapFlags,
			const D3D12_RESOURCE_DESC *pResourceDesc,
			D3D12_RESOURCE_STATES initialResourceState)
		{
			return std::make_unique<CTexture2D>(spDevice, pHeapProperties, heapFlags, pResourceDesc, initialResourceState);
		}

		// Commonly-used non-msaa creation method
		inline CTexture2D::uPtr CTexture2D::Create(CDevice::sPtr spDevice,
			UINT w, UINT h,
			UINT mipLevels,
			UINT arraySize,
			DXGI_FORMAT format,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_RESOURCE_STATES initialResourceState,
			const D3D12_CLEAR_VALUE *pOptimizedClearValue,
			D3D12_HEAP_FLAGS heapFlags)
		{
			return std::make_unique<CTexture2D>(spDevice, w, h, mipLevels, arraySize, format, resourceFlags, initialResourceState, pOptimizedClearValue, heapFlags);
		}

	}
}



