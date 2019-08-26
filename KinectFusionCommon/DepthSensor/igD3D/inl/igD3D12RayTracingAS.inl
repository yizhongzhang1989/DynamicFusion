#pragma once

namespace ig
{
	namespace D3D12
	{
		inline RayTracingBLASInputs::RayTracingBLASInputs(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags)
		{
			Reset(flags);
		}

		inline RayTracingBLASInputs::operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS*() const
		{
			return &m_Inputs;
		}

		inline RayTracingBLASInputs::operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS&() const
		{
			return m_Inputs;
		}

		inline RayTracingTLASInputs::RayTracingTLASInputs(
			ig::D3D12::CDevice::sPtr spDevice,
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS flags,
			UINT nInitialReservedInstances,
			UINT nIncrementalSize)
		{
			Reset(spDevice, flags, nInitialReservedInstances, nIncrementalSize);
		}

		inline UINT RayTracingTLASInputs::NumInstances() const
		{
			return m_nCurIdx;
		}

		inline RayTracingTLASInputs::operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS*() const
		{
			return &m_Inputs;
		}

		inline RayTracingTLASInputs::operator const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS&() const
		{
			return m_Inputs;
		}

		inline CRayTracingAS::uPtr CRayTracingAS::CreateAsBLAS(
			CDevice::sPtr spDevice,
			const RayTracingBLASInputs &blasInputs)
		{
			return std::make_unique<CRayTracingAS>(spDevice, blasInputs);
		}

		inline CRayTracingAS::uPtr CRayTracingAS::CreateAsTLAS(
			CDevice::sPtr spDevice,
			const RayTracingTLASInputs &tlasInputs)
		{
			return std::make_unique<CRayTracingAS>(spDevice, tlasInputs);
		}

		inline CBuffer* CRayTracingAS::BufferPtr() const
		{
			return m_upASBuffer.get();
		}

		inline CDevice* CRayTracingAS::DevicePtr() const
		{
			return m_spDevice.get();
		}

		inline D3D12_GPU_VIRTUAL_ADDRESS CRayTracingAS::GPUVirtualAddress() const
		{
			return m_upASBuffer->GPUVirtualAddress();
		}

		inline HRESULT CRayTracingAS::SetName(LPCWSTR name)
		{
			return m_upASBuffer->SetName(name);
		}
	}
}
