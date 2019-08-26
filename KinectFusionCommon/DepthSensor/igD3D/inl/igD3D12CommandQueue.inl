#pragma once

// Usage: D3D12 CommandQueue
// Created: 2018.9.27
// Last Modified: 2018.9.27
// 2018.9.27: Initial Version.
// Version: 0.1.180927.2230

#include <mutex>
#include <vector>
#include <queue>
#include <stdint.h>

#include "igD3D12Defs.h"
#include "igD3D12CoreCommandAllocatorPool.h"
#include "igD3D12CoreFence.h"

#include "igD3D12CommandContext.inl"

namespace ig
{
	namespace D3D12
	{	
		class CCommandQueue
		{
			friend class CDevice;

		public:
			uint64_t Execute(CCommandContext *pContext);

			uint64_t IncrementFence();

			bool IsFenceComplete(uint64_t nFenceVal);

			void WaitForFence(uint64_t nFenceVal);

			void WaitForIdle();

			// Getters
			ID3D12CommandQueue* RawPtr() { return m_pCmdQueue.Get(); }

			CGraphicsCommandContext* RequestGraphicsContext(ID3D12PipelineState* pInitialState = nullptr);
			CComputeCommandContext* RequestComputeContext(ID3D12PipelineState* pInitialState = nullptr);

		private:
			CCommandQueue(ID3D12Device *pDevice, const D3D12_COMMAND_QUEUE_DESC *pDesc);
			void DiscardContext(CCommandContext* pContext);

		private:
			DeviceComPtr m_pDevice;
			CommandQueueComPtr m_pCmdQueue;
			std::unique_ptr<Core::CFence> m_upFence;
			uint64_t m_nNextFenceVal;
			std::mutex m_FenceMutex;

			std::unique_ptr<Core::CCommandAllocatorPool> m_upAllocatorPool;
			std::mutex m_CommandContextMutex;
			std::queue<CGraphicsCommandContext*> m_ReadyGraphicsContexts;
			std::queue<CComputeCommandContext*> m_ReadyComputeContexts;
			std::vector<std::shared_ptr<CCommandContext>> m_Contexts;

		};
		
	}
}



