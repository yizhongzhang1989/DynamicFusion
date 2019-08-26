#pragma once

// Usage: D3D12 Dynamic Sampler
// Created: 2018.10.8
// 2018.10.8: Initial Version.
// Version: 0.1.181008.1435

#include "igD3D12Defs.h"
#include "igD3D12Device.h"

namespace ig
{
	namespace D3D12
	{
		class CSampler
		{
		public:
			CSampler(CDevice::sPtr spDevice, const D3D12_SAMPLER_DESC *pDesc);

			typedef std::shared_ptr<CSampler> sPtr;
			typedef std::shared_ptr<CSampler> uPtr;			
			static uPtr Create(CDevice::sPtr spDevice, const D3D12_SAMPLER_DESC *pDesc);

			// Getters
			D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle() const;		
			D3D12_SAMPLER_DESC Desc() const;		

		private:
			CDevice::sPtr m_spDevice = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE m_hCPU = {0};
			D3D12_SAMPLER_DESC m_Desc;
		};
	}
}

#include "inl\igD3D12Sampler.inl"


