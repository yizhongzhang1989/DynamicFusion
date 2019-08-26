#pragma once

namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Device* CDevice::Ptr()	const { return m_pDevice.Get(); }
		inline CDeviceContext& CDevice::ImmediateContext() { return m_ImmediateContext; }
		inline bool CDevice::IsValid() const { return m_pDevice != nullptr; }

		// Convert to ComPtr
		inline CDevice::operator ID3D11Device*() const { return m_pDevice.Get(); }

	}

}