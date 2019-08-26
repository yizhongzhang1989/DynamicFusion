#pragma once

namespace ig
{
	namespace D3D11
	{
		inline ID3D11Resource* CResource::ResourcePtr() const { return m_pRes.Get(); }
		inline ID3D11Device* CResource::DevicePtr() const { return m_pDevice.Get(); }
		inline bool CResource::IsValid() const { return m_pRes != nullptr; }

		// Static methods
		inline UINT CResource::CalMipLevels(UINT size)
		{
			UINT ret = 1;
			while (size > 1)
			{
				size = size >> 1;
				++ret;
			}
			return ret;
		}

	}
}