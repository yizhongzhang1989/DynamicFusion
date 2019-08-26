#ifdef NTDDI_WIN10_RS5
#include "igD3D12ShaderTable.h"
using namespace ig::D3D12;

inline UINT ALIGN(UINT size, UINT alignment)
{
	return (size + (alignment - 1)) & ~(alignment - 1);
}

CShaderTable::CShaderTable(CDevice::sPtr spDevice, UINT nShaderRecords, UINT nShaderRecordSize, UINT nShaderRecordAlignment)
{
	m_nShaderRecords = nShaderRecords;
	UINT alignment = std::max<UINT>(nShaderRecordAlignment, D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT);
	m_nShaderRecordSize = ALIGN(nShaderRecordSize, alignment);
	m_upBuffer = CBuffer::CreateAsUpload(spDevice, m_nShaderRecords * m_nShaderRecordSize);
}


CShaderTable::uPtr CShaderTable::Create(CDevice::sPtr spDevice, UINT nShaderRecords, UINT nShaderRecordSize, UINT nShaderRecordAlignment)
{
	return std::make_unique<CShaderTable>(spDevice, nShaderRecords, nShaderRecordSize, nShaderRecordAlignment);
}

void CShaderTable::SetShaderRecord(
	UINT recordIdx,
	const void *pShaderIdentifier,
	const void *pLocalRootArguments,
	UINT localRootArgumentsSize)
{
	IG_D3D_ASSERT(recordIdx < m_nShaderRecords);
	m_upBuffer->UploadSync(recordIdx * m_nShaderRecordSize, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES, pShaderIdentifier);
	if (pLocalRootArguments && localRootArgumentsSize > 0)
	{
		IG_D3D_ASSERT(D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + localRootArgumentsSize <= m_nShaderRecordSize);
		m_upBuffer->UploadSync(recordIdx * m_nShaderRecordSize + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES, localRootArgumentsSize, pLocalRootArguments);
	}
}
#endif