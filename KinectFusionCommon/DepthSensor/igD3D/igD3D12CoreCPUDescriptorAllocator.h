#pragma once

// Usage: CPU descriptor allocator
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// Version: 0.1.180929.2215

#include "igD3D12Defs.h"
#include <mutex>

namespace ig
{
	namespace D3D12
	{
		namespace Core
		{
			class CCPUDescriptorAllocator
			{
				static const uint32_t sm_nDescriptorsPerHeap = 256;

			public:
				CCPUDescriptorAllocator(ID3D12Device *pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type);

			public:
				D3D12_CPU_DESCRIPTOR_HANDLE Alloc(uint32_t nDescriptors = 1);
				void DestroyAllDescriptors();

			private:
				DeviceComPtr m_pDevice;
				D3D12_DESCRIPTOR_HEAP_TYPE m_Type;
				std::vector<DescriptorHeapComPtr> m_Heaps;
				ID3D12DescriptorHeap *m_pCurHeap ;
				D3D12_CPU_DESCRIPTOR_HANDLE m_CurHandle;
				uint32_t m_nFreeHandles;
				uint32_t m_nStep;
				std::mutex m_AllocMutex;
			};
		}

	}
}

