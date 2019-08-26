#include "igD2D1SolidColorBrush.h"
using namespace ig::D2D1;

// Create from existing pDC ptr.
HRESULT CSolidColorBrush::Create(ID2D1SolidColorBrush *pBrush)
{
	// Clear
	Destroy();
	// Validate ptr.
	if (!pBrush)
	{
		IG_D3D_ERROR_DUMP(L"CSolidColorBrush::Create: pBrush must not be NULL.");
		return E_INVALIDARG;
	}
	m_pBrush = pBrush;
	return S_OK;
}

// Create from D3D device.
HRESULT CSolidColorBrush::Create(
	ID2D1DeviceContext *pDC, const D2D1_COLOR_F& color, const D2D1_BRUSH_PROPERTIES *pBrushProperties)
{
	SolidColorBrushComPtr pBrush;
	IG_D3D_FAILED_RETURN(pDC->CreateSolidColorBrush(&color, pBrushProperties, pBrush.GetAddressOf()), 
		L"CSolidColorBrush::Create: failed to create brush.");

	return Create(pBrush.Get());
}

// Clear.
void CSolidColorBrush::Destroy()
{
	m_pBrush = nullptr;
}


