#pragma once

// Usage: D3D12 Raytracing Acceleration Structure (Wrapper of a buffer resource.)
// Created: 2019.6.3
// 2019.6.3: Initial Version.
// Version: 0.1.190603.1800

#include <d3d12.h>
#include <vector>
#include <DirectXMath.h>
#include "igD3D12Device.h"
#include "igD3D12Buffer.h"

namespace ig
{
	namespace D3D12
	{
		class RayTracingBLASInputs
		{
		public:
			RayTracingBLASInputs(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE);
			
			void AddGeometry(const D3D12_RAYTRACING_GEOMETRY_DESC *pDesc);
			
			void AddTriangleTypeGeometry(
				D3D12_GPU_VIRTUAL_ADDRESS vbGPUAddress,
				UINT64 vbStrideInBytes,
				UINT nVertices,
				DXGI_FORMAT vertexFormat,
				D3D12_GPU_VIRTUAL_ADDRESS ibGPUAddress,
				UINT nIndices,
				DXGI_FORMAT indexFormat,
				D3D12_GPU_VIRTUAL_ADDRESS transform3x4 = 0,
				D3D12_RAYTRACING_GEOMETRY_FLAGS flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE);

			void AddAABBTypeGeometry(
				D3D12_GPU_VIRTUAL_ADDRESS aabbGPUAddress,
				UINT64 aabbStrideInBytes,
				UINT64 nAABBs,
				D3D12_RAYTRACING_GEOMETRY_FLAGS flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE);
			
			void Reset(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE);
			

			operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS*() const;
			

			operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS&() const;
			
		private:
			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS m_Inputs;
			std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> m_Descs;
		};

		class RayTracingTLASInputs
		{
		public:
			RayTracingTLASInputs(
				ig::D3D12::CDevice::sPtr spDevice,
				D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
				UINT nInitialReservedInstances = 128, 
				UINT nIncrementalSize = 128);
			
			void AddInstance(
				D3D12_GPU_VIRTUAL_ADDRESS BLASAddress,
				UINT instanceId,
				DirectX::XMFLOAT3X4 *pTransform = nullptr,
				UINT instanceMask = 1,
				UINT instanceContributionToHitGroupIdx = 0,
				UINT flags = 0);
			
			void AddInstance(const D3D12_RAYTRACING_INSTANCE_DESC *pDesc);

			void Reset(
				ig::D3D12::CDevice::sPtr spDevice,
				D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
				UINT nInitialReservedInstances = 128, UINT nIncrementalSize = 128);

			UINT NumInstances() const;	

			operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS*() const;
			
			operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS&() const;

		private:
			ig::D3D12::CDevice::sPtr m_spDevice;
			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS m_Inputs;
			ig::D3D12::CBuffer::sPtr m_spInstanceDescBuf;
			UINT m_nCurIdx = 0;
			UINT m_nDescBufSizeInNumDescs = 0;
			UINT m_nIncrementalSize = 0;
		};

		class CRayTracingAS
		{
		public:
			enum Type
			{
				BLAS,
				TLAS,
			};
			CRayTracingAS(
				CDevice::sPtr spDevice,
				const RayTracingBLASInputs &blasInputs);

			CRayTracingAS(
				CDevice::sPtr spDevice,
				const RayTracingTLASInputs &tlasInputs);

		public:
			typedef std::shared_ptr<CRayTracingAS> sPtr;
			typedef std::unique_ptr<CRayTracingAS> uPtr;

			static uPtr CreateAsBLAS(
				CDevice::sPtr spDevice,
				const RayTracingBLASInputs &blasInputs);
			

			static uPtr CreateAsTLAS(
				CDevice::sPtr spDevice,
				const RayTracingTLASInputs &tlasInputs);
			
			void BuildSync();
			

			void BuildASync(CComputeCommandContext* pComputeContext);
			
			// TODO
			//void UpdateSync();
			//void UpdateASync();

			// Getters
			CBuffer* BufferPtr() const;
			CDevice* DevicePtr() const;
			D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() const;

			HRESULT SetName(LPCWSTR name);
			
		private:
			ig::D3D12::CDevice::sPtr m_spDevice;
			ig::D3D12::Device5ComPtr m_pDXRDevice;
			ig::D3D12::CBuffer::uPtr m_upASBuffer;
			ig::D3D12::CBuffer::uPtr m_upScratchBuffer;
			RayTracingBLASInputs m_BLASInputs;
			RayTracingTLASInputs m_TLASInputs;
			Type m_ASType;
		};
	}
}

#include "inl\igD3D12RayTracingAS.inl"
