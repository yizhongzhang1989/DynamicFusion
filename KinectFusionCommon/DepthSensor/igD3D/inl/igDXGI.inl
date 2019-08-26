#pragma once

namespace ig
{
	namespace DXGI
	{
		inline SwapChainDesc& SwapChainDesc::As(
			HWND hWnd,
			DXGI_FORMAT format,
			UINT w, UINT h,
			UINT bufferCount,
			BOOL windowed,
			UINT sampleCount,
			UINT sampleQuality,
			DXGI_USAGE usage,
			UINT refreshRate,
			UINT flags)
		{
			BufferDesc.Width = w;
			BufferDesc.Height = h;
			BufferDesc.RefreshRate.Numerator = refreshRate;
			BufferDesc.RefreshRate.Denominator = 1;
			BufferDesc.Format = format;
			BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			SampleDesc.Count = sampleCount;
			SampleDesc.Quality = sampleQuality;
			BufferUsage = usage;
			BufferCount = bufferCount;
			OutputWindow = hWnd;
			Windowed = windowed;
			SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			Flags = flags;

			return *this;
		}

		inline SwapChainDesc::operator DXGI_SWAP_CHAIN_DESC*()
		{
			return this;
		}
	}
}

