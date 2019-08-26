#include "igD3D12Device.h"
#include "igD3D12CoreCPUDescriptorAllocator.h"

using namespace ig::D3D12;

void CDevice::DoInitialization(ID3D12Device *pDevice)
{
	IG_D3D_ASSERT(pDevice != nullptr);

	m_pDevice = pDevice;
	
	// Create queues
	m_upGraphicsQueue = std::unique_ptr<CCommandQueue>(new CCommandQueue(pDevice, CommandQueueDesc().AsDirect()));
	m_upComputeQueue = std::unique_ptr<CCommandQueue>(new CCommandQueue(pDevice, CommandQueueDesc().AsCompute()));
	m_upCopyQueue = std::unique_ptr<CCommandQueue>(new CCommandQueue(pDevice, CommandQueueDesc().AsCopy()));
	m_pDevice = pDevice;

	// Create CPU Descriptor Allocators.
	for (UINT i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
	{
		m_upCPUDescriptorAllocator[i] = std::make_unique<Core::CCPUDescriptorAllocator>(pDevice, (D3D12_DESCRIPTOR_HEAP_TYPE)i);
	}
}

CDevice::CDevice(IDXGIAdapter *pAdapter, D3D_FEATURE_LEVEL minFeatureLevel)
{
#ifdef _DEBUG // enable debug layer
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}
#endif
	DeviceComPtr pDevice;
	// Create device
	IG_D3D_ASSERT_SUCCEEDED(D3D12CreateDevice(pAdapter, minFeatureLevel, IID_PPV_ARGS(&pDevice)));
	DoInitialization(pDevice.Get());
}

D3D12_CPU_DESCRIPTOR_HANDLE CDevice::AllocCPUDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t nDescriptors )
{
	return m_upCPUDescriptorAllocator[type]->Alloc(nDescriptors);
}



