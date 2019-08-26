#pragma once

// Usage: wrapper for Sampler State
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"

namespace ig
{
	namespace D3D11
	{

		class CSamplerState
		{
		public:
			// Default construct/copy/assign behaviors.
			CSamplerState() = default;
			~CSamplerState() = default;
			CSamplerState(CSamplerState&&) = default;
			CSamplerState& operator=(CSamplerState&&) = default;
			CSamplerState(const CSamplerState&) = delete;
			CSamplerState& operator=(const CSamplerState&) = delete;

		public:
			// Create from existing resource. 
			HRESULT Create(ID3D11SamplerState *pState);

			// General creation method.
			HRESULT Create(ID3D11Device* pDevice, const D3D11_SAMPLER_DESC *pDesc);

			HRESULT Create(ID3D11Device* pDevice, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
				D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_CLAMP,
				D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_CLAMP,
				D3D11_TEXTURE_ADDRESS_MODE addressW = D3D11_TEXTURE_ADDRESS_CLAMP,
				float mipLODBias = 0.0f, UINT maxAnisotropy = 1,
				D3D11_COMPARISON_FUNC compFunc = D3D11_COMPARISON_NEVER,
				float borderColorR = 1, float borderColorG = 1, float borderColorB = 1, float borderColorA = 1,
				float minLOD = -FLT_MAX, float maxLOD = FLT_MAX);

			// Clear.
			void Destroy();

			// Getters
			ID3D11Device*				DevicePtr() const;
			ID3D11SamplerState*			Ptr() const;
			const D3D11_SAMPLER_DESC&	Desc() const;
			bool						IsValid() const;

			// Convert to ComPtr
			operator ID3D11SamplerState*() const;

		private:
			DeviceComPtr		m_pDevice;
			SamplerStateComPtr	m_pState;
			D3D11_SAMPLER_DESC	m_Desc = {}; // Zero initialization. 
		};

	}
}

#include "inl\igD3D11SamplerState.inl"
