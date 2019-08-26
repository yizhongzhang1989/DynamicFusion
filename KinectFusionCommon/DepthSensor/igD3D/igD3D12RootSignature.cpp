#include "igD3D12RootSignature.h"

using namespace ig::D3D12;

void CRootSignature::DoInitialization(CDevice::sPtr spDevice, ID3D12RootSignature *pSignature)
{
	m_spDevice = spDevice;
	m_pRootSignature = pSignature;
}

CRootSignature::CRootSignature(CDevice::sPtr spDevice, ID3D12RootSignature *pSignature)
{
	DoInitialization(spDevice, pSignature);
}

CRootSignature::CRootSignature(CDevice::sPtr spDevice, const D3D12_ROOT_SIGNATURE_DESC *pDesc, UINT nodeMask)
{
	RootSignatureComPtr pSignature;
	BlobComPtr pOutBlob, pErrorBlob;

	IG_D3D_ASSERT_SUCCEEDED(D3D12SerializeRootSignature(pDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf()));

	IG_D3D_ASSERT_SUCCEEDED(spDevice->RawPtr()->CreateRootSignature(nodeMask, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(),
		IID_PPV_ARGS(&pSignature)));

	DoInitialization(spDevice, pSignature.Get());
}



