#include "igD3D12Buffer.h"

using namespace ig::D3D12;

void CBuffer::UploadSync(
	UINT64 offsetInBytes, UINT64 sizeInBytes,
	const void* pSrcData,
	D3D12_RESOURCE_STATES afterState)
{
	IG_D3D_ASSERT(pSrcData != nullptr &&
		offsetInBytes + sizeInBytes <= m_Desc.Width);

	D3D12_HEAP_PROPERTIES heapProp;
	D3D12_HEAP_FLAGS heapFlag;
	IG_D3D_ASSERT_SUCCEEDED(m_pResource->GetHeapProperties(&heapProp, &heapFlag));
	IG_D3D_ASSERT(heapProp.Type != D3D12_HEAP_TYPE_READBACK);

	if (heapProp.Type == D3D12_HEAP_TYPE_UPLOAD)
	{
		D3D12_RANGE range;
		range.Begin = offsetInBytes;
		range.End = offsetInBytes + sizeInBytes;
		void *pData = nullptr;
		IG_D3D_ASSERT_SUCCEEDED(Map(0, &range, &pData));
		memcpy_s((uint8_t*)pData + offsetInBytes, sizeInBytes, pSrcData, sizeInBytes);
		Unmap(0, &range);
		return;
	}
	// for default resource
	CBuffer::uPtr upUploadBuf = CBuffer::CreateAsUpload(m_spDevice, sizeInBytes);
	upUploadBuf->UploadSync(0, sizeInBytes, pSrcData);

	// Copy to GPU
	auto pContext = m_spDevice->GraphicsQueue()->RequestGraphicsContext();

	pContext->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_DEST);
	pContext->CopyBufferRegion(this, offsetInBytes, upUploadBuf.get(), 0, sizeInBytes);
	pContext->TransitionResource(this, afterState);

	m_spDevice->GraphicsQueue()->Execute(pContext);

	m_spDevice->GraphicsQueue()->WaitForIdle();

}

void CBuffer::ReadbackSync(
	UINT64 offsetInBytes, UINT64 sizeInBytes,
	std::vector<uint8_t> &dstData,
	D3D12_RESOURCE_STATES afterState)
{
	D3D12_HEAP_PROPERTIES heapProp;
	D3D12_HEAP_FLAGS heapFlag;
	IG_D3D_ASSERT_SUCCEEDED(m_pResource->GetHeapProperties(&heapProp, &heapFlag));
	IG_D3D_ASSERT(heapProp.Type != D3D12_HEAP_TYPE_UPLOAD);

	if (dstData.size() < sizeInBytes)
	{
		dstData.resize(sizeInBytes);
	}

	if (heapProp.Type == D3D12_HEAP_TYPE_READBACK)
	{
		D3D12_RANGE range;
		range.Begin = offsetInBytes;
		range.End = offsetInBytes + sizeInBytes;
		void *pData = nullptr;
		IG_D3D_ASSERT_SUCCEEDED(Map(0, &range, &pData));
		memcpy_s(dstData.data(), sizeInBytes, (const uint8_t*)pData + offsetInBytes, sizeInBytes);
		Unmap(0, nullptr);
		return;
	}

	CBuffer::uPtr upReadbackBuf = CBuffer::CreateAsReadback(m_spDevice, sizeInBytes);

	// Copy from GPU
	auto pContext = m_spDevice->GraphicsQueue()->RequestGraphicsContext();
	pContext->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_SOURCE);
	pContext->CopyBufferRegion(upReadbackBuf.get(), 0, this, offsetInBytes, sizeInBytes);
	pContext->TransitionResource(this, afterState);
	m_spDevice->GraphicsQueue()->Execute(pContext);
	m_spDevice->GraphicsQueue()->WaitForIdle();

	upReadbackBuf->ReadbackSync(0, sizeInBytes, dstData, afterState);
}



