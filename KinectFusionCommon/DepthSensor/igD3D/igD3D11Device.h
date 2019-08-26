#pragma once

// Usage: simple wrapper for Device
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"
#include "igD3D11DeviceContext.h"
namespace ig
{
	namespace D3D11
	{
		class CDevice
		{
		public:
			CDevice() = default;
			~CDevice() = default;
			CDevice(CDevice&&) = default;
			CDevice& operator=(CDevice&&) = default;
			CDevice(const CDevice&) = delete;
			CDevice& operator=(const CDevice&) = delete;
			
			// Create from existing pDevice ptr.
			HRESULT Create(ID3D11Device *pDevice);

			// General creation method.
			HRESULT Create(IDXGIAdapter* pAdapter = nullptr,
				D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE,
				D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0,
				UINT deviceFlags = 0);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device* Ptr()	const;
			CDeviceContext& ImmediateContext();
			bool IsValid() const;

			// Convert to ComPtr
			operator ID3D11Device*() const;

		private:
			DeviceComPtr	m_pDevice;
			CDeviceContext	m_ImmediateContext;
		};

	}

}

#include "inl\igD3D11Device.inl"