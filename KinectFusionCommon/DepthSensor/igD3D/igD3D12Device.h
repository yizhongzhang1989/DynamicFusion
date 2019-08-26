#pragma once

// Usage: D3D12 Device
// Can only be created by CDevice::Create method.
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// 2019.5.29: Added SetName method.
// Version: 0.1.190529.1600

#include "igD3D12Defs.h"
#include "igD3D12CoreCPUDescriptorAllocator.h"

namespace ig
{
	namespace D3D12
	{
		class CCommandQueue;

		class CDevice
		{
		private:
			void DoInitialization(ID3D12Device *pDevice);
			explicit CDevice(ID3D12Device *pDevice);
			CDevice(IDXGIAdapter *pAdapter = nullptr, D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0);
		
		public:
			typedef std::shared_ptr<CDevice> sPtr;			
			static sPtr Create(ID3D12Device *pDevice);
			static sPtr Create(IDXGIAdapter *pAdapter = nullptr, D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0);

		public:
			// Getters
			ID3D12Device * RawPtr() const;
			CCommandQueue* CommandQueue(D3D12_COMMAND_LIST_TYPE type);
			
			CCommandQueue* GraphicsQueue();
			
			CCommandQueue* ComputeQueue();
			
			CCommandQueue* CopyQueue();

			D3D12_CPU_DESCRIPTOR_HANDLE AllocCPUDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t nDescriptors = 1);

			HRESULT SetName(LPCWSTR strName);

		private:
			DeviceComPtr m_pDevice;
			std::unique_ptr<CCommandQueue> m_upGraphicsQueue;
			std::unique_ptr<CCommandQueue> m_upComputeQueue;
			std::unique_ptr<CCommandQueue> m_upCopyQueue;
			std::unique_ptr<Core::CCPUDescriptorAllocator> m_upCPUDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
		};		
	}
}

#include "inl\igD3D12Device.inl"

