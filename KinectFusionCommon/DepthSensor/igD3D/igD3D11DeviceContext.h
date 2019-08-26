#pragma once

// Usage: simple wrapper for DeviceContext
// Created: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// 2018.6.2: Added SetXXXXMirrorSampler.
// 2019.4.9: Set names for common samplers and states.
// Version: 1.0.190409.2000

#include "igD3D11Defs.h"

#include "igD3D11BlendState.h"
#include "igD3D11DepthStencilState.h"
#include "igD3D11SamplerState.h"
#include "igD3D11RasterizerState.h"

namespace ig
{
    namespace D3D11
    {

        class CDeviceContext
        {
            //friend class CDevice;
        private:
            class InputAssemblerStage
            {
            public:
                void Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC);
                
                void Destroy();
                
                void SetInputLayout(ID3D11InputLayout *pIA);
                
                void SetVertexBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer *const *ppVBs, const UINT *pStrides, const UINT *pOffsets);
                
                void SetVertexBuffer(UINT slot, ID3D11Buffer *pVB, UINT stride, UINT offset = 0);
                
                void SetIndexBuffer(ID3D11Buffer *pIB, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT, UINT offset = 0);
                
                void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
                
            private:
                DeviceComPtr m_pDevice;
                DeviceContextComPtr m_pDC;
            };
            class RasterizerStage
            {
            public:
                void Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC);
                
                void Destroy();
                
                void SetRasterizer(ID3D11RasterizerState *pRSState);
                
                void SetRasterizerDefault();
                
                // CCW as front face
                void SetRasterizerCCW();

                void SetRasterizerTwoSided();
                
                void SetViewports(UINT numVPs, const D3D11_VIEWPORT *pVPs);
                
                void SetViewport(D3D11_VIEWPORT vp);
                
                void SetViewport(FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height,
                    FLOAT minDepth = D3D11_MIN_DEPTH, FLOAT maxDepth = D3D11_MAX_DEPTH);
                
                void SetScissorRects(UINT numRects, const D3D11_RECT *pRects);
                

