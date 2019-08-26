#pragma once

// Usage: Dynamic upload memory allocator
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// Version: 0.1.180929.2215

#include "igD3D12Defs.h"
#include <mutex>
#include <vector>
#include <queue>
#include <list>
#include <stdint.h>

namespace ig
{
	namespace D3D12
	{
		namespace Core
		{
			class CUploadMemoryAllocator
			{
			public:
				class MemoryBlock
				{
					friend class CUploadMemoryAllocator;
				public:
					ID3D12Resource * pResource = nullptr;
					size_t nOffsetInResource = 0;
					size_t nActualSize = 0;
					void *CPUAddress = nullptr;
					D3D12_GPU_VIRTUAL_ADDRESS GPUAddress = 0;

				private:
					int32_t m_nPageId = -1;
				};

			public:
				CUploadMemoryAllocator(ID3D12Device *pDevice, size_t nPageSize = 2 * 1024 * 1024) :
					m_pDevice(pDevice), m_nPageSize(nPageSize) {}

				MemoryBlock Alloc(size_t size, const void* pInitialData = nullptr, size_t alignment = 256);
				void Free(MemoryBlock &m, ID3D12Fence *pFence, uint64_t completionFenceVal);

			private:
				HRESULT CreateNewPage();

				DeviceComPtr m_pDevice = nullptr;
				size_t m_nPageSize = 0;
				std::mutex m_AllocMutex;

				struct Page
				{
					ResourceComPtr pResource;
					void *pMappedAddress;
					uint32_t nRefCount;

				};
				std::vector<Page> m_Pages;
				std::vector<int32_t> m_AvailablePageIds;
				std::mutex m_BlockMutex;

				struct RetiredBlock
				{
					int32_t nPageId;
					FenceComPtr pFence;
					uint64_t completionFenceVal;
				};
				std::list<RetiredBlock> m_RetiredBlocks;
				int32_t m_nCurPageId = -1;
				size_t m_nCurOffset = 0;
			};
		}
	}
}

