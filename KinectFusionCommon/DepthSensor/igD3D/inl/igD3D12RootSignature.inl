#pragma once
#include "igD3D12RootSignature.h"

namespace ig
{
	namespace D3D12
	{
		inline CRootSignature::uPtr CRootSignature::Create(CDevice::sPtr spDevice, ID3D12RootSignature *pSignature)
		{
			return std::make_unique<CRootSignature>(spDevice, pSignature);
		}

		inline CRootSignature::uPtr CRootSignature::Create(CDevice::sPtr spDevice, const D3D12_ROOT_SIGNATURE_DESC* pDesc, UINT nodeMask)
		{
			return std::make_unique<CRootSignature>(spDevice, pDesc, nodeMask);
		}

		// Getters
		inline ID3D12RootSignature* CRootSignature::RawPtr() const
		{
			return m_pRootSignature.Get();
		}

		inline CDevice* CRootSignature::DevicePtr() const
		{
			return m_spDevice.get();
		}

		inline HRESULT CRootSignature::SetName(LPCWSTR strName)
		{
			return m_pRootSignature->SetName(strName);
		}
	}
}





