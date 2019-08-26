#ifdef NTDDI_WIN10_RS5
#include "igD3D12StateObject.h"

using namespace ig::D3D12;

void CStateObject::DoInitialization(CDevice::sPtr spDevice, ID3D12StateObject *pStateObject)
{
	m_spDevice = spDevice;
	m_pStateObject = pStateObject;
	StateObjectPropertiesComPtr pSOProp;
	IG_D3D_ASSERT_SUCCEEDED(m_pStateObject->QueryInterface(IID_PPV_ARGS(&pSOProp)));
	m_pStateObjectProperties = pSOProp;
}

// Create from existing ptr.
CStateObject::CStateObject(CDevice::sPtr spDevice, ID3D12StateObject *pStateObject)
{
	DoInitialization(spDevice, pStateObject);
}

CStateObject::CStateObject(CDevice::sPtr spDevice, const D3D12_STATE_OBJECT_DESC *pDesc)
{
	StateObjectComPtr pSO;
	Device5ComPtr pDevice5;
	IG_D3D_ASSERT_SUCCEEDED(spDevice->RawPtr()->QueryInterface(IID_PPV_ARGS(&pDevice5)));
	IG_D3D_ASSERT_SUCCEEDED(pDevice5->CreateStateObject(pDesc, IID_PPV_ARGS(&pSO)));
	DoInitialization(spDevice, pSO.Get());
}

#endif

