#pragma once
#include "igD3D12Device.h"

namespace ig
{
	namespace D3D12
	{
		inline CDevice::CDevice(ID3D12Device *pDevice)
		{
			DoInitialization(pDevice);
		}
		inline CDevice::sPtr CDevice::Create(ID3D12Device *pDevice)
		{
			return std::shared_ptr<CDevice>(new CDevice(pDevice));
		}

		inline CDevice::sPtr CDevice::Create(IDXGIAdapter *pAdapter, D3D_FEATURE_LEVEL minFeatureLevel)
		{
			return std::shared_ptr<CDevice>(new CDevice(pAdapter, minFeatureLevel));
		}

		inline ID3D12Device *  CDevice::RawPtr() const { return m_pDevice.Get(); }
		inline CCommandQueue*  CDevice::CommandQueue(D3D12_COMMAND_LIST_TYPE type)
		{
			if (type == D3D12_COMMAND_LIST_TYPE_DIRECT)
				return m_upGraphicsQueue.get();
			else if (type == D3D12_COMMAND_LIST_TYPE_COMPUTE)
				return m_upComputeQueue.get();
			else if (type == D3D12_COMMAND_LIST_TYPE_COPY)
				return m_upCopyQueue.get();
			return nullptr;
		}
		inline CCommandQueue*  CDevice::GraphicsQueue()
		{
			return m_upGraphicsQueue.get();
		}
		inline CCommandQueue*  CDevice::ComputeQueue()
		{
			return m_upComputeQueue.get();
		}
		inline CCommandQueue*  CDevice::CopyQueue()
		{
			return m_upCopyQueue.get();
		}

		inline HRESULT CDevice::SetName(LPCWSTR strName)
		{
			return m_pDevice->SetName(strName);
		}
	}
}

#include "igD3D12CommandQueue.inl"


