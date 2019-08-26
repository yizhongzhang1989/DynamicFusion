#include "inl\igD3D12MipMapGen.inl"
#include "Shaders\MipMapGen_CS.h"
#include "igD3D12Resource.h"

using namespace ig::D3D12;

MipMapGen::MipMapGen(ID3D12Device *pDevice, UINT nodeMask)
{
	// Root Signature
	RootSignatureDesc rsDesc;
	rsDesc.Reset(2, 1, D3D12_ROOT_SIGNATURE_FLAG_NONE);
	rsDesc.Parameter(0).AsConstants(0, 3);
	rsDesc.Parameter(1).AsDescriptorTable(2);
	rsDesc.Parameter(1).SetTableRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1); // SRV
	rsDesc.Parameter(1).SetTableRange(1, D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1); // UAV
	rsDesc.Sampler(0).AsLinearClamp(0); // Bilinear

	BlobComPtr pOutBlob, pErrorBlob;
	IG_D3D_ASSERT_SUCCEEDED(D3D12SerializeRootSignature(rsDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf()));
	IG_D3D_ASSERT_SUCCEEDED(pDevice->CreateRootSignature(nodeMask, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_pRootSignature)));
	// PSO
	ComputePipelineStateDesc psoDesc;
	psoDesc.SetRootSignature(m_pRootSignature.Get());
	psoDesc.SetComputeShader(g_CS_MipMap, sizeof(g_CS_MipMap));
	IG_D3D_ASSERT_SUCCEEDED(pDevice->CreateComputePipelineState(psoDesc, IID_PPV_ARGS(&m_pPSO)));
	
	m_pDevice = pDevice;
}

static DXGI_FORMAT ToUAVFormat(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		return DXGI_FORMAT_B8G8R8A8_UNORM;

	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		return DXGI_FORMAT_B8G8R8X8_UNORM;

	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_R32_FLOAT:
		return DXGI_FORMAT_R32_FLOAT;

	default:
		return format;
	}
}

void MipMapGen::Run(CComputeCommandContext *pComputeContext, CResource *pTex)
{
	if (pComputeContext == nullptr ||
		pTex == nullptr ||
		pTex->Desc().Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D ||
		!(pTex->Desc().Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS))
	{
		IG_D3D_ERROR_DUMP(L"MipMapGen::Run: Invalid inputs.");
		return;
	}
	if (pTex->Desc().MipLevels == 1)
		return;

	auto beforeState = pTex->CurrentState();
	DXGI_FORMAT format = pTex->Desc().Format;
	pComputeContext->SetPipelineState(m_pPSO.Get());
	// for each 
	UINT constants[3] = { (UINT)pTex->Desc().Width, (UINT)pTex->Desc().Height, ig::DXGI::IsSRGB(format) ? 0xFFFFFFFF : 0 };
	for (UINT m = 1; m < pTex->Desc().MipLevels; ++m)
	{
		constants[0] = std::max<UINT>(1, constants[0] >> 1);
		constants[1] = std::max<UINT>(1, constants[1] >> 1);

		pComputeContext->ComputeRoot.SetSignature(m_pRootSignature.Get());
		pComputeContext->ComputeRoot.Set32BitConstants(0, 3, constants, 0);
		pComputeContext->TransitionResource(pTex, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		for (UINT i = 0; i < pTex->Desc().DepthOrArraySize; ++i)
		{
			auto hSRV = pTex->RetriveSRV(SRVDesc().AsTex2DArray(format, m - 1, 1, i, 1));
			auto hUAV = pTex->RetriveUAV(UAVDesc().AsTex2DArray(ToUAVFormat(format), m, i, 1));
			pComputeContext->ComputeRoot.SetDynamicViewDescriptor(1, 0, hSRV);
			pComputeContext->ComputeRoot.SetDynamicViewDescriptor(1, 1, hUAV);

			UINT gx = (constants[0] + 15) / 16;
			UINT gy = (constants[1] + 15) / 16;
			pComputeContext->Dispatch(gx, gy, 1);
		}
	}
	pComputeContext->TransitionResource(pTex, beforeState);
}
