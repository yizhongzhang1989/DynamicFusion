#pragma once
#include "igD3D12SwapChain.h"

namespace ig
{
	namespace D3D12
	{
		inline CSwapChain::uPtr CSwapChain::Create(CDevice::sPtr spDevice, IDXGISwapChain3 *pSwapChain)
		{
			return std::make_unique<CSwapChain>(spDevice, pSwapChain);
		}

		inline CSwapChain::uPtr CSwapChain::Create(CDevice::sPtr spDevice, const DXGI_SWAP_CHAIN_DESC *pDesc)
		{
			return std::make_unique<CSwapChain>(spDevice, pDesc);
		}

		inline IDXGISwapChain3* CSwapChain::Ptr() const
		{
			return m_pSwapChain.Get();
		}

		inline const DXGI_SWAP_CHAIN_DESC& CSwapChain::Desc() const
		{
			return m_Desc;
		}
		inline CTexture2D* CSwapChain::BufferPtr(UINT idx)
		{
			return m_Bufs[idx].get();
		}
		inline CTexture2D* CSwapChain::DSBufferPtr(UINT idx)
		{
			return m_DSBufs[idx].get();
		}
		inline UINT CSwapChain::BufferCount() const
		{
			return m_Desc.BufferCount;
		}

		inline UINT CSwapChain::CurrentBackBufferIndex() const
		{
			return m_pSwapChain->GetCurrentBackBufferIndex();
		}

		inline HRESULT CSwapChain::Present(UINT syncInterval, UINT flags)
		{
			IG_D3D_FAILED_RETURN(m_pSwapChain->Present(syncInterval, flags));
			return S_OK;
		}

		inline D3D12_CPU_DESCRIPTOR_HANDLE CSwapChain::RTV(UINT idx)
		{ 
			return BufferPtr(idx)->RTV(); 
		}
		inline D3D12_CPU_DESCRIPTOR_HANDLE CSwapChain::DSV(UINT idx)
		{ 
			return DSBufferPtr(idx)->DSV(); 
		}
	}
}







