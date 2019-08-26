#pragma once

// Usage: Dynamic GPU descriptor allocator
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
			class CGPUDescriptorAllocator
			{
			public:
				class HeapBlock
				{
					friend class CGPUDescriptorAllocator;

				public:
					D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle(UINT idx = 0)
					{
						D3D12_CPU_DESCRIPTOR_HANDLE ret = m_hCPU;
						ret.ptr += idx * m_nStep;
						return ret;
					}
					D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle(UINT idx = 0)
					{
						D3D12_GPU_DESCRIPTOR_HANDLE ret = m_hGPU;
						ret.ptr += idx * m_nStep;
						return ret;
					}

					ID3D12DescriptorHeap* SrcHeapPtr() const { return m_pHeap.Get(); }

					void Clear()
					{
						m_pHeap = nullptr;
						m_hCPU = { 0 };
						m_hGPU = { 0 };
						m_nHeapId = -1;
						m_nStep = 0;
					}
				private:
					ig::D3D12::DescriptorHeapComPtr m_pHeap = nullptr;
					D3D12_CPU_DESCRIPTOR_HANDLE m_hCPU = { 0 };
					D3D12_GPU_DESCRIPTOR_HANDLE m_hGPU = { 0 };
					int32_t m_nHeapId = -1;
					UINT m_nStep = 0;
				};


			public:
				CGPUDescriptorAllocator(ID3D12Device *pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT nMaxHeapBlockSize = 1024) :
					m_pDevice(pDevice), m_Type(type), m_nHeapSize(nMaxHeapBlockSize)
				{
					m_nIncrementStep = pDevice->GetDescriptorHandleIncrementSize(type);
				}

				HeapBlock Alloc(UINT nHandles);
				void Free(HeapBlock &m, ID3D12Fence *pFence, uint64_t completionFenceVal);

			private:
				HRESULT CreateNewDescriptorHeap();

				DeviceComPtr m_pDevice;
				UINT m_nHeapSize;
				D3D12_DESCRIPTOR_HEAP_TYPE m_Type;
				UINT m_nIncrementStep;
				std::mutex m_AllocMutex;

				struct DescHeap
				{
					DescriptorHeapComPtr pDescriptorHeap;
					uint32_t nRefCount;
				};
				std::vector<DescHeap> m_Heaps;
				std::vector<int32_t> m_AvailableHeapIds;
				std::mutex m_BlockMutex;

				struct RetiredBlock
				{
					int32_t nHeapId;
					FenceComPtr pFence;
					uint64_t completionFenceVal;
				};
				std::list<RetiredBlock> m_RetiredBlocks;
				int32_t m_nCurHeapId = -1;
				UINT m_nCurOffset = 0;
			};
		}
	}
}


