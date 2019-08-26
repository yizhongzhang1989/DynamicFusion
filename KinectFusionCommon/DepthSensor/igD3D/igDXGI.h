#pragma once

// Usage: DXGI Common Header

// Created: 2018.9.28
// 2018.9.28: Initial Version.
// Version: 0.1.180928.2215

#include <wrl.h>
#include <dxgi.h>
#include <dxgi1_4.h> // for D3D12

#include <vector>
#pragma comment(lib, "dxgi.lib")

#include "igD3DCommon.h"

namespace ig
{
	namespace DXGI
	{
		using Microsoft::WRL::ComPtr;
		// DXGI
		typedef ComPtr<IDXGIFactory>	FactoryComPtr;
		typedef ComPtr<IDXGIFactory1>	Factory1ComPtr;
		typedef ComPtr<IDXGIFactory2>	Factory2ComPtr;
		typedef ComPtr<IDXGIFactory3>	Factory3ComPtr;
		typedef ComPtr<IDXGIFactory4>	Factory4ComPtr;

		typedef ComPtr<IDXGIAdapter>	AdapterComPtr;

		typedef ComPtr<IDXGISwapChain>	SwapChainComPtr;
		typedef ComPtr<IDXGISwapChain1> SwapChain1ComPtr;
		typedef ComPtr<IDXGISwapChain2> SwapChain2ComPtr;
		typedef ComPtr<IDXGISwapChain3> SwapChain3ComPtr;

		typedef ComPtr<IDXGIDevice>		DeviceComPtr;
		typedef ComPtr<IDXGIDevice1>	Device1ComPtr;

		typedef ComPtr<IDXGISurface>	SurfaceComPtr;
		typedef ComPtr<IDXGISurface1>	Surface1ComPtr;

		// Utls
		// Format
		UINT GetDXGIFormatBytes(DXGI_FORMAT format);
		bool IsTypeless(DXGI_FORMAT format);
		bool IsDepthFormat(DXGI_FORMAT format);
		DXGI_FORMAT TypelessToDepthFormat(DXGI_FORMAT format);
		DXGI_FORMAT TypelessToFloatFormat(DXGI_FORMAT format);
		bool IsSRGB(DXGI_FORMAT format);
		DXGI_FORMAT SRGB2NonSRGB(DXGI_FORMAT format);

		// Adapter
		std::vector<DXGI::AdapterComPtr> GetAllAdapters();
		std::vector<DXGI::AdapterComPtr> GetDedicateAdapters();
		DXGI::AdapterComPtr GetPrimaryAdapter();

		// Structure Wrapper
		struct SwapChainDesc : public DXGI_SWAP_CHAIN_DESC
		{
			SwapChainDesc& As(
				HWND hWnd,
				DXGI_FORMAT format,
				UINT w, UINT h,
				UINT bufferCount,
				BOOL windowed = true,
				UINT sampleCount = 1,
				UINT sampleQuality = 0,
				DXGI_USAGE usage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
				UINT refreshRate = 60,
				UINT flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

			operator DXGI_SWAP_CHAIN_DESC*();
		};
	}
}

#include "inl\igDXGI.inl"