#pragma once

// Usage: CResource base class
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Defs.h"
#include <unordered_map>

namespace ig
{
	namespace D3D11
	{
		class CResource
		{
		public:
			// Public Operations
			void CopyTo(ID3D11Resource *pDstRes, ID3D11DeviceContext *pOptionalDC = nullptr);
			void CopyFrom(ID3D11Resource *pSrcRes, ID3D11DeviceContext *pOptionalDC = nullptr);
			
			// Getters
			ID3D11Resource*		ResourcePtr() const;
			ID3D11Device*		DevicePtr() const;
			bool				IsValid() const;
			
			// Static methods
			static UINT CalMipLevels(UINT size);		

		protected:
			CResource() = default;
			~CResource() = default;
			CResource(CResource&&) = default;
			CResource& operator=(CResource&&) = default;
			CResource(const CResource&) = delete;
			CResource& operator=(const CResource&) = delete;
			
			// Create from existing resource.
			HRESULT Create(ID3D11Resource *pResource);
			
			// Clear.
			void Destroy();

			// For general views.
			ID3D11ShaderResourceView*	RetriveD3DSRV(const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc);
			ID3D11RenderTargetView*		RetriveD3DRTV(const D3D11_RENDER_TARGET_VIEW_DESC *pDesc);
			ID3D11UnorderedAccessView*	RetriveD3DUAV(const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc);
			ID3D11DepthStencilView*		RetriveD3DDSV(const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc);			

			DeviceComPtr		m_pDevice;
			DeviceContextComPtr m_pImmediateDC;
			ResourceComPtr		m_pRes;
			std::shared_ptr<std::unordered_map<size_t, ShaderResourceViewComPtr>>	m_spSRVs;
			std::shared_ptr<std::unordered_map<size_t, RenderTargetViewComPtr>>		m_spRTVs;
			std::shared_ptr<std::unordered_map<size_t, UnorderedAccessViewComPtr>>	m_spUAVs;
			std::shared_ptr<std::unordered_map<size_t, DepthStencilViewComPtr>>		m_spDSVs;

		};
	}
}

#include "inl\igD3D11Resource.inl"