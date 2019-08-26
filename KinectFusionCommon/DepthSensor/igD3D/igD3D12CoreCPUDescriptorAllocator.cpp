#include "igD3D12CoreCPUDescriptorAllocator.h"
using namespace ig::D3D12::Core;

CCPUDescriptorAllocator::CCPUDescriptorAllocator(ID3D12Device *pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type) :
	m_pDevice(pDevice), m_Type(type), m_pCurHeap(nullptr), m_CurHandle({0}), m_nFreeHandles(0)
{
	IG_D3D_ASSERT(pDevice != nullptr);
	m_nStep = m_pDevice->GetDescriptorHandleIncrementSize(type);
}

void CCPUDescriptorAllocator::DestroyAllDescriptors()
{
	m_Heaps.clear();
	m_pCurHeap = nullptr;
	m_CurHandle = { 0 };
	m_nFreeHandles = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE CCPUDescriptorAllocator::Alloc(uint32_t nDescriptors)
{
	std::lock_guard<std::mutex> LockGuard(m_AllocMutex);

	if (m_pCurHeap == nullptr || m_nFreeHandles < nDescriptors)
	{
		D3D12_DESCRIPTOR_HEAP_DESC Desc;
		Desc.Type = m_Type;
		Desc.NumDescriptors = sm_nDescriptorsPerHeap;
		Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		Desc.NodeMask = 0; // Currently assume single adapter.

		DescriptorHeapComPtr pHeap;
		IG_D3D_ASSERT_SUCCEEDED(m_pDevice->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&pHeap)));
		m_Heaps.push_back(pHeap);
		m_pCurHeap = pHeap.Get();
		m_CurHandle = m_pCurHeap->GetCPUDescriptorHandleForHeapStart();
		m_nFreeHandles = sm_nDescriptorsPerHeap;
	}
	D3D12_CPU_DESCRIPTOR_HANDLE ret = m_CurHandle;
	m_CurHandle.ptr += nDescriptors * m_nStep;
	m_nFreeHandles -= nDescriptors;
	return ret;
}
