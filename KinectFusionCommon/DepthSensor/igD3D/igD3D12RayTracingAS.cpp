#ifdef NTDDI_WIN10_RS5
#include "igD3D12RayTracingAS.h"

using namespace ig::D3D12;

void RayTracingBLASInputs::AddGeometry(const D3D12_RAYTRACING_GEOMETRY_DESC *pDesc)
{
	m_Descs.push_back(*pDesc);
	m_Inputs.NumDescs = (UINT)m_Descs.size();
	m_Inputs.pGeometryDescs = m_Descs.data();
}

void RayTracingBLASInputs::AddTriangleTypeGeometry(
	D3D12_GPU_VIRTUAL_ADDRESS vbGPUAddress,
	UINT64 vbStrideInBytes,
	UINT nVertices,
	DXGI_FORMAT vertexFormat,
	D3D12_GPU_VIRTUAL_ADDRESS ibGPUAddress,
	UINT nIndices,
	DXGI_FORMAT indexFormat,
	D3D12_GPU_VIRTUAL_ADDRESS transform3x4,
	D3D12_RAYTRACING_GEOMETRY_FLAGS flags)
{
	D3D12_RAYTRACING_GEOMETRY_DESC desc;
	desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	desc.Flags = flags;
	desc.Triangles.VertexBuffer.StartAddress = vbGPUAddress;
	desc.Triangles.VertexBuffer.StrideInBytes = vbStrideInBytes;
	desc.Triangles.VertexCount = nVertices;
	desc.Triangles.VertexFormat = vertexFormat;
	desc.Triangles.IndexBuffer = ibGPUAddress;
	desc.Triangles.IndexCount = nIndices;
	desc.Triangles.IndexFormat = indexFormat;
	desc.Triangles.Transform3x4 = transform3x4;
	AddGeometry(&desc);
}

void RayTracingBLASInputs::AddAABBTypeGeometry(
	D3D12_GPU_VIRTUAL_ADDRESS aabbGPUAddress,
	UINT64 aabbStrideInBytes,
	UINT64 nAABBs,
	D3D12_RAYTRACING_GEOMETRY_FLAGS flags)
{
	D3D12_RAYTRACING_GEOMETRY_DESC desc;
	desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_PROCEDURAL_PRIMITIVE_AABBS;
	desc.Flags = flags;
	desc.AABBs.AABBs.StartAddress = aabbGPUAddress;
	desc.AABBs.AABBs.StrideInBytes = aabbStrideInBytes;
	desc.AABBs.AABBCount = nAABBs;
	AddGeometry(&desc);
}

void RayTracingBLASInputs::Reset(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags)
{
	ZeroMemory(&m_Inputs, sizeof(m_Inputs));
	m_Inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	m_Inputs.Flags = flags;
	m_Inputs.NumDescs = 0;
	m_Inputs.pGeometryDescs = nullptr;
	m_Inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	m_Descs.clear();
}

	
void RayTracingTLASInputs::AddInstance(
	D3D12_GPU_VIRTUAL_ADDRESS BLASAddress,
	UINT instanceId,
	DirectX::XMFLOAT3X4 *pTransform,
	UINT instanceMask,
	UINT instanceContributionToHitGroupIdx,
	UINT flags)
{
	DirectX::XMFLOAT3X4 identity = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };
	if (pTransform == nullptr)
	{
		pTransform = &identity;
	}
	D3D12_RAYTRACING_INSTANCE_DESC desc;
	desc.AccelerationStructure = BLASAddress;
	desc.InstanceID = instanceId;
	desc.InstanceMask = instanceMask;
	desc.InstanceContributionToHitGroupIndex = instanceContributionToHitGroupIdx;
	desc.Transform[0][0] = pTransform->m[0][0]; desc.Transform[0][1] = pTransform->m[0][1];
	desc.Transform[0][2] = pTransform->m[0][2]; desc.Transform[0][3] = pTransform->m[0][3];
	desc.Transform[1][0] = pTransform->m[1][0]; desc.Transform[1][1] = pTransform->m[1][1];
	desc.Transform[1][2] = pTransform->m[1][2]; desc.Transform[1][3] = pTransform->m[1][3];
	desc.Transform[2][0] = pTransform->m[2][0]; desc.Transform[2][1] = pTransform->m[2][1];
	desc.Transform[2][2] = pTransform->m[2][2]; desc.Transform[2][3] = pTransform->m[2][3];
	desc.Flags = flags;
	AddInstance(&desc);
}

