#pragma once

// Usage: wrapper for Depth-Stencil State
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"

namespace ig
{
	namespace D3D11
	{
		class CDepthStencilState
		{
		public:
			CDepthStencilState() = default;
			~CDepthStencilState() = default;
			CDepthStencilState(CDepthStencilState&&) = default;
			CDepthStencilState& operator=(CDepthStencilState&&) = default;
			CDepthStencilState(const CDepthStencilState&) = delete;
			CDepthStencilState& operator=(const CDepthStencilState&) = delete;

			// Create from existing resource.
			HRESULT Create(ID3D11DepthStencilState *pState);

			// General Creation method.
			HRESULT Create(ID3D11Device* pDevice, const D3D11_DEPTH_STENCIL_DESC *pDesc);
			
			HRESULT Create(ID3D11Device* pDevice, bool bEnableDepth = true, bool bEnableDepthWrite = true, D3D11_COMPARISON_FUNC depthFunc = D3D11_COMPARISON_LESS,
				bool bEnableStencil = false, UINT8 stencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK, UINT8 stencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
				D3D11_STENCIL_OP frontStencilFailOp = D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP frontStencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
				D3D11_STENCIL_OP frontStencilPassOp = D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_FUNC frontStencilFunc = D3D11_COMPARISON_ALWAYS,
				D3D11_STENCIL_OP backStencilFailOp = D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP backStencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
				D3D11_STENCIL_OP backStencilPassOp = D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_FUNC backStencilFunc = D3D11_COMPARISON_ALWAYS);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device*					DevicePtr() const;
			ID3D11DepthStencilState*		Ptr() const;
			const D3D11_DEPTH_STENCIL_DESC& Desc() const;
			bool							IsValid() const;

			// Convert to ComPtr
			operator ID3D11DepthStencilState*() const;

		private:
			DeviceComPtr				m_pDevice;
			DepthStencilStateComPtr		m_pState;
			D3D11_DEPTH_STENCIL_DESC	m_Desc = {}; // Zero initialization. 
		};

	}

}

#include "inl\igD3D11DepthStencilState.inl"