#pragma once


namespace ig
{
	namespace D3D11
	{

		inline void CDeviceContext::InputAssemblerStage::Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC)
		{
			m_pDevice = pDevice;
			m_pDC = pDC;
		}
		inline void CDeviceContext::InputAssemblerStage::Destroy()
		{
			m_pDevice = nullptr;
			m_pDC = nullptr;
		}
		inline void CDeviceContext::InputAssemblerStage::SetInputLayout(ID3D11InputLayout *pIA)
		{
			m_pDC->IASetInputLayout(pIA);
		}
		inline void CDeviceContext::InputAssemblerStage::SetVertexBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppVBs, const UINT *pStrides, const UINT *pOffsets)
		{
			m_pDC->IASetVertexBuffers(startSlot, numBuffers, ppVBs, pStrides, pOffsets);
		}
		inline void CDeviceContext::InputAssemblerStage::SetVertexBuffer(UINT slot, ID3D11Buffer *pVB, UINT stride, UINT offset)
		{
			SetVertexBuffers(slot, 1, &pVB, &stride, &offset);
		}
		inline void CDeviceContext::InputAssemblerStage::SetIndexBuffer(ID3D11Buffer *pIB, DXGI_FORMAT format, UINT offset)
		{
			m_pDC->IASetIndexBuffer(pIB, format, offset);
		}
		inline void CDeviceContext::InputAssemblerStage::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
		{
			m_pDC->IASetPrimitiveTopology(topology);
		}

		inline void CDeviceContext::RasterizerStage::Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC)
		{
			m_pDevice = pDevice;
			m_pDC = pDC;
		}
		inline void CDeviceContext::RasterizerStage::Destroy()
		{
			m_pDevice = nullptr;
			m_pDC = nullptr;
            m_CCWRSState.Destroy();
			m_TwoSidedRSState.Destroy();
		}
		inline void CDeviceContext::RasterizerStage::SetRasterizer(ID3D11RasterizerState *pRSState)
		{
			m_pDC->RSSetState(pRSState);
		}
		inline void CDeviceContext::RasterizerStage::SetRasterizerDefault()
		{
			m_pDC->RSSetState(nullptr);
		}
        inline void CDeviceContext::RasterizerStage::SetRasterizerCCW()
        {
            if (!m_CCWRSState.IsValid() || m_CCWRSState.DevicePtr() != m_pDevice.Get())
            {
                m_CCWRSState.Create(m_pDevice.Get(), D3D11_FILL_SOLID, D3D11_CULL_BACK, true);
				D3D_SET_OBJECT_NAME_A(m_CCWRSState.Ptr(), "::CCW-RSState");
            }
            SetRasterizer(m_CCWRSState.Ptr());
        }
		inline void CDeviceContext::RasterizerStage::SetRasterizerTwoSided()
		{
			if (!m_TwoSidedRSState.IsValid() || m_TwoSidedRSState.DevicePtr() != m_pDevice.Get())
			{
				m_TwoSidedRSState.Create(m_pDevice.Get(), D3D11_FILL_SOLID, D3D11_CULL_NONE);
				D3D_SET_OBJECT_NAME_A(m_TwoSidedRSState.Ptr(), "::TwoSided-RSState");
			}
			SetRasterizer(m_TwoSidedRSState.Ptr());
		}
		inline void CDeviceContext::RasterizerStage::SetViewports(UINT numVPs, const D3D11_VIEWPORT *pVPs)
		{
			m_pDC->RSSetViewports(numVPs, pVPs);
		}
		inline void CDeviceContext::RasterizerStage::SetViewport(D3D11_VIEWPORT vp)
		{
			m_pDC->RSSetViewports(1, &vp);
		}
		inline void CDeviceContext::RasterizerStage::SetViewport(FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth, FLOAT maxDepth)
		{
			CD3D11_VIEWPORT vp(topLeftX, topLeftY, width, height, minDepth, maxDepth);
			m_pDC->RSSetViewports(1, &vp);
		}
		inline void CDeviceContext::RasterizerStage::SetScissorRects(UINT numRects, const D3D11_RECT *pRects)
		{
			m_pDC->RSSetScissorRects(numRects, pRects);
		}


		inline void CDeviceContext::OutputMergerStage::Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC)
		{
			m_pDevice = pDevice;
			m_pDC = pDC;
		}
		inline void CDeviceContext::OutputMergerStage::Destroy()
		{
			m_pDevice = nullptr;
			m_pDC = nullptr;
			m_DisableDSState.Destroy();
			m_ReadOnlyDSState.Destroy();
			m_GreaterEqualDSState.Destroy();
			m_AdditiveBlendState.Destroy();
		}
		inline void CDeviceContext::OutputMergerStage::SetDepthStencil(ID3D11DepthStencilState *pDSState, UINT stencilRef)
		{
			m_pDC->OMSetDepthStencilState(pDSState, stencilRef);
		}
		inline void CDeviceContext::OutputMergerStage::SetDepthStencilDefault()
		{
			m_pDC->OMSetDepthStencilState(nullptr, 0);
		}
		inline void CDeviceContext::OutputMergerStage::SetDepthStencilReadOnly()
		{
			if (!m_ReadOnlyDSState.IsValid() || m_ReadOnlyDSState.DevicePtr() != m_pDevice.Get())
			{
				m_ReadOnlyDSState.Create(m_pDevice.Get(), true, false, D3D11_COMPARISON_LESS_EQUAL);
				D3D_SET_OBJECT_NAME_A(m_ReadOnlyDSState.Ptr(), "::ReadOnly-DSState");
			}
			SetDepthStencil(m_ReadOnlyDSState.Ptr(), 0);
		}
		inline void CDeviceContext::OutputMergerStage::SetDepthStencilGreaterEqual()
		{
			if (!m_GreaterEqualDSState.IsValid() || m_GreaterEqualDSState.DevicePtr() != m_pDevice.Get())
			{
				m_GreaterEqualDSState.Create(m_pDevice.Get(), true, true, D3D11_COMPARISON_GREATER_EQUAL);
				D3D_SET_OBJECT_NAME_A(m_GreaterEqualDSState.Ptr(), "::GreaterEqual-DSState");
			}
			SetDepthStencil(m_GreaterEqualDSState.Ptr(), 0);
		}
		inline void CDeviceContext::OutputMergerStage::SetDepthStencilLessEqual()
		{
			if (!m_LessEqualDSState.IsValid() || m_LessEqualDSState.DevicePtr() != m_pDevice.Get())
			{
				m_LessEqualDSState.Create(m_pDevice.Get(), true, true, D3D11_COMPARISON_LESS_EQUAL);
				D3D_SET_OBJECT_NAME_A(m_LessEqualDSState.Ptr(), "::LessEqual-DSState");
			}
			SetDepthStencil(m_LessEqualDSState.Ptr(), 0);
		}
		inline void CDeviceContext::OutputMergerStage::DisableDepthStencil()
		{
			if (!m_DisableDSState.IsValid() || m_DisableDSState.DevicePtr() != m_pDevice.Get())
			{
				m_DisableDSState.Create(m_pDevice.Get(), false, false);
				D3D_SET_OBJECT_NAME_A(m_DisableDSState.Ptr(), "::Disable-DSState");
			}
			SetDepthStencil(m_DisableDSState.Ptr(), 0);
		}
		inline void CDeviceContext::OutputMergerStage::SetBlend(ID3D11BlendState *pBlendState, const FLOAT *blendFactor, UINT sampleMask)
		{
			m_pDC->OMSetBlendState(pBlendState, blendFactor, sampleMask);
		}
		inline void CDeviceContext::OutputMergerStage::SetBlendAdditive()
		{
			if (!m_AdditiveBlendState.IsValid() || m_AdditiveBlendState.DevicePtr() != m_pDevice.Get())
			{
				m_AdditiveBlendState.Create(
					m_pDevice.Get(), true, false,
					D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
					D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
					D3D11_COLOR_WRITE_ENABLE_ALL);
				D3D_SET_OBJECT_NAME_A(m_AdditiveBlendState.Ptr(), "::Additive-DSState");
			}
			SetBlend(m_AdditiveBlendState.Ptr());
		}
		inline void CDeviceContext::OutputMergerStage::DisableBlend()
		{
			m_pDC->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		}
		inline void CDeviceContext::OutputMergerStage::SetRenderTargets(UINT numViews, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView)
		{
			m_pDC->OMSetRenderTargets(numViews, ppRenderTargetViews, pDepthStencilView);
		}
		inline void CDeviceContext::OutputMergerStage::SetRenderTarget(ID3D11RenderTargetView*pRenderTargetView, ID3D11DepthStencilView *pDepthStencilView)
		{
			SetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
		}

		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC)
		{
			m_pDevice = pDevice;
			m_pDC = pDC;
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::Destroy()
		{
			m_pDevice = nullptr;
			m_pDC = nullptr;
			m_PointClampSampler.Destroy();
			m_PointWrapSampler.Destroy();
			m_PointMirrorSampler.Destroy();
			m_LinearClampSampler.Destroy();
			m_LinearWrapSampler.Destroy();
			m_LinearMirrorSampler.Destroy();
			m_AnisoClampSampler.Destroy();
			m_AnisoWrapSampler.Destroy();
			m_AnisoMirrorSampler.Destroy();
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::Disable()
		{
			IG_D3D_ASSERT(false, L"Unsupported shader type.");
		}

		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetShader(ShaderType *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT numClassInstances)
		{
			IG_D3D_ASSERT(false, L"Unsupported shader type.");
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			IG_D3D_ASSERT(false, L"Unsupported shader type.");
		}

		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetSampler(UINT slot, ID3D11SamplerState* pSamplers)
		{
			SetSamplers(slot, 1, &pSamplers);
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetPointClampSampler(UINT slot)
		{
			if (!m_PointClampSampler.IsValid() || m_PointClampSampler.DevicePtr() != m_pDevice.Get())
			{
				m_PointClampSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP,
					D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP);
				D3D_SET_OBJECT_NAME_A(m_PointClampSampler.Ptr(), "::PointClampSampler");
			}
			SetSampler(slot, m_PointClampSampler.Ptr());
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetPointWrapSampler(UINT slot)
		{
			if (!m_PointWrapSampler.IsValid() || m_PointWrapSampler.DevicePtr() != m_pDevice.Get())
			{
				m_PointWrapSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP,
					D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
				D3D_SET_OBJECT_NAME_A(m_PointWrapSampler.Ptr(), "::PointWrapSampler");
			}
			SetSampler(slot, m_PointWrapSampler.Ptr());
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetPointMirrorSampler(UINT slot)
		{
			if (!m_PointMirrorSampler.IsValid() || m_PointMirrorSampler.DevicePtr() != m_pDevice.Get())
			{
				m_PointMirrorSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_MIRROR,
					D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR);
				D3D_SET_OBJECT_NAME_A(m_PointMirrorSampler.Ptr(), "::PointMirrorSampler");
			}
			SetSampler(slot, m_PointMirrorSampler.Ptr());
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetLinearClampSampler(UINT slot)
		{
			if (!m_LinearClampSampler.IsValid() || m_LinearClampSampler.DevicePtr() != m_pDevice.Get())
			{
				m_LinearClampSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP,
					D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP);
				D3D_SET_OBJECT_NAME_A(m_LinearClampSampler.Ptr(), "::LinearClampSampler");
			}
			SetSampler(slot, m_LinearClampSampler.Ptr());
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetLinearWrapSampler(UINT slot)
		{
			if (!m_LinearWrapSampler.IsValid() || m_LinearWrapSampler.DevicePtr() != m_pDevice.Get())
			{
				m_LinearWrapSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP,
					D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
				D3D_SET_OBJECT_NAME_A(m_LinearWrapSampler.Ptr(), "::LinearWrapSampler");
			}
			SetSampler(slot, m_LinearWrapSampler.Ptr());
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetLinearMirrorSampler(UINT slot)
		{
			if (!m_LinearMirrorSampler.IsValid() || m_LinearMirrorSampler.DevicePtr() != m_pDevice.Get())
			{
				m_LinearMirrorSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MIRROR,
					D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR);
				D3D_SET_OBJECT_NAME_A(m_LinearMirrorSampler.Ptr(), "::LinearMirrorSampler");
			}
			SetSampler(slot, m_LinearMirrorSampler.Ptr());
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetAnisoClampSampler(UINT slot)
		{
			if (!m_AnisoClampSampler.IsValid() || m_AnisoClampSampler.DevicePtr() != m_pDevice.Get())
			{
				m_AnisoClampSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP,
					D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, 0, 16);
				D3D_SET_OBJECT_NAME_A(m_AnisoClampSampler.Ptr(), "::AnisoClampSampler");
			}
			SetSampler(slot, m_AnisoClampSampler.Ptr());
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetAnisoWrapSampler(UINT slot)
		{
			if (!m_AnisoWrapSampler.IsValid() || m_AnisoWrapSampler.DevicePtr() != m_pDevice.Get())
			{
				m_AnisoWrapSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP,
					D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, 0, 16);
				D3D_SET_OBJECT_NAME_A(m_AnisoWrapSampler.Ptr(), "::AnisoWrapSampler");
			}
			SetSampler(slot, m_AnisoWrapSampler.Ptr());
		}

		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetAnisoMirrorSampler(UINT slot)
		{
			if (!m_AnisoMirrorSampler.IsValid() || m_AnisoMirrorSampler.DevicePtr() != m_pDevice.Get())
			{
				m_AnisoMirrorSampler.Create(m_pDevice.Get(),
					D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_MIRROR,
					D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR, 0, 16);
				D3D_SET_OBJECT_NAME_A(m_AnisoMirrorSampler.Ptr(), "::AnisoMirrorSampler");
			}
			SetSampler(slot, m_AnisoMirrorSampler.Ptr());
		}

		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers);
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetConstantBuffer(UINT slot, ID3D11Buffer *pConstantBuffer)
		{
			SetConstantBuffers(slot, 1, &pConstantBuffer);
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews);
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetShaderResource(UINT slot, ID3D11ShaderResourceView *pShaderResourceView)
		{
			SetShaderResources(slot, 1, &pShaderResourceView);
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetUnorderedAccessViews(UINT startSlot, UINT numUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts)
		{
			UNREFERENCED_PARAMETER(startSlot);
			UNREFERENCED_PARAMETER(numUAVs);
			UNREFERENCED_PARAMETER(ppUnorderedAccessViews);
			UNREFERENCED_PARAMETER(pUAVInitialCounts);

			IG_D3D_ASSERT(false, L"Such stage does not support UAV.")
		}
		template<class ShaderType>
		inline void CDeviceContext::ShaderStage<ShaderType>::SetUnorderedAccessView(UINT slot, ID3D11UnorderedAccessView *pUnorderedAccessView)
		{
			SetUnorderedAccessViews(slot, 1, &pUnorderedAccessView);
		}

		inline ID3D11Device*			CDeviceContext::DevicePtr()	const { return m_pDevice.Get(); }
		inline ID3D11DeviceContext*	CDeviceContext::Ptr()	const { return m_pDeviceContext.Get(); }
		inline bool CDeviceContext::IsValid() const { return m_pDeviceContext != nullptr; }

		// Convert to ComPtr
		inline CDeviceContext::operator ID3D11DeviceContext*() const { return m_pDeviceContext.Get(); }

		// Pipeline operations
		inline void CDeviceContext::ClearRenderTarget(ID3D11RenderTargetView *pRTV, const FLOAT color[4])
		{
			m_pDeviceContext->ClearRenderTargetView(pRTV, color);
		}
		inline void CDeviceContext::ClearRenderTarget(ID3D11RenderTargetView *pRTV, float R, float G, float B, float A)
		{
			float color[4] = {R, G, B, A};
			m_pDeviceContext->ClearRenderTargetView(pRTV, color);
		}

		inline void CDeviceContext::ClearDepthStencil(ID3D11DepthStencilView *pDSV, UINT clearFlags, FLOAT depth, UINT8 stencil)
		{
			m_pDeviceContext->ClearDepthStencilView(pDSV, clearFlags, depth, stencil);
		}

		inline void CDeviceContext::ClearUnorderedAccessViewFloat(ID3D11UnorderedAccessView *pUAV, const FLOAT vals[4])
		{
			m_pDeviceContext->ClearUnorderedAccessViewFloat(pUAV, vals);
		}
		inline void CDeviceContext::ClearUnorderedAccessViewUint(ID3D11UnorderedAccessView *pUAV, const UINT vals[4])
		{
			m_pDeviceContext->ClearUnorderedAccessViewUint(pUAV, vals);
		}
		inline void CDeviceContext::ClearUnorderedAccessViewFloat(ID3D11UnorderedAccessView *pUAV, float X, float Y, float Z, float W)
		{
			float vals[4] = { X, Y, Z, W };
			m_pDeviceContext->ClearUnorderedAccessViewFloat(pUAV, vals);
		}
		inline void CDeviceContext::ClearUnorderedAccessViewUint(ID3D11UnorderedAccessView *pUAV, UINT X, UINT Y, UINT Z, UINT W)
		{
			UINT vals[4] = { X, Y, Z, W };
			m_pDeviceContext->ClearUnorderedAccessViewUint(pUAV, vals);
		}
		inline void CDeviceContext::Draw(UINT vtxCount, UINT startVtxLocation)
		{
			m_pDeviceContext->Draw(vtxCount, startVtxLocation);
		}
		inline void CDeviceContext::DrawIndexed(UINT idxCount, UINT startIdxLocation, UINT baseVtxLocation)
		{
			m_pDeviceContext->DrawIndexed(idxCount, startIdxLocation, baseVtxLocation);
		}
		inline void CDeviceContext::Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ)
		{
			m_pDeviceContext->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
		}

		inline void CDeviceContext::CopyTexture1DSubresourceRegion(
			ID3D11Resource *pDstResource, UINT dstSubres, UINT dstX, 
			ID3D11Resource *pSrcResource, UINT srcSubres, UINT srcX, 
			UINT w)
		{
			D3D11_BOX box;
			box.left = srcX;
			box.right = srcX + w;
			box.top = 0;
			box.bottom = 1;
			box.front = 0;
			box.back = 1;
			m_pDeviceContext->CopySubresourceRegion(pDstResource, dstSubres, dstX, 0, 0, pSrcResource, srcSubres, &box);
		}

		inline void CDeviceContext::CopyTexture2DSubresourceRegion(
			ID3D11Resource *pDstResource, UINT dstSubres, UINT dstX, UINT dstY,
			ID3D11Resource *pSrcResource, UINT srcSubres, UINT srcX, UINT srcY,
			UINT w, UINT h)
		{
			D3D11_BOX box;
			box.left = srcX;
			box.right = srcX + w;
			box.top = srcY;
			box.bottom = srcY + h;
			box.front = 0;
			box.back = 1;
			m_pDeviceContext->CopySubresourceRegion(pDstResource, dstSubres, dstX, dstY, 0, pSrcResource, srcSubres, &box);
		}

		inline void CDeviceContext::CopyTexture3DSubresourceRegion(
			ID3D11Resource *pDstResource, UINT dstSubres, UINT dstX, UINT dstY, UINT dstZ,
			ID3D11Resource *pSrcResource, UINT srcSubres, UINT srcX, UINT srcY, UINT srcZ,
			UINT w, UINT h, UINT d)
		{
			D3D11_BOX box;
			box.left = srcX;
			box.right = srcX + w;
			box.top = srcY;
			box.bottom = srcY + h;
			box.front = srcZ;
			box.back = srcZ + d;
			m_pDeviceContext->CopySubresourceRegion(pDstResource, dstSubres, dstX, dstY, dstZ, pSrcResource, srcSubres, &box);
		}

		inline void CDeviceContext::CopyResource(ID3D11Resource *pDstResource, ID3D11Resource *pSrcResource)
		{
			m_pDeviceContext->CopyResource(pDstResource, pSrcResource);
		}

		inline void CDeviceContext::Begin(ID3D11Asynchronous *pAsync)
		{
			m_pDeviceContext->Begin(pAsync);
		}

		inline void CDeviceContext::End(ID3D11Asynchronous *pAsync)
		{
			m_pDeviceContext->End(pAsync);
		}

		inline HRESULT CDeviceContext::GetData(ID3D11Asynchronous *pAsync, void *pData, UINT dataSize, UINT flags)
		{
			return m_pDeviceContext->GetData(pAsync, pData, dataSize, flags);
		}

		template<>
		inline void CDeviceContext::ShaderStage<ID3D11VertexShader>::Disable()
		{
			m_pDC->VSSetShader(nullptr, nullptr, 0);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11HullShader>::Disable()
		{
			m_pDC->HSSetShader(nullptr, nullptr, 0);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11DomainShader>::Disable()
		{
			m_pDC->DSSetShader(nullptr, nullptr, 0);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11GeometryShader>::Disable()
		{
			m_pDC->GSSetShader(nullptr, nullptr, 0);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11PixelShader>::Disable()
		{
			m_pDC->PSSetShader(nullptr, nullptr, 0);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11ComputeShader>::Disable()
		{
			m_pDC->CSSetShader(nullptr, nullptr, 0);
		}

		template<>
		inline void CDeviceContext::ShaderStage<ID3D11VertexShader>::SetShader(ID3D11VertexShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT numClassInstances)
		{
			m_pDC->VSSetShader(pShader, ppClassInstances, numClassInstances);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11HullShader>::SetShader(ID3D11HullShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT numClassInstances)
		{
			m_pDC->HSSetShader(pShader, ppClassInstances, numClassInstances);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11DomainShader>::SetShader(ID3D11DomainShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT numClassInstances)
		{
			m_pDC->DSSetShader(pShader, ppClassInstances, numClassInstances);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11GeometryShader>::SetShader(ID3D11GeometryShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT numClassInstances)
		{
			m_pDC->GSSetShader(pShader, ppClassInstances, numClassInstances);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11PixelShader>::SetShader(ID3D11PixelShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT numClassInstances)
		{
			m_pDC->PSSetShader(pShader, ppClassInstances, numClassInstances);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11ComputeShader>::SetShader(ID3D11ComputeShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT numClassInstances)
		{
			m_pDC->CSSetShader(pShader, ppClassInstances, numClassInstances);
		}

		template<>
		inline void CDeviceContext::ShaderStage<ID3D11VertexShader>::SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_pDC->VSSetSamplers(startSlot, numSamplers, ppSamplers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11HullShader>::SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_pDC->HSSetSamplers(startSlot, numSamplers, ppSamplers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11DomainShader>::SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_pDC->DSSetSamplers(startSlot, numSamplers, ppSamplers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11GeometryShader>::SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_pDC->GSSetSamplers(startSlot, numSamplers, ppSamplers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11PixelShader>::SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_pDC->PSSetSamplers(startSlot, numSamplers, ppSamplers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11ComputeShader>::SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers)
		{
			m_pDC->CSSetSamplers(startSlot, numSamplers, ppSamplers);
		}

		template<>
		inline void CDeviceContext::ShaderStage<ID3D11VertexShader>::SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_pDC->VSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11HullShader>::SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_pDC->HSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11DomainShader>::SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_pDC->DSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11GeometryShader>::SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_pDC->GSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11PixelShader>::SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_pDC->PSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11ComputeShader>::SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers)
		{
			m_pDC->CSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
		}

		template<>
		inline void CDeviceContext::ShaderStage<ID3D11VertexShader>::SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_pDC->VSSetShaderResources(startSlot, numSRVs, ppShaderResourceViews);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11HullShader>::SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_pDC->HSSetShaderResources(startSlot, numSRVs, ppShaderResourceViews);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11DomainShader>::SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_pDC->DSSetShaderResources(startSlot, numSRVs, ppShaderResourceViews);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11GeometryShader>::SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_pDC->GSSetShaderResources(startSlot, numSRVs, ppShaderResourceViews);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11PixelShader>::SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_pDC->PSSetShaderResources(startSlot, numSRVs, ppShaderResourceViews);
		}
		template<>
		inline void CDeviceContext::ShaderStage<ID3D11ComputeShader>::SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews)
		{
			m_pDC->CSSetShaderResources(startSlot, numSRVs, ppShaderResourceViews);
		}

		template<>
		inline void CDeviceContext::ShaderStage<ID3D11ComputeShader>::SetUnorderedAccessViews(UINT startSlot, UINT numUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts)
		{
			m_pDC->CSSetUnorderedAccessViews(startSlot, numUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
		}
	}

}
