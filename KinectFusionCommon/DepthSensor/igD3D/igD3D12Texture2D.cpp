#include "igD3D12Texture2D.h"
#include "igD3D12Buffer.h"

using namespace ig::D3D12;

void CTexture2D::UploadSync(
	UINT arrayIdx, UINT level,
	const void* pSrcData, UINT srcRowPitch,
	D3D12_RESOURCE_STATES afterState)
{
	IG_D3D_ASSERT(arrayIdx < m_Desc.DepthOrArraySize &&
		level < m_Desc.MipLevels && pSrcData != nullptr);

	UINT subresId = arrayIdx * m_Desc.MipLevels + level;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts[1];
	UINT nRows;
	UINT64 nRowSize, nTotSize;
	m_spDevice->RawPtr()->GetCopyableFootprints(&m_Desc, subresId, 1, 0, layouts, &nRows, &nRowSize, &nTotSize);

	CBuffer::uPtr upUploadBuf = CBuffer::CreateAsUpload(m_spDevice, nTotSize);

	// Copy to upload buffer.
	uint8_t *pData = nullptr;
	IG_D3D_ASSERT_SUCCEEDED(upUploadBuf->Map(0, 0, (void**)&pData));
	for (UINT y = 0; y < nRows; ++y)
	{
		memcpy_s(pData + layouts->Footprint.RowPitch * y, nRowSize,
			(const uint8_t*)pSrcData + srcRowPitch * y, nRowSize);
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
	pContext->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_DEST);
	pContext->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
	pContext->TransitionResource(this, afterState);

	m_spDevice->GraphicsQueue()->Execute(pContext);
	m_spDevice->GraphicsQueue()->WaitForIdle();
}

void CTexture2D::ReadbackSync(
	UINT arrayIdx, UINT level,
	std::vector<uint8_t> &dstData,
	UINT &width, UINT &height,
	UINT &rowPitch,
	D3D12_RESOURCE_STATES afterState)
{
	IG_D3D_ASSERT(arrayIdx < m_Desc.DepthOrArraySize &&
		level < m_Desc.MipLevels);

	UINT subresId = arrayIdx * m_Desc.MipLevels + level;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts[1];
	UINT nRows;
	UINT64 nRowSize, nTotSize;
	m_spDevice->RawPtr()->GetCopyableFootprints(&m_Desc, subresId, 1, 0, layouts, &nRows, &nRowSize, &nTotSize);

	CBuffer::uPtr upReadbackBuf = CBuffer::CreateAsReadback(m_spDevice, nTotSize);

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

	//auto curState = m_ResourceState;
	pContext->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_SOURCE);
	pContext->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
	pContext->TransitionResource(this, afterState);

	m_spDevice->GraphicsQueue()->Execute(pContext);

	m_spDevice->GraphicsQueue()->WaitForIdle();

	if (dstData.size() < nTotSize)
	{
		dstData.resize(nTotSize);
	}
	// Copy from read-back buffer.
	D3D12_RANGE range;
	range.Begin = 0;
	range.End = nTotSize;
	uint8_t *pData = nullptr;
	upReadbackBuf->ReadbackSync(0, nTotSize, dstData, afterState);
	width = layouts[0].Footprint.Width;
	height = layouts[0].Footprint.Height;
	rowPitch = layouts[0].Footprint.RowPitch;
}