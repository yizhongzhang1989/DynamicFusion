#pragma once

// Usage: D3D12 Pipeline State
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// 2019.5.29: Added SetName method.
// Version: 0.1.190529.1600

#include "igD3D12Defs.h"
#include "igD3D12Device.h"

namespace ig
{
	namespace D3D12
	{		
		class CPipelineState
		{
		private:
			CDevice::sPtr m_spDevice;
			PipelineStateComPtr m_pPipelineState;
			void DoInitialization(CDevice::sPtr spDevice, ID3D12PipelineState *pPipelineState);

		public:
			CPipelineState(CDevice::sPtr spDevice, ID3D12PipelineState *pPipelineState);
			CPipelineState(CDevice::sPtr spDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc);
			CPipelineState(CDevice::sPtr spDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc);

			typedef std::shared_ptr<CPipelineState> sPtr;
			typedef std::unique_ptr<CPipelineState> uPtr;
			static uPtr Create(CDevice::sPtr spDevice, ID3D12PipelineState *pPipelineState);		
			static uPtr Create(CDevice::sPtr spDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc);			
			static uPtr Create(CDevice::sPtr spDevice, const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc);
			
			// Getters
			ID3D12PipelineState* RawPtr() const;			
			CDevice* DevicePtr() const;

			HRESULT SetName(LPCWSTR strName);
		};
		
	}
}
#include "inl\igD3D12PipelineState.inl"



