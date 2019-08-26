#include "igD3D12CoreUploadMemoryAllocator.h"

using namespace ig::D3D12::Core;

CUploadMemoryAllocator::MemoryBlock CUploadMemoryAllocator::Alloc(size_t size, const void *pInitialData, size_t alignment)
{
	const size_t alignMask = alignment - 1;
	IG_D3D_ASSERT((alignMask & alignment) == 0);

	std::lock_guard<std::mutex> LockGuard(m_AllocMutex);
	CUploadMemoryAllocator::MemoryBlock ret;

	// Handle alignment
	size_t allocSize = (size + alignMask) & ~alignMask;
	m_nCurOffset = (m_nCurOffset + alignMask) & ~alignMask;

	if (allocSize > m_nPageSize)
	{
		// TODO: handle large page
		IG_D3D_ASSERT(0);
	}

	if (m_nCurPageId < 0 || m_nCurOffset + allocSize > m_nPageSize)
	{
		if (m_AvailablePageIds.size() > 0)
		{
			m_nCurPageId = m_AvailablePageIds.back();
			m_nCurOffset = 0;
			m_AvailablePageIds.pop_back();
		}
		else
		{
			std::lock_guard<std::mutex> LockGuard(m_BlockMutex);
			auto it = m_RetiredBlocks.begin();
			while (it != m_RetiredBlocks.end())
			{
				if (it->pFence->GetCompletedValue() >= it->completionFenceVal)
				{
					InterlockedDecrement(&m_Pages[it->nPageId].nRefCount);
					it = m_RetiredBlocks.erase(it);
				}
				else
				{
					++it;
				}
			}
			auto pageIt = m_Pages.begin();
			int32_t id = 0;
			while (pageIt != m_Pages.end())
			{
				if (pageIt->nRefCount == 0)
				{
					m_AvailablePageIds.push_back(id);
				}
				++pageIt;
				++id;
			}
		}

		if (m_AvailablePageIds.size() > 0)
		{
			m_nCurPageId = m_AvailablePageIds.back();
			m_nCurOffset = 0;
			m_AvailablePageIds.pop_back();
		}
		else
		{
			if (FAILED(CreateNewPage()))
				return ret;
		}

	}

	ret.m_nPageId = m_nCurPageId;
	ret.nActualSize = allocSize;
	ret.nOffsetInResource = m_nCurOffset;
	ret.pResource = m_Pages[m_nCurPageId].pResource.Get();
	ret.GPUAddress = m_Pages[m_nCurPageId].pResource->GetGPUVirtualAddress() + m_nCurOffset;
	ret.CPUAddress = (uint8_t*)m_Pages[m_nCurPageId].pMappedAddress + m_nCurOffset;
	if (pInitialData)
	{
		memcpy_s(ret.CPUAddress, size, pInitialData, size);
	}

	InterlockedIncrement(&m_Pages[m_nCurPageId].nRefCount);
	m_nCurOffset += allocSize;
	return ret;
}

void CUploadMemoryAllocator::Free(MemoryBlock &m, ID3D12Fence *pFence, uint64_t completionFenceVal)
{
	if (pFence->GetCompletedValue() >= completionFenceVal)
	{
		InterlockedDecrement(&m_Pages[m.m_nPageId].nRefCount);
	}
	else
	{
		std::lock_guard<std::mutex> LockGuard(m_BlockMutex);
		RetiredBlock rb;
		rb.nPageId = m.m_nPageId;
		rb.pFence = pFence;
		rb.completionFenceVal = completionFenceVal;
		m_RetiredBlocks.push_back(rb);
	}
}

HRESULT CUploadMemoryAllocator::CreateNewPage()
{
	Page page;
	IG_D3D_ASSERT_SUCCEEDED(m_pDevice->CreateCommittedResource(HeapProperties().AsUpload(), D3D12_HEAP_FLAG_NONE,
		ResourceDesc().AsBuffer(m_nPageSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&page.pResource)));
	
	IG_D3D_FAILED_RETURN(page.pResource->Map(0, nullptr, &page.pMappedAddress));
	page.nRefCount = 0;

	m_Pages.push_back(std::move(page));
	m_nCurOffset = 0;
	m_nCurPageId = (int32_t)m_Pages.size() - 1;
	return S_OK;
}
