#include "igD2D1DeviceContext.h"
using namespace ig::D2D1;

// Create from existing pDC ptr.
HRESULT CDeviceContext::Create(ID2D1DeviceContext *pDC)
{
	// Clear
	Destroy();
	// Validate ptr.
	if (!pDC)
	{
		IG_D3D_ERROR_DUMP(L"CDeviceContext::Create: pDC must not be NULL.");
		return E_INVALIDARG;
	}

	pDC->GetDevice(m_pDevice.GetAddressOf());
	m_pDC = pDC;
	return S_OK;
}

// Create from D3D device.
HRESULT CDeviceContext::Create(
	ID2D1Device *pDevice,
	D2D1_DEVICE_CONTEXT_OPTIONS options)
{
	DeviceContextComPtr pDC;
	IG_D3D_FAILED_RETURN(pDevice->CreateDeviceContext(options, pDC.GetAddressOf()),
		L"CDeviceContext::Create: failed to create device context.");
	return Create(pDC.Get());
}

// Clear.
void CDeviceContext::Destroy()
{
	m_pDC = nullptr;
	m_pDevice = nullptr;
}


