#pragma once

namespace ig
{
	namespace D3D12
	{
		inline CSampler::uPtr CSampler::Create(CDevice::sPtr spDevice, const D3D12_SAMPLER_DESC *pDesc)
		{
			return std::make_unique<CSampler>(spDevice, pDesc);
		}

		// Getters
		inline D3D12_CPU_DESCRIPTOR_HANDLE CSampler::CPUHandle() const
		{
			return m_hCPU;
		}
		inline D3D12_SAMPLER_DESC CSampler::Desc() const
		{
			return m_Desc;
		}
	}
}




