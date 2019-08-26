#pragma once

#pragma once

// Usage: simple wrapper for Device
// Created: 2018.4.19
// Last Modified: 2018.4.19
// 2018.4.19: Initial Version.
// Version: 0.1.180419.1445

#include "igD2D1Defs.h"

namespace ig
{
	namespace D2D1
	{
		class CDevice
		{
		public:
			// Default construct/copy/assign behaviors.
			CDevice() = default;
			~CDevice() = default;

			CDevice(CDevice&&) = default;
			CDevice& operator=(CDevice&&) = default;

			CDevice(const CDevice&) = delete;
			CDevice& operator=(const CDevice&) = delete;

			// Create from existing pDevice ptr.
			HRESULT Create(ID2D1Device *pDevice);

			// Create from D3D device.
			HRESULT Create(ID3D11Device *pD3D11Device, D2D1_FACTORY_TYPE factoryType = D2D1_FACTORY_TYPE_MULTI_THREADED);

			// Clear.
			void Destroy();

			// Getters
			ID2D1Factory1* FactoryPtr()	const;
			ID2D1Device* Ptr()	const;
			
			bool IsValid() const;

			// Convert to ComPtr
			operator ID2D1Device*() const;

		private:
			Factory1ComPtr m_pFactory;
			DeviceComPtr	m_pDevice;
			//CDeviceContext	m_ImmediateContext;
		};

	}

}

#include "inl\igD2D1Device.inl"
