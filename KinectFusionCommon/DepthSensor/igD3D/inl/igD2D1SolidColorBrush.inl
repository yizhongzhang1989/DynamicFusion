#pragma once
#include "igD2D1SolidColorBrush.h"

namespace ig
{
	namespace D2D1
	{
		// Getters
		//inline ID2D1DeviceContext* CSolidColorBrush::DCPtr() const { return m_pDC.Get(); }
		inline ID2D1SolidColorBrush* CSolidColorBrush::Ptr() const { return m_pBrush.Get(); }
		inline bool CSolidColorBrush::IsValid() const { return m_pBrush != nullptr; }
		// Convert to ComPtr
		inline CSolidColorBrush::operator ID2D1SolidColorBrush*() const { return m_pBrush.Get(); }

	}

}




