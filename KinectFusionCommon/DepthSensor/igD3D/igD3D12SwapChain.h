#pragma once

// Usage: D3D12 SwapChain
// Created: 2018.9.29
// 2018.9.29: Initial Version.
// Version: 0.1.180929.2215

#include "igD3D12Texture2D.h"
#include <vector>

namespace ig
{
	namespace D3D12
	{
		
		class CSwapChain
		{
		private:
			CDevice::sPtr m_spDevice;
			ig::DXGI::SwapChain3ComPtr m_pSwapChain;
			DXGI_SWAP_CHAIN_DESC m_Desc;
			std::vector<CTexture2D::uPtr> m_Bufs;
			std::vector<CTexture2D::uPtr> m_DSBufs; // default depth buffers, currently only support D32 format

			void DoInitialization(CDevice::sPtr spDevice, IDXGISwapChain3 *pSwapChain);

		public:
			CSwapChain(CDevice::sPtr spDevice, IDXGISwapChain3 *pSwapChain);
			CSwapChain(CDevice::sPtr spDevice, const DXGI_SWAP_CHAIN_DESC *pDesc);
		
			typedef std::shared_ptr<CSwapChain> sPtr;
			typedef std::unique_ptr<CSwapChain> uPtr;
			static uPtr Create(CDevice::sPtr spDevice, IDXGISwapChain3 *pSwapChain);
			static uPtr Create(CDevice::sPtr spDevice, const DXGI_SWAP_CHAIN_DESC *pDesc);

			HRESULT Resize(
				UINT bufferCount,
				UINT w, UINT h,
				DXGI_FORMAT format,
				UINT swapchainFlags,
				const UINT* pCreationNodeMask,
				IUnknown *const *ppPresentQueue);

			HRESULT Present(UINT syncInterval = 0, UINT flags = 0);

			// Getters
			IDXGISwapChain3* Ptr() const;
			const DXGI_SWAP_CHAIN_DESC& Desc() const;
			UINT BufferCount() const;
			UINT CurrentBackBufferIndex() const;
			CTexture2D* BufferPtr(UINT idx);
			CTexture2D* DSBufferPtr(UINT idx);
			D3D12_CPU_DESCRIPTOR_HANDLE RTV(UINT idx);
			D3D12_CPU_DESCRIPTOR_HANDLE DSV(UINT idx);
		};
	}
}

#include "inl\igD3D12SwapChain.inl"




