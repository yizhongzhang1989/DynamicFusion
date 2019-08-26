#pragma once

// Usage: Dynamic command allocator pool
// Designed to work with single increasing fence

// Created: 2018.9.29
// 2018.9.29: Initial Version.
// Version: 0.1.180929.2215

#include "igD3D12Defs.h"
#include <queue>
#include <mutex>

namespace ig
{
	namespace D3D12
	{
		namespace Core
		{
			class CCommandAllocatorPool
			{
			public:
				CCommandAllocatorPool(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE type) :
					m_pDevice(pDevice), m_CommandListType(type)
				{
				}

				ID3D12CommandAllocator* RequestAllocator(uint64_t CompletedFenceValue);
				void DiscardAllocator(uint64_t FenceValue, ID3D12CommandAllocator* Allocator);

			private:
				DeviceComPtr m_pDevice;
				D3D12_COMMAND_LIST_TYPE m_CommandListType;
				std::vector<CommandAllocatorComPtr> m_AllocatorPool;
				std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> m_ReadyAllocators;
				std::mutex m_AllocatorMutex;
			};

		}
	}
}
