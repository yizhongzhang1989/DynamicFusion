#pragma once

// Usage: wrapper for Rasterizer State
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"

namespace ig
{
	namespace D3D11
	{
		class CRasterizerState
		{
		public:
			CRasterizerState() = default;
			~CRasterizerState() = default;
			CRasterizerState(CRasterizerState&&) = default;
			CRasterizerState& operator=(CRasterizerState&&) = default;
			CRasterizerState(const CRasterizerState&) = delete;
			CRasterizerState& operator=(const CRasterizerState&) = delete;

			// Create from existing resource.
			HRESULT Create(ID3D11RasterizerState* pState);
			
			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, const D3D11_RASTERIZER_DESC *pDesc);
			
			HRESULT Create(ID3D11Device* pDevice, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
				D3D11_CULL_MODE cullMode = D3D11_CULL_BACK, bool bIsFrontCounterClockwise = false,
				int depthBias = 0, float depthBiasClamp = 0.0f, float slopScaledDepthBias = 0.0f,
				bool bEnableDepthClip = true, bool bEnableScissor = false, bool bEnableMultisample = false,
				bool bEnableAntialiasedLine = false);
			
			// Clear.
			void Destroy();

			// Getters
			ID3D11Device*					DevicePtr() const;
			ID3D11RasterizerState*			Ptr() const;
			const D3D11_RASTERIZER_DESC&	Desc() const;
			bool							IsValid() const;

			// Convert to ComPtr
			operator ID3D11RasterizerState*() const;

		private:
			DeviceComPtr			m_pDevice;
			RasterizerStateComPtr	m_pState;
			D3D11_RASTERIZER_DESC	m_Desc = {}; // Zero initialization. 
		};

	}

}

#include "inl\igD3D11RasterizerState.inl"
