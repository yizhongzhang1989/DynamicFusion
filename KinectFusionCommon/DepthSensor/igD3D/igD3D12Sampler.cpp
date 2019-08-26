#include "igD3D12Sampler.h"

using namespace ig::D3D12;


CSampler::CSampler(CDevice::sPtr spDevice, const D3D12_SAMPLER_DESC *pDesc):
	m_spDevice(spDevice), m_Desc(*pDesc)
{
	m_hCPU = spDevice->AllocCPUDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 1);
	spDevice->RawPtr()->CreateSampler(pDesc, m_hCPU);
}


