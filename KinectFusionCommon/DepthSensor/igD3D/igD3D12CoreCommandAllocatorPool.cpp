#include "igD3D12CoreCommandAllocatorPool.h"

using namespace ig::D3D12::Core;

ID3D12CommandAllocator* CCommandAllocatorPool::RequestAllocator(uint64_t CompletedFenceValue)
{
	std::lock_guard<std::mutex> LockGuard(m_AllocatorMutex);

	ID3D12CommandAllocator* pAllocator = nullptr;

	if (!m_ReadyAllocators.empty())
	{
		std::pair<uint64_t, ID3D12CommandAllocator*>& AllocatorPair = m_ReadyAllocators.front();
		if (AllocatorPair.first <= CompletedFenceValue)
		{
			pAllocator = AllocatorPair.second;
			IG_D3D_ASSERT_SUCCEEDED(pAllocator->Reset());
			m_ReadyAllocators.pop();
		}
	}

	if (pAllocator == nullptr)
	{
		CommandAllocatorComPtr pNewAllocator;
		if (FAILED(m_pDevice->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&pNewAllocator))))
			return nullptr;
		m_AllocatorPool.push_back(pNewAllocator);
		pAllocator = pNewAllocator.Get();
	}

	return pAllocator;
}

void CCommandAllocatorPool::DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator * Allocator)
{
	std::lock_guard<std::mutex> LockGuard(m_AllocatorMutex);
	m_ReadyAllocators.push(std::make_pair(FenceValue, Allocator));
}
