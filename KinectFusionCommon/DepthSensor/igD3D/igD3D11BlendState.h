#pragma once

// Usage: wrapper for Blend State
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"

namespace ig
{
	namespace D3D11
	{
		class CBlendState
		{
		public:
			CBlendState() = default;
			~CBlendState() = default;
			CBlendState(CBlendState&&) = default;
			CBlendState& operator=(CBlendState&&) = default;
			CBlendState(const CBlendState&) = delete;
			CBlendState& operator=(const CBlendState&) = delete;

			// Create from existing resource. 
			HRESULT Create(ID3D11BlendState *pState);

			// General creation method.
			HRESULT Create(ID3D11Device *pDevice, const D3D11_BLEND_DESC *pDesc);

			// Uniform blend creation method.
			HRESULT Create(ID3D11Device* pDevice, bool bEnableBlend = false, bool bEnableAlphaToCoverage = false,
				D3D11_BLEND srcBlend = D3D11_BLEND_ONE, D3D11_BLEND dstBlend = D3D11_BLEND_ZERO, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
				D3D11_BLEND srcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND dstBlendAlpha = D3D11_BLEND_ZERO, D3D11_BLEND_OP blendOpAlpha = D3D11_BLEND_OP_ADD,
				UINT8 RTWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device*				DevicePtr() const;
			ID3D11BlendState*			Ptr() const;
			const D3D11_BLEND_DESC&		Desc() const;
			bool						IsValid() const;

			// Convert to ComPtr
			operator ID3D11BlendState*() const;

		private:
			DeviceComPtr		m_pDevice;
			BlendStateComPtr	m_pState;
			D3D11_BLEND_DESC	m_Desc = {}; // Zero initialization. 

		};

	}

}

#include "inl\igD3D11BlendState.inl"
