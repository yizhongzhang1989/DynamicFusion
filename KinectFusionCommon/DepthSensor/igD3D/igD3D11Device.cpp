#include "igD3D11Device.h"
#include <vector>

using namespace ig::D3D11;

HRESULT CDevice::Create(ID3D11Device *pDevice)
{
	// Clear
	Destroy();

	// Validate ptr.
	if (!pDevice)
	{
		IG_D3D_ERROR_DUMP(L"CDevice::Create: pDevice must not be NULL.");
		return E_INVALIDARG;
	}

	// Fill data members & create DC.
	m_pDevice = pDevice;
	DeviceContextComPtr pDC;
	m_pDevice->GetImmediateContext(pDC.ReleaseAndGetAddressOf());
	m_ImmediateContext.Create(m_pDevice.Get(), pDC.Get());
	return S_OK;
}

HRESULT CDevice::Create(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE driverType, D3D_FEATURE_LEVEL featureLevel, UINT deviceFlags)
{
#ifdef _DEBUG // add debug flag
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL level[] = { featureLevel };
	D3D_FEATURE_LEVEL createdLevel;

	DeviceContextComPtr pImmDC;
	DeviceComPtr pDevice;
	// Create device	
	IG_D3D_FAILED_RETURN(D3D11CreateDevice(
		pAdapter,
		driverType,
		nullptr,
		deviceFlags,
		level,
		1,
		D3D11_SDK_VERSION,
		pDevice.GetAddressOf(),
		&createdLevel,
		pImmDC.GetAddressOf()), L"CDevice::Create: Failed to create device.");

	return Create(pDevice.Get());
}

void CDevice::Destroy()
{
	m_pDevice = nullptr;
	m_ImmediateContext.Destroy();
}

