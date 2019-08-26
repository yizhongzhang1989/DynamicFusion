#include "igD3D12CoreGPUDescriptorAllocator.h"

using namespace ig::D3D12::Core;

CGPUDescriptorAllocator::HeapBlock CGPUDescriptorAllocator::Alloc(UINT nHandles)
{
	std::lock_guard<std::mutex> LockGuard(m_AllocMutex);
	HeapBlock ret;
	if (nHandles > m_nHeapSize)
	{
		IG_D3D_ASSERT(0, L"DescriptorHeapPool::Request: Requested size > supported max size.");
	}
	// Request new heap
	if (m_nCurHeapId < 0 || m_nCurOffset + nHandles > m_nHeapSize)
	{
		if (m_AvailableHeapIds.size() > 0)
		{
			m_nCurHeapId = m_AvailableHeapIds.back();
			m_nCurOffset = 0;
			m_AvailableHeapIds.pop_back();

		}
		else
		{
			std::lock_guard<std::mutex> LockGuard(m_BlockMutex);
			auto it = m_RetiredBlocks.begin();
			while (it != m_RetiredBlocks.end())
			{
				if (it->pFence->GetCompletedValue() >= it->completionFenceVal)
				{
					InterlockedDecrement(&m_Heaps[it->nHeapId].nRefCount);
					it = m_RetiredBlocks.erase(it);
				}
				else
				{
					++it;
				}
			}
			auto heapIt = m_Heaps.begin();
			int32_t id = 0;
			while (heapIt != m_Heaps.end())
			{
				if (heapIt->nRefCount == 0)
				{
					m_AvailableHeapIds.push_back(id);
				}
				++heapIt;
				++id;
			}
		}

		if (m_AvailableHeapIds.size() > 0)
		{
			m_nCurHeapId = m_AvailableHeapIds.back();
			m_nCurOffset = 0;
			m_AvailableHeapIds.pop_back();
		}
		else
		{
			if (FAILED(CreateNewDescriptorHeap()))
				return ret;
		}

	}

	//ret.m_pDevice = m_pDevice;
	//ret.m_Type = m_Type;
	ret.m_nStep = m_nIncrementStep;
	ret.m_nHeapId = m_nCurHeapId;
	ret.m_pHeap = m_Heaps[m_nCurHeapId].pDescriptorHeap.Get();
	ret.m_hCPU = m_Heaps[m_nCurHeapId].pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	ret.m_hCPU.ptr += m_nIncrementStep * m_nCurOffset;
	ret.m_hGPU = m_Heaps[m_nCurHeapId].pDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	ret.m_hGPU.ptr += m_nIncrementStep * m_nCurOffset;
	InterlockedIncrement(&m_Heaps[m_nCurHeapId].nRefCount);
	m_nCurOffset += nHandles;
	return ret;
}

void CGPUDescriptorAllocator::Free(HeapBlock &m, ID3D12Fence *pFence, uint64_t completionFenceVal)
{
	if (m.m_nHeapId < 0) return;

	if (pFence->GetCompletedValue() >= completionFenceVal)
	{
		InterlockedDecrement(&m_Heaps[m.m_nHeapId].nRefCount);
	}
	else
	{
		std::lock_guard<std::mutex> LockGuard(m_BlockMutex);
		RetiredBlock rb;
		rb.nHeapId = m.m_nHeapId;
		rb.pFence = pFence;
		rb.completionFenceVal = completionFenceVal;
		m_RetiredBlocks.push_back(rb);
	}
}

HRESULT CGPUDescriptorAllocator::CreateNewDescriptorHeap()
{
	DescHeap heap;
	DescriptorHeapDesc desc;
	desc.Type = m_Type;
	desc.NodeMask = 0;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NumDescriptors = m_nHeapSize;
	IG_D3D_FAILED_RETURN(m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap.pDescriptorHeap)));

	heap.nRefCount = 0;
	m_Heaps.push_back(std::move(heap));
	m_nCurOffset = 0;
	m_nCurHeapId = (int32_t)m_Heaps.size() - 1;
	return S_OK;
}



