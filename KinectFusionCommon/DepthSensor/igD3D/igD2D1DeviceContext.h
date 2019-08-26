#pragma once

#pragma once

// Usage: simple wrapper for DeviceContext
// Created: 2018.4.19
// Last Modified: 2018.4.19
// 2018.4.19: Initial Version.
// Version: 0.1.180419.1445

#include "igD2D1Defs.h"

namespace ig
{
	namespace D2D1
	{
		class CDeviceContext
		{
		public:
			// Default construct/copy/assign behaviors.
			CDeviceContext() = default;
			~CDeviceContext() = default;

			CDeviceContext(CDeviceContext&&) = default;
			CDeviceContext& operator=(CDeviceContext&&) = default;

			CDeviceContext(const CDeviceContext&) = delete;
			CDeviceContext& operator=(const CDeviceContext&) = delete;

			// Create from existing pDC ptr.
			HRESULT Create(ID2D1DeviceContext *pDC);

			// Create from D3D device.
			HRESULT Create(
				ID2D1Device *pDevice, 
				D2D1_DEVICE_CONTEXT_OPTIONS options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE);

			// Clear.
			void Destroy();

			// Getters
			ID2D1Device* DevicePtr()	const;
			ID2D1DeviceContext* Ptr()	const;
			//CDeviceContext& ImmediateContext();
			bool IsValid() const;

			// Convert to ComPtr
			operator ID2D1DeviceContext*() const;


		//public: // Method wrappers
		//	void BeginDraw() { m_pDC->BeginDraw(); }
		//	HRESULT EndDraw() { return m_pDC->EndDraw(); }

		private:
			DeviceComPtr	m_pDevice;
			DeviceContextComPtr m_pDC;
			//CDeviceContext	m_ImmediateContext;
		};

	}

}

#include "inl\igD2D1DeviceContext.inl"

