#include "igD3D12CoreFence.h"

using namespace ig::D3D12::Core;

void CFence::DoInitialization(ID3D12Fence *pFence)
{
	m_pFence = pFence;
	// Create event
	HANDLE hEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	IG_D3D_ASSERT(hEvent != 0);
	m_Event = hEvent;
}

CFence::CFence(ID3D12Fence *pFence)
{
	DoInitialization(pFence);
}

CFence::CFence(ID3D12Device *pDevice, UINT64 nInitialVal, D3D12_FENCE_FLAGS flags)
{
	FenceComPtr pFence;
	IG_D3D_ASSERT_SUCCEEDED(pDevice->CreateFence(nInitialVal, flags, IID_PPV_ARGS(&pFence)));
	DoInitialization(pFence.Get());
}

HRESULT CFence::Wait(UINT64 nFenceVal, DWORD nMilliseconds)
{
	if (m_pFence->GetCompletedValue() < nFenceVal)
	{
		std::lock_guard<std::mutex> lock(m_EventMutex);

		IG_D3D_FAILED_RETURN(m_pFence->SetEventOnCompletion(nFenceVal, m_Event));
		WaitForSingleObject(m_Event, nMilliseconds);
	}
	return S_OK;
}

