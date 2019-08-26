#pragma once

// Usage: igD3D12 Core Fence class
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
			class CFence
			{
			private:
				//DeviceComPtr m_pDevice;
				FenceComPtr m_pFence;
				HANDLE m_Event;
				std::mutex m_EventMutex;
				
				void DoInitialization(ID3D12Fence *pFence);

			public:
				CFence(ID3D12Fence *pFence);
				CFence(ID3D12Device *pDevice, UINT64 nInitialVal = 0, D3D12_FENCE_FLAGS flags = D3D12_FENCE_FLAG_NONE);
				~CFence() { if (m_Event) CloseHandle(m_Event); }

				HRESULT Wait(UINT64 nFenceVal, DWORD nMilliseconds = INFINITE);

				// Getters
				ID3D12Fence* RawPtr() const { return m_pFence.Get(); }
				HANDLE EventHandle() const { return m_Event; }
				UINT64 CompletedValue() const { return m_pFence->GetCompletedValue(); }

			};
		}
	}

}




