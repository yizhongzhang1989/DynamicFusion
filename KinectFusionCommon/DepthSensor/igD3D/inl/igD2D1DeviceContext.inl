#pragma once
#include "igD2D1DeviceContext.h"

namespace ig
{
	namespace D2D1
	{
		// Getters
		inline ID2D1Device* CDeviceContext::DevicePtr()	const { return m_pDevice.Get(); }
		inline ID2D1DeviceContext* CDeviceContext::Ptr()	const { return m_pDC.Get(); }
		inline bool CDeviceContext::IsValid() const { return m_pDC != nullptr; }

		// Convert to ComPtr
		inline CDeviceContext::operator ID2D1DeviceContext*() const { return m_pDC.Get(); }

	}

}

