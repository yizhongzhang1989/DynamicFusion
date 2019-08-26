#pragma once

#pragma once

// Usage: simple wrapper for SolidColorBrush
// Created: 2018.4.19
// Last Modified: 2018.4.19
// 2018.4.19: Initial Version.
// Version: 0.1.180419.1945

#include "igD2D1Defs.h"

namespace ig
{
	namespace D2D1
	{
		class CSolidColorBrush
		{
		public:
			// Default construct/copy/assign behaviors.
			CSolidColorBrush() = default;
			~CSolidColorBrush() = default;

			CSolidColorBrush(CSolidColorBrush&&) = default;
			CSolidColorBrush& operator=(CSolidColorBrush&&) = default;

			CSolidColorBrush(const CSolidColorBrush&) = delete;
			CSolidColorBrush& operator=(const CSolidColorBrush&) = delete;

			// Create from existing ptr.
			HRESULT Create(ID2D1SolidColorBrush *pBrush);
			
			HRESULT Create(
				ID2D1DeviceContext *pDC, 
				const D2D1_COLOR_F& color, 
				const D2D1_BRUSH_PROPERTIES *pBrushProperties = nullptr);

			// Clear.
			void Destroy();

			// Getters
			//ID2D1DeviceContext* DCPtr()	const;
			ID2D1SolidColorBrush* Ptr()	const;
			//CDeviceContext& ImmediateContext();
			bool IsValid() const;

			// Convert to ComPtr
			operator ID2D1SolidColorBrush*() const;

		private:
			//DeviceContextComPtr m_pDC;
			SolidColorBrushComPtr m_pBrush;
		};

	}

}

#include "inl\igD2D1SolidColorBrush.inl"


