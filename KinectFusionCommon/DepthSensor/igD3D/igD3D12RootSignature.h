#pragma once

// Usage: D3D12 Root Signature
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
		class CRootSignature
		{
		private:
			CDevice::sPtr m_spDevice;
			RootSignatureComPtr m_pRootSignature;

		private:
			void DoInitialization(CDevice::sPtr spDevice, ID3D12RootSignature *pSignature);

		public:
			CRootSignature(CDevice::sPtr spDevice, ID3D12RootSignature *pSignature);
			CRootSignature(CDevice::sPtr spDevice, const D3D12_ROOT_SIGNATURE_DESC* pDesc, UINT nodeMask = 0);
			
			typedef std::shared_ptr<CRootSignature> sPtr;
			typedef std::unique_ptr<CRootSignature> uPtr;
			static uPtr Create(CDevice::sPtr spDevice, ID3D12RootSignature *pSignature);
			static uPtr Create(CDevice::sPtr spDevice, const D3D12_ROOT_SIGNATURE_DESC* pDesc, UINT nodeMask = 0);	

			// Getters
			ID3D12RootSignature* RawPtr() const;		
			CDevice* DevicePtr() const;

			HRESULT SetName(LPCWSTR strName);

		};		
	}
}

#include "inl\igD3D12RootSignature.inl"



