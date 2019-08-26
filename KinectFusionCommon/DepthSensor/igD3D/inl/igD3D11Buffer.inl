#pragma once


namespace ig
{
	namespace D3D11
	{
		// Getters
		inline ID3D11Buffer*					CBuffer::Ptr() const { return m_pBuf.Get(); }
		inline ID3D11ShaderResourceView*		CBuffer::SRV() const { return m_pSRV.Get(); }
		inline ID3D11UnorderedAccessView*		CBuffer::UAV() const { return m_pUAV.Get(); }
		inline ID3D11ShaderResourceView*		CBuffer::RawSRV() const { return m_pRawSRV.Get(); }
		inline ID3D11UnorderedAccessView*		CBuffer::RawUAV() const { return m_pRawUAV.Get(); }
		inline const D3D11_BUFFER_DESC&		CBuffer::Desc() const { return m_Desc; };

		// Convert to ComPtr
		inline CBuffer::operator ID3D11Buffer*() const { return m_pBuf.Get(); }

	}

}
