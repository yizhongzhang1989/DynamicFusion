#pragma once
#include "igD3D12TextureCube.h"

namespace ig
{
	namespace D3D12
	{
		inline CTextureCube::CTextureCube(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state) :
			CResource(spDevice, pResource, state)
		{
			if (m_hSRV.ptr != 0) // Has default SRV, then reset to Cube SRV.
			{
				m_hSRV = RetriveSRV(SRVDesc().AsTexCube(Desc().Format, 0, Desc().MipLevels));
			}
		}

		inline CTextureCube::CTextureCube(
			CDevice::sPtr spDevice,
			UINT size,
			UINT mipLevels,
			DXGI_FORMAT format,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_RESOURCE_STATES initialResourceState,
			const D3D12_CLEAR_VALUE *pOptimizedClearValue,
			D3D12_HEAP_FLAGS heapFlags) :
			CResource(spDevice, HeapProperties().AsDefault(), heapFlags,
				ResourceDesc().AsTex2D(format, size, size, mipLevels, 6, 1, 0, resourceFlags),
				initialResourceState, pOptimizedClearValue)
		{
			if (m_hSRV.ptr != 0) // Has default SRV, then reset to Cube SRV.
			{
				m_hSRV = RetriveSRV(SRVDesc().AsTexCube(Desc().Format, 0, Desc().MipLevels));
			}
		}

		inline CTextureCube::uPtr CTextureCube::Create(CDevice::sPtr spDevice, ID3D12Resource *pResource, D3D12_RESOURCE_STATES state)
		{
			return std::make_unique<CTextureCube>(spDevice, pResource, state);
		}

		// Commonly-used non-msaa creation method
		inline CTextureCube::uPtr CTextureCube::Create(
			CDevice::sPtr spDevice,
			UINT size,
			UINT mipLevels,
			DXGI_FORMAT format,
			D3D12_RESOURCE_FLAGS resourceFlags,
			D3D12_RESOURCE_STATES initialResourceState,
			const D3D12_CLEAR_VALUE *pOptimizedClearValue,
			D3D12_HEAP_FLAGS heapFlags)
		{
			return std::make_unique<CTextureCube>(spDevice, size, mipLevels, format, resourceFlags, initialResourceState, pOptimizedClearValue, heapFlags);
		}

	}
}




