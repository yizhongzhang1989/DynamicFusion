#pragma once

// Usage: igD2D1 Common Header
// Created: 2018.4.19
// Last Modified: 2018.4.19
// 2018.4.19: Initial Version.
// Version: 0.1.180419.1445

// TCHAR
#include <tchar.h>

// D3D
#include <d3d11.h>

// D2D
#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

// WRL
#include <wrl.h>

// shared_ptr
#include <memory>

// ComPtr Defs

namespace ig
{
	namespace D2D1
	{
		//using namespace DirectX;
		using Microsoft::WRL::ComPtr;
		//**************** ComPtr Typedefs ****************//
		// factory
		typedef ComPtr<ID2D1Factory>				FactoryComPtr;
		typedef ComPtr<ID2D1Factory1>				Factory1ComPtr;

		// device
		typedef ComPtr<ID2D1Device>					DeviceComPtr;
		typedef ComPtr<ID2D1DeviceContext>			DeviceContextComPtr;

		// Bitmap
		typedef ComPtr<ID2D1Bitmap>					BitmapComPtr;
		typedef ComPtr<ID2D1Bitmap1>				Bitmap1ComPtr;

		// Brush
		typedef ComPtr<ID2D1SolidColorBrush>		SolidColorBrushComPtr;
	}
}

#include "igD3DCommon.h"
#include "igDXGI.h"