void RayTracingTLASInputs::AddInstance(const D3D12_RAYTRACING_INSTANCE_DESC *pDesc)
{
	// Re-allocate when necessary.
	if (m_nCurIdx >= m_nDescBufSizeInNumDescs)
	{
		auto pDescBuf = ig::D3D12::CBuffer::CreateAsUpload(m_spDevice,
			(m_nDescBufSizeInNumDescs + m_nIncrementalSize) * sizeof(D3D12_RAYTRACING_INSTANCE_DESC));

		uint8_t *pSrcData = nullptr;
		uint8_t *pDstData = nullptr;
		IG_D3D_ASSERT_SUCCEEDED(pDescBuf->Map(0, nullptr, (void**)&pDstData));
		IG_D3D_ASSERT_SUCCEEDED(m_spInstanceDescBuf->Map(0, nullptr, (void**)&pSrcData));

		memcpy_s(pDstData, sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * m_nDescBufSizeInNumDescs,
			pSrcData, sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * m_nDescBufSizeInNumDescs);

		pDescBuf->Unmap(0, nullptr);
		m_spInstanceDescBuf->Unmap(0, nullptr);

		m_spInstanceDescBuf = std::move(pDescBuf);
		m_nDescBufSizeInNumDescs += m_nIncrementalSize;
	}
	// Upload
	m_spInstanceDescBuf->UploadSync(
		sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * m_nCurIdx,
		sizeof(D3D12_RAYTRACING_INSTANCE_DESC), pDesc);
	m_nCurIdx++;
	m_Inputs.InstanceDescs = m_spInstanceDescBuf->GPUVirtualAddress();
	m_Inputs.NumDescs = m_nCurIdx;
}

void RayTracingTLASInputs::Reset(
	ig::D3D12::CDevice::sPtr spDevice, 
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags,
	UINT nInitialReservedInstances, 
	UINT nIncrementalSize)
{
	ZeroMemory(&m_Inputs, sizeof(m_Inputs));
	m_Inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	m_Inputs.Flags = flags;
	m_Inputs.NumDescs = 0;
	m_Inputs.InstanceDescs = 0;
	m_Inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	m_nDescBufSizeInNumDescs = nInitialReservedInstances;
	m_nIncrementalSize = nIncrementalSize;
	if (m_nDescBufSizeInNumDescs > 0)
	{
		m_spInstanceDescBuf = ig::D3D12::CBuffer::CreateAsUpload(spDevice, m_nDescBufSizeInNumDescs * sizeof(D3D12_RAYTRACING_INSTANCE_DESC));
	}
	m_spDevice = spDevice;
}
	
CRayTracingAS::CRayTracingAS(
	CDevice::sPtr spDevice,
	const RayTracingBLASInputs &blasInputs) :
	m_TLASInputs(spDevice, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE, 0, 0)
{
	IG_D3D_ASSERT_SUCCEEDED(spDevice->RawPtr()->QueryInterface(IID_PPV_ARGS(&m_pDXRDevice)));
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo = {};
	m_pDXRDevice->GetRaytracingAccelerationStructurePrebuildInfo(blasInputs, &prebuildInfo);

	m_upASBuffer = CBuffer::Create(spDevice,
		prebuildInfo.ResultDataMaxSizeInBytes,
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);

	m_upScratchBuffer = CBuffer::Create(spDevice,
		prebuildInfo.ScratchDataSizeInBytes,
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	m_ASType = BLAS;
	m_spDevice = spDevice;
	m_BLASInputs = blasInputs;
}

CRayTracingAS::CRayTracingAS(
	CDevice::sPtr spDevice,
	const RayTracingTLASInputs &tlasInputs) :
	m_TLASInputs(spDevice, D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE, 0, 0)
{
	IG_D3D_ASSERT_SUCCEEDED(spDevice->RawPtr()->QueryInterface(IID_PPV_ARGS(&m_pDXRDevice)));
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo = {};
	m_pDXRDevice->GetRaytracingAccelerationStructurePrebuildInfo(tlasInputs, &prebuildInfo);

	m_upASBuffer = CBuffer::Create(spDevice,
		prebuildInfo.ResultDataMaxSizeInBytes,
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);

	m_upScratchBuffer = CBuffer::Create(spDevice,
		prebuildInfo.ScratchDataSizeInBytes,
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	m_ASType = TLAS;
	m_spDevice = spDevice;
	m_TLASInputs = tlasInputs;
}


void CRayTracingAS::BuildSync()
{
	auto pContext = m_spDevice->GraphicsQueue()->RequestComputeContext();
	BuildASync(pContext);
	m_spDevice->GraphicsQueue()->Execute(pContext);
	m_spDevice->GraphicsQueue()->WaitForIdle();
}

void CRayTracingAS::BuildASync(CComputeCommandContext* pComputeContext)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc = {};
	{
		if (m_ASType == BLAS)
			buildDesc.Inputs = m_BLASInputs;
		else
			buildDesc.Inputs = m_TLASInputs;
		buildDesc.ScratchAccelerationStructureData = m_upScratchBuffer->GPUVirtualAddress();
		buildDesc.DestAccelerationStructureData = m_upASBuffer->GPUVirtualAddress();
	}
	pComputeContext->BuildRaytracingAccelerationStructure(&buildDesc, 0, nullptr);
}
#endif