#pragma once

namespace ig
{
	namespace D3D12
	{
		inline CPipelineState::uPtr CPipelineState::Create(CDevice::sPtr spDevice, ID3D12PipelineState *pPipelineState)
		{
			return std::make_unique<CPipelineState>(spDevice, pPipelineState);
		}
		inline CPipelineState::uPtr CPipelineState::Create(CDevice::sPtr spDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc)
		{
			return std::make_unique<CPipelineState>(spDevice, pDesc);
		}
		inline CPipelineState::uPtr CPipelineState::Create(CDevice::sPtr spDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc)
		{
			return std::make_unique<CPipelineState>(spDevice, pDesc);
		}

		// Getters
		inline ID3D12PipelineState* CPipelineState::RawPtr() const
		{
			return m_pPipelineState.Get();
		}
		inline CDevice* CPipelineState::DevicePtr() const
		{
			return m_spDevice.get();
		}

		inline HRESULT CPipelineState::SetName(LPCWSTR strName)
		{
			return m_pPipelineState->SetName(strName);
		}
	}
}





