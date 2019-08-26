#include "igD3D11SwapChain.h"

using namespace ig::D3D11;

HRESULT CSwapChain::Create(IDXGISwapChain *pSwapChain, ID2D1DeviceContext *pD2D1DC, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties)
{
	// Clear
	Destroy();
	// Validate ptr.
	if (!pSwapChain)
	{
		IG_D3D_ERROR_DUMP(L"CSwapChain::Create: pSwapChain must not be NULL.");
		return E_INVALIDARG;
	}

	// Queries.
	DeviceComPtr pDevice;
	IG_D3D_FAILED_RETURN(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(pDevice.GetAddressOf())), L"CSwapChain::Create: Failed to get device.");
	// Acquire the texture interface from the swap chain.
	Texture2DComPtr pTex;
	IG_D3D_FAILED_RETURN(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pTex.GetAddressOf())), L"CSwapChain::Create: Failed to get back buffer.");
	RenderTargetViewComPtr pRTV;
	IG_D3D_FAILED_RETURN(pDevice->CreateRenderTargetView(pTex.Get(), nullptr, pRTV.GetAddressOf()), L"CSwapChain::Create: Failed to create render target view.");

	// Acquire DXGI interface from swap chain
	DXGI::SurfaceComPtr pDXGISurface;
	IG_D3D_FAILED_RETURN(pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(pDXGISurface.GetAddressOf())), L"CSwapChain::Create: Failed to get DXGISurface.");

	/*D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProperties.dpiX = 0;
	bitmapProperties.dpiY = 0;
	bitmapProperties.colorContext = nullptr;*/

	ComPtr<ID2D1Bitmap1> pBitmap;
	if (pD2D1DC)
	{
		IG_D3D_FAILED_RETURN(pD2D1DC->CreateBitmapFromDxgiSurface(pDXGISurface.Get(), pD2D1BitmapProperties, pBitmap.GetAddressOf()), L"CSwapChain::Create: Failed to create d2d1bitmap.");
	}
	// Fill data members.
	m_pSwapChain = pSwapChain;
	m_pDevice = pDevice;
	m_pTex = pTex;
	m_pRTV = pRTV;
	m_pDXGISurface = pDXGISurface;
	m_pD2D1DC = pD2D1DC;
	m_pD2D1Bitmap1 = pBitmap;
	m_pSwapChain->GetDesc(&m_Desc);

	return S_OK;

}

HRESULT CSwapChain::Create(ID3D11Device* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, ID2D1DeviceContext *pD2D1DC, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties)
{
	if (!pDevice)
	{
		IG_D3D_ERROR_DUMP(L"CSwapChain::Create: pDevice must not be NULL.");
		return E_INVALIDARG;
	}

	ComPtr<IDXGIDevice> pDXGIDevice;
	IG_D3D_FAILED_RETURN(pDevice->QueryInterface(pDXGIDevice.GetAddressOf()), L"CSwapChain::Create: Failed to query IDXGIDevice interface.");

	ComPtr<IDXGIAdapter> pDXGIAdapter;
	IG_D3D_FAILED_RETURN(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void **>(pDXGIAdapter.GetAddressOf())), L"CSwapChain::Create: Failed to get IDXGIAdapter.");

	ComPtr<IDXGIFactory> pFactory;
	IG_D3D_FAILED_RETURN(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void **>(pFactory.GetAddressOf())), L"CSwapChain::Create: Failed to get IDXGIFactory.");
	
	//Create Swap Chain
	DXGI::SwapChainComPtr pSwapChain;
	IG_D3D_FAILED_RETURN(pFactory->CreateSwapChain(pDevice, pDesc, pSwapChain.GetAddressOf()), L"CSwapChain::Create: Failed to create swap-chain.");

	return Create(pSwapChain.Get(), pD2D1DC, pD2D1BitmapProperties);
}

