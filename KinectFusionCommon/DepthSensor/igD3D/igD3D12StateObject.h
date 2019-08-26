#pragma once

// Usage: D3D12 State Object
// Created: 2019.6.3
// 2019.6.3: Initial Version.
// Version: 0.1.190603.1800

#include "igD3D12Defs.h"
#include "igD3D12Device.h"

namespace ig
{
	namespace D3D12
	{
		class CStateObject
		{
		private:
			CDevice::sPtr m_spDevice;
			StateObjectComPtr m_pStateObject;
			StateObjectPropertiesComPtr m_pStateObjectProperties;

			void DoInitialization(CDevice::sPtr spDevice, ID3D12StateObject *pStateObject);

		public:
			CStateObject(CDevice::sPtr spDevice, ID3D12StateObject *pPipelineState);
			CStateObject(CDevice::sPtr spDevice, const D3D12_STATE_OBJECT_DESC *pDesc);

			typedef std::shared_ptr<CStateObject> sPtr;
			typedef std::unique_ptr<CStateObject> uPtr;
			static uPtr Create(CDevice::sPtr spDevice, ID3D12StateObject *pStateObject);
			static uPtr Create(CDevice::sPtr spDevice, const D3D12_STATE_OBJECT_DESC *pDesc);

			// Getters
			ID3D12StateObject* RawPtr() const;
			ID3D12StateObjectProperties* PropertiesRawPtr() const;
			CDevice* DevicePtr() const;
			void* ShaderIdentifier(LPCWSTR pExportName);

			HRESULT SetName(LPCWSTR strName);
		};

	}
}
#include "inl\igD3D12StateObject.inl"
