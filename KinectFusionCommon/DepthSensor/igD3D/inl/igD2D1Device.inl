#pragma once
//#include "igD2D1Device.h"

namespace ig
{
	namespace D2D1
	{
		// Getters
		inline ID2D1Factory1* CDevice::FactoryPtr()	const { return m_pFactory.Get(); }
		inline ID2D1Device* CDevice::Ptr()	const { return m_pDevice.Get(); }
		//inline CDeviceContext& CDevice::ImmediateContext() { return m_ImmediateContext; }
		inline bool CDevice::IsValid() const { return m_pDevice != nullptr; }

		// Convert to ComPtr
		inline CDevice::operator ID2D1Device*() const { return m_pDevice.Get(); }

	}

}
