#include "igD3D12PipelineState.h"

using namespace ig::D3D12;

void CPipelineState::DoInitialization(CDevice::sPtr spDevice, ID3D12PipelineState *pPipelineState)
{
	m_spDevice = spDevice;
	m_pPipelineState = pPipelineState;
}

// Create from existing ptr.
CPipelineState::CPipelineState(CDevice::sPtr spDevice, ID3D12PipelineState *pPipelineState)
{
	DoInitialization(spDevice, pPipelineState);
}

// Graphics
CPipelineState::CPipelineState(CDevice::sPtr spDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc)
{
	PipelineStateComPtr pPSO;
	IG_D3D_ASSERT_SUCCEEDED(spDevice->RawPtr()->CreateGraphicsPipelineState(pDesc, IID_PPV_ARGS(&pPSO)));
	DoInitialization(spDevice, pPSO.Get());
}

// Compute
CPipelineState::CPipelineState(CDevice::sPtr spDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc)
{
	PipelineStateComPtr pPSO;
	IG_D3D_ASSERT_SUCCEEDED(spDevice->RawPtr()->CreateComputePipelineState(pDesc, IID_PPV_ARGS(&pPSO)));
	DoInitialization(spDevice, pPSO.Get());
}


