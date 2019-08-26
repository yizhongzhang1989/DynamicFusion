#include "igD3D12SwapChain.h"
#include "igD3D12Resource.h"

using namespace ig::D3D12;

void CSwapChain::DoInitialization(CDevice::sPtr spDevice, IDXGISwapChain3 *pSwapChain)
{
	m_spDevice = spDevice;
	m_pSwapChain = pSwapChain;

	IG_D3D_ASSERT_SUCCEEDED(pSwapChain->GetDesc(&m_Desc));
	// Get buffers
	m_Bufs.resize(m_Desc.BufferCount);
	m_DSBufs.resize(m_Desc.BufferCount);
	for (UINT i = 0; i < m_Desc.BufferCount; ++i)
	{
		ResourceComPtr pResource;
		IG_D3D_ASSERT_SUCCEEDED(pSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)pResource.GetAddressOf()));
		m_Bufs[i] = CTexture2D::Create(spDevice, pResource.Get(), D3D12_RESOURCE_STATE_COMMON);
		m_DSBufs[i] = CTexture2D::Create(spDevice, m_Desc.BufferDesc.Width, m_Desc.BufferDesc.Height, 1, 1, DXGI_FORMAT_D32_FLOAT,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}
}

CSwapChain::CSwapChain(CDevice::sPtr spDevice, IDXGISwapChain3 *pSwapChain)
{
	DoInitialization(spDevice, pSwapChain);
}

CSwapChain::CSwapChain(CDevice::sPtr spDevice, const DXGI_SWAP_CHAIN_DESC *pDesc)
{
	ig::DXGI::Factory4ComPtr pFactory;
	IG_D3D_ASSERT_SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&pFactory)));

	//Create Swap Chain
	ig::DXGI::SwapChainComPtr pSwapChain;
	DXGI_SWAP_CHAIN_DESC scDesc = *pDesc;
	IG_D3D_ASSERT_SUCCEEDED(pFactory->CreateSwapChain(spDevice->GraphicsQueue()->RawPtr(), &scDesc, pSwapChain.GetAddressOf()));

	ig::DXGI::SwapChain3ComPtr pSwapChain3;
	pSwapChain.As(&pSwapChain3);

	IG_D3D_ASSERT_SUCCEEDED(pSwapChain.As(&pSwapChain3));
	DoInitialization(spDevice, pSwapChain3.Get());
}

HRESULT CSwapChain::Resize(
	UINT bufferCount,
	UINT w, UINT h,
	DXGI_FORMAT format,
	UINT swapchainFlags,
	const UINT* pCreationNodeMask,
	IUnknown *const *ppPresentQueue)
{

	m_Bufs.clear();
	IG_D3D_FAILED_RETURN(m_pSwapChain->ResizeBuffers1(bufferCount, w, h, format, swapchainFlags, pCreationNodeMask, ppPresentQueue));

	DXGI_SWAP_CHAIN_DESC desc;
	IG_D3D_FAILED_RETURN(m_pSwapChain->GetDesc(&desc));

	// Get buffers
	m_Bufs.resize(desc.BufferCount);
	for (UINT i = 0; i < desc.BufferCount; ++i)
	{
		ResourceComPtr pResource;
		IG_D3D_FAILED_RETURN(m_pSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)pResource.GetAddressOf()));

		m_Bufs[i] = CTexture2D::Create(m_spDevice, pResource.Get(), D3D12_RESOURCE_STATE_COMMON);
		m_DSBufs[i] = CTexture2D::Create(m_spDevice, m_Desc.BufferDesc.Width, m_Desc.BufferDesc.Height, 1, 1, DXGI_FORMAT_D32_FLOAT,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}
	return S_OK;
}