HRESULT CSwapChain::Create(ID3D11Device* pDevice, HWND hWnd, UINT w, UINT h, bool bIsWindowed, DXGI_FORMAT format,
	DXGI_USAGE usage, UINT refreshRate, UINT bufferCount)
{
	// Ignore flag & effect
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = bufferCount;
	sd.BufferDesc.Width = w;
	sd.BufferDesc.Height = h;
	sd.BufferDesc.Format = format;
	sd.BufferDesc.RefreshRate.Numerator = refreshRate;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = usage;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = bIsWindowed ? TRUE : FALSE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (ig::DXGI::IsSRGB(format))
	{
		// Flip model does not support srgb format
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	else
	{
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	}
	return Create(pDevice, &sd);
}

HRESULT CSwapChain::CreateMS(ID3D11Device* pDevice, HWND hWnd, UINT w, UINT h, UINT sampleCount, UINT sampleQuality, bool bIsWindowed, DXGI_FORMAT format,
	DXGI_USAGE usage, UINT refreshRate, UINT bufferCount)
{
	// Ignore flag & effect
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = bufferCount;
	sd.BufferDesc.Width = w;
	sd.BufferDesc.Height = h;
	sd.BufferDesc.Format = format;
	sd.BufferDesc.RefreshRate.Numerator = refreshRate;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = usage;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = sampleCount;
	sd.SampleDesc.Quality = sampleQuality;
	sd.Windowed = bIsWindowed ? TRUE : FALSE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (ig::DXGI::IsSRGB(format))
	{
		// Flip model does not support srgb format
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	else
	{
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	}
	return Create(pDevice, &sd);
}

HRESULT CSwapChain::CreateWithD2D1Support(ID3D11Device* pDevice, ID2D1DeviceContext *pD2D1DC, HWND hWnd, UINT w, UINT h, bool bIsWindowed, DXGI_FORMAT format,
	DXGI_USAGE usage , UINT refreshRate, UINT bufferCount, D2D1_BITMAP_PROPERTIES1 *pD2D1BitmapProperties)
{
	if (!pD2D1DC)
	{
		IG_D3D_ERROR_DUMP(L"CSwapChain::CreateWithD2D1Support: pD2D1DC must not be NULL.");
		return E_INVALIDARG;
	}
	// Ignore flag & effect
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = bufferCount;
	sd.BufferDesc.Width = w;
	sd.BufferDesc.Height = h;
	sd.BufferDesc.Format = format;
	sd.BufferDesc.RefreshRate.Numerator = refreshRate;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = usage;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = bIsWindowed ? TRUE : FALSE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (ig::DXGI::IsSRGB(format))
	{
		// Flip model does not support srgb format
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	else
	{
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	}
	return Create(pDevice, &sd, pD2D1DC, pD2D1BitmapProperties);
}

void CSwapChain::Destroy()
{
	m_pDevice = nullptr;
	m_pSwapChain = nullptr;
	m_pTex = nullptr;
	m_pRTV = nullptr;
	m_pDXGISurface = nullptr;
	m_pD2D1Bitmap1 = nullptr;
	m_pD2D1DC = nullptr;
	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

HRESULT CSwapChain::Resize(UINT w, UINT h, DXGI_FORMAT newFormat)
{
	D2D1_BITMAP_PROPERTIES1 prop;
	
	Microsoft::WRL::ComPtr<ID2D1ColorContext> pColorContext;
	if (m_pD2D1Bitmap1)
	{
		m_pD2D1Bitmap1->GetColorContext(pColorContext.GetAddressOf());
		prop.colorContext = pColorContext.Get();
		prop.bitmapOptions = m_pD2D1Bitmap1->GetOptions();
		float dpiX, dpiY;
		m_pD2D1Bitmap1->GetDpi(&dpiX, &dpiY);
		prop.dpiX = dpiX;
		prop.dpiY = dpiY;
		prop.pixelFormat = m_pD2D1Bitmap1->GetPixelFormat();
		//if(newFormat != prop.pixelFormat.format)
		// TODO: change pixel format if necessary.
		//D2D1_PIXEL_FORMAT fmt;
		//fmt.alphaMode
	}
	//D2D1_PIXEL_FORMAT
	m_pTex = nullptr;
	m_pRTV = nullptr;
	m_pDXGISurface = nullptr;
	m_pD2D1Bitmap1 = nullptr;

	IG_D3D_FAILED_RETURN(m_pSwapChain->ResizeBuffers(m_Desc.BufferCount, w, h, newFormat, m_Desc.Flags), L"CSwapChain::Resize: Failed to resize swap-chain.");

	// recreate RTV	
	IG_D3D_FAILED_RETURN(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_pTex.GetAddressOf())), L"CSwapChain::Resize: Failed to get back buffer.");
	IG_D3D_FAILED_RETURN(m_pDevice->CreateRenderTargetView(m_pTex.Get(), nullptr, m_pRTV.GetAddressOf()), L"CSwapChain::Resize: Failed to create render target view.");

	IG_D3D_FAILED_RETURN(m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(m_pDXGISurface.GetAddressOf())), L"CSwapChain::Resize: Failed to get DXGISurface.");
	if (m_pD2D1DC)
	{
		IG_D3D_FAILED_RETURN(m_pD2D1DC->CreateBitmapFromDxgiSurface(m_pDXGISurface.Get(), &prop, m_pD2D1Bitmap1.GetAddressOf()), L"CSwapChain::Resize: Failed to create d2d1bitmap.");
	}

	// Get Desc
	IG_D3D_FAILED_RETURN(m_pSwapChain->GetDesc(&m_Desc), L"CSwapChain::Resize: Failed to get Desc structure.");
	return S_OK;
}

