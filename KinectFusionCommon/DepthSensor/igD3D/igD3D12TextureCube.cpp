#include "igD3D12TextureCube.h"
#include "igD3D12Buffer.h"

using namespace ig::D3D12;


void CTextureCube::UploadSync(
	UINT mipLevel,
	const void* pSrcData[6], UINT srcRowPitch[6],
	D3D12_RESOURCE_STATES afterState)//,
{
	CBuffer::uPtr upUploadBuf;

	for (UINT f = 0; f < 6; ++f)
	{
		UINT subresId = f * m_Desc.MipLevels + mipLevel;
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts[1];
		UINT nRows;
		UINT64 nRowSize, nTotSize;
		m_spDevice->RawPtr()->GetCopyableFootprints(&m_Desc, subresId, 1, 0, layouts, &nRows, &nRowSize, &nTotSize);

		if (!upUploadBuf)
		{
			upUploadBuf = CBuffer::CreateAsUpload(m_spDevice, nTotSize);
		}
		// Copy to upload buffer.
		uint8_t *pData = nullptr;
		IG_D3D_ASSERT_SUCCEEDED(upUploadBuf->Map(0, 0, (void**)&pData));
		for (UINT y = 0; y < nRows; ++y)
		{
			memcpy_s(pData + layouts->Footprint.RowPitch * y, nRowSize,
				(const uint8_t*)pSrcData[f] + srcRowPitch[f] * y, nRowSize);
		}
		upUploadBuf->Unmap(0, 0);

		// Copy to GPU
		D3D12_TEXTURE_COPY_LOCATION dstLocation, srcLocation;
		dstLocation.pResource = RawPtr();
		dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLocation.SubresourceIndex = subresId;
		srcLocation.pResource = upUploadBuf->RawPtr();
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLocation.PlacedFootprint.Offset = 0;
		srcLocation.PlacedFootprint.Footprint = layouts->Footprint;

		auto pContext = m_spDevice->GraphicsQueue()->RequestGraphicsContext();

		//auto curState = m_ResourceState;
		if (f == 0)
		{
			pContext->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_DEST);
		}
		pContext->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
		if (f == 5)
		{
			pContext->TransitionResource(this, afterState);
		}
		m_spDevice->GraphicsQueue()->Execute(pContext);

		m_spDevice->GraphicsQueue()->WaitForIdle();
	}

}

void CTextureCube::ReadbackSync(
	UINT mipLevel,
	std::vector<uint8_t> &dstData,
	UINT &resolution, UINT &rowPitch,
	UINT &facePitch,
	D3D12_RESOURCE_STATES afterState)
{
	// Assume all faces have same layout.
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts[1];
	UINT nRows;
	UINT64 nRowSize, nTotSize;
	m_spDevice->RawPtr()->GetCopyableFootprints(&m_Desc, mipLevel, 1, 0, layouts, &nRows, &nRowSize, &nTotSize);

	CBuffer::uPtr upReadbackBuf = CBuffer::Create(m_spDevice, HeapProperties().AsReadback(),
		D3D12_HEAP_FLAG_NONE, ResourceDesc().AsBuffer(nTotSize), D3D12_RESOURCE_STATE_COPY_DEST);

	if (dstData.size() < nTotSize * 6)
	{
		dstData.resize(nTotSize * 6);
	}

	for (UINT f = 0; f < 6; ++f)
	{
		UINT subresId = f * m_Desc.MipLevels + mipLevel;
		// Copy from GPU
		D3D12_TEXTURE_COPY_LOCATION dstLocation, srcLocation;
		dstLocation.pResource = upReadbackBuf->RawPtr();
		dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		dstLocation.PlacedFootprint.Offset = 0;
		dstLocation.PlacedFootprint.Footprint = layouts->Footprint;

		srcLocation.pResource = RawPtr();
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		srcLocation.SubresourceIndex = subresId;

		auto pContext = m_spDevice->GraphicsQueue()->RequestGraphicsContext();

		if (f == 0)
		{
			pContext->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_SOURCE);
		}
		pContext->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
		if (f == 5)
		{
			pContext->TransitionResource(this, afterState);
		}
		m_spDevice->GraphicsQueue()->Execute(pContext);
		m_spDevice->GraphicsQueue()->WaitForIdle();

		// Copy from read-back buffer.
		D3D12_RANGE range;
		range.Begin = 0;
		range.End = nTotSize;
		uint8_t *pData = nullptr;
		IG_D3D_ASSERT_SUCCEEDED(upReadbackBuf->Map(0, &range, (void**)&pData));
		memcpy_s(dstData.data() + nTotSize * f, nTotSize, pData, nTotSize);
		upReadbackBuf->Unmap(0, nullptr);
	}

	resolution = layouts[0].Footprint.Width;
	rowPitch = layouts[0].Footprint.RowPitch;
	facePitch = (UINT)nTotSize;
}