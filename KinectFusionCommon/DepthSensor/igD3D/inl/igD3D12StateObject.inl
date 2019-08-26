#pragma once

namespace ig
{
	namespace D3D12
	{
		inline CStateObject::uPtr CStateObject::Create(CDevice::sPtr spDevice, ID3D12StateObject *pStateObject)
		{
			return std::make_unique<CStateObject>(spDevice, pStateObject);
		}
		inline CStateObject::uPtr CStateObject::Create(CDevice::sPtr spDevice, const D3D12_STATE_OBJECT_DESC *pDesc)
		{
			return std::make_unique<CStateObject>(spDevice, pDesc);
		}
		

		// Getters
		inline ID3D12StateObject* CStateObject::RawPtr() const
		{
			return m_pStateObject.Get();
		}
		
		inline ID3D12StateObjectProperties* CStateObject::PropertiesRawPtr() const
		{
			return m_pStateObjectProperties.Get();
		}

		inline CDevice* CStateObject::DevicePtr() const
		{
			return m_spDevice.get();
		}

		inline void* CStateObject::ShaderIdentifier(LPCWSTR pExportName)
		{
			return m_pStateObjectProperties->GetShaderIdentifier(pExportName);
		}

		inline HRESULT CStateObject::SetName(LPCWSTR strName)
		{
			return m_pStateObject->SetName(strName);
		}
	}
}





