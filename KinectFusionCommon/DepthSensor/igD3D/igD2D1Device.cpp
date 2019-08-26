#include "igD2D1Device.h"
#include <vector>

using namespace ig::D2D1;

HRESULT CDevice::Create(ID2D1Device *pDevice)
{
	// Clear
	Destroy();

	// Validate ptr.
	if (!pDevice)
	{
		IG_D3D_ERROR_DUMP(L"CDevice::Create: pDevice must not be NULL.");
		return E_INVALIDARG;
	}

	m_pDevice = pDevice;
	return S_OK;
}

HRESULT CDevice::Create(ID3D11Device *pD3D11Device, D2D1_FACTORY_TYPE factoryType)
{
	if (!pD3D11Device)
	{
		IG_D3D_ERROR_DUMP(L"CDevice::Create: pD3D11Device must not be NULL.");
		return E_INVALIDARG;
	}

	DeviceComPtr pDevice;
	D2D1_FACTORY_OPTIONS *pOptions = nullptr;

/*#ifdef _DEBUG
	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	pOptions = &options;
#endif*/	

	IG_D3D_FAILED_RETURN(D2D1CreateFactory(
		factoryType, 
		__uuidof(ID2D1Factory1), 
		pOptions, 
		(void**)m_pFactory.GetAddressOf()), 
		L"CDevice::Create: Failed to create d2d1factory.");

	ig::DXGI::DeviceComPtr pDXGIDevice;
	IG_D3D_FAILED_RETURN(pD3D11Device->QueryInterface(pDXGIDevice.GetAddressOf()),
		L"CDevice::Create: Failed to get dxgiDevice.");

	IG_D3D_FAILED_RETURN(m_pFactory->CreateDevice(pDXGIDevice.Get(), pDevice.GetAddressOf()),
		L"CDevice::Create: Failed to create device.");
	
	return Create(pDevice.Get());
}

void CDevice::Destroy()
{
	m_pDevice = nullptr;
	m_pFactory = nullptr;
}

