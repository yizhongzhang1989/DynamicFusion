#include "inl\igD3D12CommandQueue.inl"

using namespace ig::D3D12;

CCommandQueue::CCommandQueue(ID3D12Device *pDevice, const D3D12_COMMAND_QUEUE_DESC *pDesc) : 
	m_pDevice(pDevice), m_nNextFenceVal(1)
{
	IG_D3D_ASSERT_SUCCEEDED(pDevice->CreateCommandQueue(pDesc, IID_PPV_ARGS(&m_pCmdQueue)));
	m_upAllocatorPool = std::make_unique<Core::CCommandAllocatorPool>(pDevice, pDesc->Type);
	m_upFence = std::make_unique<Core::CFence>(pDevice, 0);
}

uint64_t CCommandQueue::Execute(CCommandContext *pContext)
{
	std::lock_guard<std::mutex> lock(m_FenceMutex);

	// Close
	IG_D3D_ASSERT_SUCCEEDED(pContext->Close());
	// Submit
	ID3D12CommandList* pCmdList = pContext->CommandListRawPtr();
	m_pCmdQueue->ExecuteCommandLists(1, &pCmdList);
	// Signal	
	m_pCmdQueue->Signal(m_upFence->RawPtr(), m_nNextFenceVal);
	
	m_upAllocatorPool->DiscardAllocator(m_nNextFenceVal, pContext->AssociatedAllocator());
	pContext->FreeUploadMemoryBlocks(m_upFence->RawPtr(), m_nNextFenceVal);
	pContext->FreeDynamicDescriptorHeapBlocks(m_upFence->RawPtr(), m_nNextFenceVal);

	DiscardContext(pContext);

	return m_nNextFenceVal++;
}

uint64_t CCommandQueue::IncrementFence()
{
	std::lock_guard<std::mutex> lock(m_FenceMutex);
	// Signal
	//m_CmdQueue.Signal(m_Fence, m_nNextFenceVal);
	m_pCmdQueue->Signal(m_upFence->RawPtr(), m_nNextFenceVal);
	return m_nNextFenceVal++;
}

bool CCommandQueue::IsFenceComplete(uint64_t nFenceVal)
{
	return m_upFence->CompletedValue() >= nFenceVal;
}

void CCommandQueue::WaitForFence(uint64_t nFenceVal)
{
	if (IsFenceComplete(nFenceVal))
		return;
	m_upFence->Wait(nFenceVal);
}

void CCommandQueue::WaitForIdle()
{
	WaitForFence(IncrementFence());
}

CGraphicsCommandContext* CCommandQueue::RequestGraphicsContext(ID3D12PipelineState* pInitialState)
{
	std::lock_guard<std::mutex> LockGuard(m_CommandContextMutex);
	auto pAllocator = m_upAllocatorPool->RequestAllocator(m_upFence->CompletedValue());

	CGraphicsCommandContext* pRet = nullptr;
	if (!m_ReadyGraphicsContexts.empty())
	{
		pRet = m_ReadyGraphicsContexts.front();
		IG_D3D_ASSERT_SUCCEEDED(pRet->Reset(pAllocator, pInitialState));
		m_ReadyGraphicsContexts.pop();
	}

	if (pRet == nullptr)
	{
		auto spContext = std::shared_ptr<CGraphicsCommandContext>(new CGraphicsCommandContext(m_pDevice.Get(), m_pCmdQueue->GetDesc().Type, pAllocator, pInitialState));
		m_Contexts.push_back(spContext);
		pRet = spContext.get();
	}

	return pRet;
}

CComputeCommandContext* CCommandQueue::RequestComputeContext(ID3D12PipelineState* pInitialState)
{
	std::lock_guard<std::mutex> LockGuard(m_CommandContextMutex);
	auto pAllocator = m_upAllocatorPool->RequestAllocator(m_upFence->CompletedValue());
	
	CComputeCommandContext* pRet = nullptr;	
	if (!m_ReadyComputeContexts.empty())
	{
		pRet = m_ReadyComputeContexts.front();
		IG_D3D_ASSERT_SUCCEEDED(pRet->Reset(pAllocator, pInitialState));
		m_ReadyComputeContexts.pop();
	}

	if (pRet == nullptr)
	{
		auto spContext = std::shared_ptr<CComputeCommandContext>(new CComputeCommandContext(m_pDevice.Get(), m_pCmdQueue->GetDesc().Type, pAllocator, pInitialState));
		m_Contexts.push_back(spContext);
		pRet = spContext.get();
	}

	return pRet;
}

void CCommandQueue::DiscardContext(CCommandContext* pContext)
{
	std::lock_guard<std::mutex> LockGuard(m_CommandContextMutex);
	if (pContext->m_ContextType == CCommandContext::Compute)
	{
		m_ReadyComputeContexts.push((CComputeCommandContext*)pContext);
	}
	else if (pContext->m_ContextType == CCommandContext::Graphics)
	{
		m_ReadyGraphicsContexts.push((CGraphicsCommandContext*)pContext);
	}
}