            private:
                DeviceComPtr m_pDevice;
                DeviceContextComPtr m_pDC;
                CRasterizerState m_CCWRSState;
                CRasterizerState m_TwoSidedRSState;
            };
            class OutputMergerStage 
            {
            public:
                void Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC);
                
                void Destroy();
                
                void SetDepthStencil(ID3D11DepthStencilState *pDSState, UINT stencilRef = 0);
                
                void SetDepthStencilDefault();
                
                void SetDepthStencilReadOnly();
                
                void SetDepthStencilGreaterEqual();

				void SetDepthStencilLessEqual();
                
                void DisableDepthStencil();
                
                void SetBlend(ID3D11BlendState *pBlendState, const FLOAT *blendFactor = nullptr, UINT sampleMask = 0xffffffff);
                
                void SetBlendAdditive();
                
                void DisableBlend();
                
                void SetRenderTargets(UINT numViews, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView);
                
                void SetRenderTarget(ID3D11RenderTargetView*pRenderTargetView = nullptr, ID3D11DepthStencilView *pDepthStencilView = nullptr);
                

            private:
                DeviceComPtr m_pDevice;
                DeviceContextComPtr m_pDC;
                CDepthStencilState m_DisableDSState;
                CDepthStencilState m_ReadOnlyDSState;
                CDepthStencilState m_GreaterEqualDSState;
				CDepthStencilState m_LessEqualDSState;
                CBlendState m_AdditiveBlendState;
            };
            template<class ShaderType>
            class ShaderStage
            {
            public:
                void Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC);

                void Destroy();
                
                void Disable();
                
                void SetShader(ShaderType *pShader, ID3D11ClassInstance *const *ppClassInstances = nullptr, UINT numClassInstances = 0);
                
                void SetSamplers(UINT startSlot, UINT numSamplers, ID3D11SamplerState *const *ppSamplers);
                
                void SetSampler(UINT slot, ID3D11SamplerState* pSamplers);
                
                void SetPointClampSampler(UINT slot);
                
                void SetPointWrapSampler(UINT slot);

				void SetPointMirrorSampler(UINT slot);
                
                void SetLinearClampSampler(UINT slot);
                
                void SetLinearWrapSampler(UINT slot);

				void SetLinearMirrorSampler(UINT slot);
                
                void SetAnisoClampSampler(UINT slot);
                
                void SetAnisoWrapSampler(UINT slot);
                
				void SetAnisoMirrorSampler(UINT slot);

                void SetConstantBuffers(UINT startSlot, UINT numBuffers, ID3D11Buffer **ppConstantBuffers);
                
                void SetConstantBuffer(UINT slot, ID3D11Buffer *pConstantBuffer);
                
                void SetShaderResources(UINT startSlot, UINT numSRVs, ID3D11ShaderResourceView *const *ppShaderResourceViews);
                
                void SetShaderResource(UINT slot, ID3D11ShaderResourceView *pShaderResourceView);
                
                void SetUnorderedAccessViews(UINT startSlot, UINT numUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts = nullptr);
                
                void SetUnorderedAccessView(UINT slot, ID3D11UnorderedAccessView *pUnorderedAccessView);
                
            private:
                DeviceComPtr m_pDevice;
                DeviceContextComPtr m_pDC;
                CSamplerState m_PointClampSampler;
                CSamplerState m_PointWrapSampler;
				CSamplerState m_PointMirrorSampler;
                CSamplerState m_LinearClampSampler;
                CSamplerState m_LinearWrapSampler;
				CSamplerState m_LinearMirrorSampler;
                CSamplerState m_AnisoClampSampler;
                CSamplerState m_AnisoWrapSampler;
				CSamplerState m_AnisoMirrorSampler;
            };

        public:
            CDeviceContext() = default;
            ~CDeviceContext() = default;
			CDeviceContext(CDeviceContext&&) = default;
			CDeviceContext& operator=(CDeviceContext&&) = default;
			CDeviceContext(const CDeviceContext&) = delete;
			CDeviceContext& operator=(const CDeviceContext&) = delete;
            
            // Create from existing Context ComPtr.
            void Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC);
            
            // Clear.
            void Destroy();


            // Getters
            ID3D11Device*			DevicePtr()	const;
            ID3D11DeviceContext*	Ptr()	const;
            bool					IsValid() const;

            // Convert to ComPtr
            operator ID3D11DeviceContext*() const;

            // Pipeline Stages
            InputAssemblerStage					IA;
            RasterizerStage						RS;
            OutputMergerStage					OM;
            ShaderStage<ID3D11VertexShader>		VS;
            ShaderStage<ID3D11HullShader>		HS;
            ShaderStage<ID3D11DomainShader>		DS;
            ShaderStage<ID3D11GeometryShader>	GS;
            ShaderStage<ID3D11PixelShader>		PS;
            ShaderStage<ID3D11ComputeShader>	CS;

            // Operations
            void ClearRenderTarget(ID3D11RenderTargetView *pRTV, const FLOAT color[4]);
            void ClearRenderTarget(ID3D11RenderTargetView *pRTV, float R = 0, float G = 0, float B = 0, float A = 0);
            void ClearDepthStencil(ID3D11DepthStencilView *pDSV, UINT clearFlags = D3D11_CLEAR_DEPTH, FLOAT depth = 1.f, UINT8 stencil = 0);

            void ClearUnorderedAccessViewFloat(ID3D11UnorderedAccessView *pUAV, const FLOAT vals[4]);
            void ClearUnorderedAccessViewUint(ID3D11UnorderedAccessView *pUAV, const UINT vals[4]);
            void ClearUnorderedAccessViewFloat(ID3D11UnorderedAccessView *pUAV, float X = 0, float Y = 0, float Z = 0, float W = 0);
            void ClearUnorderedAccessViewUint(ID3D11UnorderedAccessView *pUAV, UINT X = 0, UINT Y = 0, UINT Z = 0, UINT W = 0);

            void Draw(UINT vtxCount, UINT startVtxLocation = 0);
            
            void DrawIndexed(UINT idxCount, UINT startIdxLocation = 0, UINT baseVtxLocation = 0);
            
            void Dispatch(UINT threadGroupCountX, UINT threadGroupCountY = 1, UINT threadGroupCountZ = 1);

            void CopyTexture1DSubresourceRegion(
                ID3D11Resource *pDstResource, UINT dstSubres, UINT dstX, 
                ID3D11Resource *pSrcResource, UINT srcSubres, UINT srcX, 
                UINT w);
            void CopyTexture2DSubresourceRegion(
                ID3D11Resource *pDstResource, UINT dstSubres, UINT dstX, UINT dstY,
                ID3D11Resource *pSrcResource, UINT srcSubres, UINT srcX, UINT srcY, 
                UINT w, UINT h);
            void CopyTexture3DSubresourceRegion(
                ID3D11Resource *pDstResource, UINT dstSubres, UINT dstX, UINT dstY, UINT dstZ,
                ID3D11Resource *pSrcResource, UINT srcSubres, UINT srcX, UINT srcY, UINT srcZ,
                UINT w, UINT h, UINT d);
            
            void CopyResource(ID3D11Resource *pDstResource, ID3D11Resource *pSrcResource);
            
            void Begin(ID3D11Asynchronous *pAsync);
            
            void End(ID3D11Asynchronous *pAsync);
            
            HRESULT GetData(ID3D11Asynchronous *pAsync, void *pData, UINT dataSize, UINT flags);

        private:
            DeviceComPtr		m_pDevice;
            DeviceContextComPtr m_pDeviceContext;
        };

    }

}

#include "inl\igD3D11DeviceContext.inl"
