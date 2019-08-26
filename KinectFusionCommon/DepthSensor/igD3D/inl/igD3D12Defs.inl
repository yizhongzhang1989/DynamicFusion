#pragma once

namespace ig
{
	namespace D3D12
	{
		inline CommandQueueDesc& CommandQueueDesc::AsDirect(
			INT priority,
			D3D12_COMMAND_QUEUE_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			Priority = priority;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline CommandQueueDesc& CommandQueueDesc::AsBundle(
			INT priority,
			D3D12_COMMAND_QUEUE_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_COMMAND_LIST_TYPE_BUNDLE;
			Priority = priority;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline CommandQueueDesc& CommandQueueDesc::AsCompute(
			INT priority,
			D3D12_COMMAND_QUEUE_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			Priority = priority;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline CommandQueueDesc& CommandQueueDesc::AsCopy(
			INT priority,
			D3D12_COMMAND_QUEUE_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_COMMAND_LIST_TYPE_COPY;
			Priority = priority;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline CommandQueueDesc::operator const D3D12_COMMAND_QUEUE_DESC*() const
		{
			return this;
		}



		inline DescriptorHeapDesc& DescriptorHeapDesc::AsCBVSRVUAV(
			UINT numDescriptors,
			D3D12_DESCRIPTOR_HEAP_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			NumDescriptors = numDescriptors;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline DescriptorHeapDesc& DescriptorHeapDesc::AsSampler(
			UINT numDescriptors,
			D3D12_DESCRIPTOR_HEAP_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			NumDescriptors = numDescriptors;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline DescriptorHeapDesc& DescriptorHeapDesc::AsRTV(
			UINT numDescriptors,
			D3D12_DESCRIPTOR_HEAP_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			NumDescriptors = numDescriptors;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline DescriptorHeapDesc& DescriptorHeapDesc::AsDSV(
			UINT numDescriptors,
			D3D12_DESCRIPTOR_HEAP_FLAGS flags,
			UINT nodeMask)
		{
			Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			NumDescriptors = numDescriptors;
			Flags = flags;
			NodeMask = nodeMask;
			return *this;
		}

		inline DescriptorHeapDesc::operator const D3D12_DESCRIPTOR_HEAP_DESC*() const
		{
			return this;
		}



		inline HeapDesc& HeapDesc::AsDefault(
			UINT64 size,
			UINT64 alignment,
			D3D12_HEAP_FLAGS flags,
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			SizeInBytes = size;
			Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
			Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			Properties.CreationNodeMask = creationNodeMask;
			Properties.VisibleNodeMask = visibleNodeMask;
			Alignment = alignment;
			Flags = flags;
			return *this;
		}

		inline HeapDesc& HeapDesc::AsUpload(
			UINT64 size,
			UINT64 alignment,
			D3D12_HEAP_FLAGS flags,
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			SizeInBytes = size;
			Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
			Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			Properties.CreationNodeMask = creationNodeMask;
			Properties.VisibleNodeMask = visibleNodeMask;
			Alignment = alignment;
			Flags = flags;
			return *this;
		}

		inline HeapDesc& HeapDesc::AsReadback(
			UINT64 size,
			UINT64 alignment,
			D3D12_HEAP_FLAGS flags,
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			SizeInBytes = size;
			Properties.Type = D3D12_HEAP_TYPE_READBACK;
			Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			Properties.CreationNodeMask = creationNodeMask;
			Properties.VisibleNodeMask = visibleNodeMask;
			Alignment = alignment;
			Flags = flags;
			return *this;
		}

		inline HeapDesc& HeapDesc::AsCustom(
			UINT64 size,
			D3D12_CPU_PAGE_PROPERTY pageProperty,
			D3D12_MEMORY_POOL memPool,
			UINT64 alignment,
			D3D12_HEAP_FLAGS flags,
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			SizeInBytes = size;
			Properties.Type = D3D12_HEAP_TYPE_CUSTOM;
			Properties.CPUPageProperty = pageProperty;
			Properties.MemoryPoolPreference = memPool;
			Properties.CreationNodeMask = creationNodeMask;
			Properties.VisibleNodeMask = visibleNodeMask;
			Alignment = alignment;
			Flags = flags;
			return *this;
		}

		inline HeapDesc::operator const D3D12_HEAP_DESC*() const
		{
			return this;
		}

		inline HeapProperties& HeapProperties::As(
			D3D12_HEAP_TYPE type,
			D3D12_CPU_PAGE_PROPERTY cpuPageProperty,
			D3D12_MEMORY_POOL memoryPoolPreference,
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			Type = type;
			CPUPageProperty = cpuPageProperty;
			MemoryPoolPreference = memoryPoolPreference;
			CreationNodeMask = creationNodeMask;
			VisibleNodeMask = visibleNodeMask;
			return *this;
		}

		inline HeapProperties& HeapProperties::AsDefault(
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			Type = D3D12_HEAP_TYPE_DEFAULT;
			CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			CreationNodeMask = creationNodeMask;
			VisibleNodeMask = visibleNodeMask;
			return *this;
		}

		inline HeapProperties& HeapProperties::AsUpload(
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			Type = D3D12_HEAP_TYPE_UPLOAD;
			CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			CreationNodeMask = creationNodeMask;
			VisibleNodeMask = visibleNodeMask;
			return *this;
		}

		inline HeapProperties& HeapProperties::AsReadback(
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			Type = D3D12_HEAP_TYPE_READBACK;
			CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			CreationNodeMask = creationNodeMask;
			VisibleNodeMask = visibleNodeMask;
			return *this;
		}

		inline HeapProperties& HeapProperties::AsCustom(
			D3D12_CPU_PAGE_PROPERTY pageProperty,
			D3D12_MEMORY_POOL memPool,
			UINT creationNodeMask,
			UINT visibleNodeMask)
		{
			Type = D3D12_HEAP_TYPE_CUSTOM;
			CPUPageProperty = pageProperty;
			MemoryPoolPreference = memPool;
			CreationNodeMask = creationNodeMask;
			VisibleNodeMask = visibleNodeMask;
			return *this;
		}

		inline HeapProperties::operator const D3D12_HEAP_PROPERTIES*() const
		{
			return this;
		}

		inline ResourceDesc& ResourceDesc::AsBuffer(
			UINT64 size,
			D3D12_RESOURCE_FLAGS flags,
			UINT64 alignment)
		{
			Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			Alignment = alignment;
			Width = size;
			Height = 1;
			DepthOrArraySize = 1;
			MipLevels = 1;
			Format = DXGI_FORMAT_UNKNOWN;
			SampleDesc.Count = 1;
			SampleDesc.Quality = 0;
			Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			Flags = flags;
			return *this;
		}

		inline ResourceDesc& ResourceDesc::AsTex1D(
			DXGI_FORMAT format,
			UINT64 width,
			UINT16 mipLevels,
			UINT16 arraySize,
			D3D12_RESOURCE_FLAGS flags,
			D3D12_TEXTURE_LAYOUT layout,
			UINT64 alignment)
		{
			Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			Alignment = alignment;
			Width = width;
			Height = 1;
			DepthOrArraySize = arraySize;
			MipLevels = mipLevels;
			Format = format;
			SampleDesc.Count = 1;
			SampleDesc.Quality = 0;
			Layout = layout;
			Flags = flags;
			return *this;
		}

		inline ResourceDesc& ResourceDesc::AsTex2D(
			DXGI_FORMAT format,
			UINT64 width,
			UINT height,
			UINT16 mipLevels,
			UINT16 arraySize,
			UINT sampleCount,
			UINT sampleQuality,
			D3D12_RESOURCE_FLAGS flags,
			D3D12_TEXTURE_LAYOUT layout,
			UINT64 alignment)
		{
			Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			Alignment = alignment;
			Width = width;
			Height = height;
			DepthOrArraySize = arraySize;
			MipLevels = mipLevels;
			Format = format;
			SampleDesc.Count = sampleCount;
			SampleDesc.Quality = sampleQuality;
			Layout = layout;
			Flags = flags;
			return *this;
		}

		inline ResourceDesc& ResourceDesc::AsTex3D(
			DXGI_FORMAT format,
			UINT64 width,
			UINT height,
			UINT16 depth,
			UINT16 mipLevels,
			D3D12_RESOURCE_FLAGS flags,
			D3D12_TEXTURE_LAYOUT layout,
			UINT64 alignment)
		{
			Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			Alignment = alignment;
			Width = width;
			Height = height;
			DepthOrArraySize = depth;
			MipLevels = mipLevels;
			Format = format;
			SampleDesc.Count = 1;
			SampleDesc.Quality = 0;
			Layout = layout;
			Flags = flags;
			return *this;
		}

		inline ResourceDesc::operator const D3D12_RESOURCE_DESC*() const
		{
			return this;
		}


		inline SRVDesc& SRVDesc::AsBuffer(
			DXGI_FORMAT format,
			UINT64 firstElement,
			UINT numElements,
			UINT structureByteStride,
			D3D12_BUFFER_SRV_FLAGS flags,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			Shader4ComponentMapping = shaderComponentMapping;
			Buffer.FirstElement = firstElement;
			Buffer.NumElements = numElements;
			Buffer.StructureByteStride = structureByteStride;
			Buffer.Flags = flags;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTex1D(
			DXGI_FORMAT format,
			UINT mostDetailedMip,
			UINT mipLevels,
			FLOAT resourceMinLODClamp,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			Shader4ComponentMapping = shaderComponentMapping;
			Texture1D.MostDetailedMip = mostDetailedMip;
			Texture1D.MipLevels = mipLevels;
			Texture1D.ResourceMinLODClamp = resourceMinLODClamp;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTex1DArray(
			DXGI_FORMAT format,
			UINT mostDetailedMip,
			UINT mipLevels,
			UINT firstArraySlice,
			UINT arraySize,
			UINT planeSlice,
			FLOAT resourceMinLODClamp,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
			Shader4ComponentMapping = shaderComponentMapping;
			Texture1DArray.MostDetailedMip = mostDetailedMip;
			Texture1DArray.MipLevels = mipLevels;
			Texture1DArray.FirstArraySlice = firstArraySlice;
			Texture1DArray.ArraySize = arraySize;
			Texture1DArray.ResourceMinLODClamp = resourceMinLODClamp;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTex2D(
			DXGI_FORMAT format,
			UINT mostDetailedMip,
			UINT mipLevels,
			UINT planeSlice,
			FLOAT resourceMinLODClamp,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			Shader4ComponentMapping = shaderComponentMapping;
			Texture2D.MostDetailedMip = mostDetailedMip;
			Texture2D.MipLevels = mipLevels;
			Texture2D.PlaneSlice = planeSlice;
			Texture2D.ResourceMinLODClamp = resourceMinLODClamp;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTex2DArray(
			DXGI_FORMAT format,
			UINT mostDetailedMip,
			UINT mipLevels,
			UINT firstArraySlice,
			UINT arraySize,
			UINT planeSlice,
			FLOAT resourceMinLODClamp,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			Shader4ComponentMapping = shaderComponentMapping;
			Texture2DArray.MostDetailedMip = mostDetailedMip;
			Texture2DArray.MipLevels = mipLevels;
			Texture2DArray.FirstArraySlice = firstArraySlice;
			Texture2DArray.ArraySize = arraySize;
			Texture2DArray.PlaneSlice = planeSlice;
			Texture2DArray.ResourceMinLODClamp = resourceMinLODClamp;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTex2DMS(
			DXGI_FORMAT format,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			Shader4ComponentMapping = shaderComponentMapping;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTex2DMSArray(
			DXGI_FORMAT format,
			UINT firstArraySlice,
			UINT arraySize,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			Shader4ComponentMapping = shaderComponentMapping;
			Texture2DMSArray.FirstArraySlice = firstArraySlice;
			Texture2DMSArray.ArraySize = arraySize;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTex3D(
			DXGI_FORMAT format,
			UINT mostDetailedMip,
			UINT mipLevels,
			FLOAT resourceMinLODClamp,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
			Shader4ComponentMapping = shaderComponentMapping;
			Texture3D.MostDetailedMip = mostDetailedMip;
			Texture3D.MipLevels = mipLevels;
			Texture3D.ResourceMinLODClamp = resourceMinLODClamp;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTexCube(
			DXGI_FORMAT format,
			UINT  mostDetailedMip,
			UINT  mipLevels,
			FLOAT resourceMinLODClamp,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			Shader4ComponentMapping = shaderComponentMapping;
			TextureCube.MostDetailedMip = mostDetailedMip;
			TextureCube.MipLevels = mipLevels;
			TextureCube.ResourceMinLODClamp = resourceMinLODClamp;
			return *this;
		}

		inline SRVDesc& SRVDesc::AsTexCubeArray(
			DXGI_FORMAT format,
			UINT mostDetailedMip,
			UINT mipLevels,
			UINT first2DArrayFace,
			UINT numCubes,
			FLOAT resourceMinLODClamp,
			UINT shaderComponentMapping)
		{
			Format = format;
			ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
			Shader4ComponentMapping = shaderComponentMapping;
			TextureCubeArray.MostDetailedMip = mostDetailedMip;
			TextureCubeArray.MipLevels = mipLevels;
			TextureCubeArray.First2DArrayFace = first2DArrayFace;
			TextureCubeArray.NumCubes = numCubes;
			TextureCubeArray.ResourceMinLODClamp = resourceMinLODClamp;
			return *this;
		}

		inline SRVDesc::operator const D3D12_SHADER_RESOURCE_VIEW_DESC*() const
		{
			return this;
		}



		inline RTVDesc& RTVDesc::AsBuffer(
			DXGI_FORMAT format,
			UINT64 firstElement,
			UINT numElements)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_BUFFER;
			Buffer.FirstElement = firstElement;
			Buffer.NumElements = numElements;
			return *this;
		}

		inline RTVDesc& RTVDesc::AsTex1D(
			DXGI_FORMAT format,
			UINT mipSlice)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
			Texture1D.MipSlice = mipSlice;
			return *this;
		}

		inline RTVDesc& RTVDesc::AsTex1DArray(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstArraySlice,
			UINT arraySize)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
			Texture1DArray.MipSlice = mipSlice;
			Texture1DArray.FirstArraySlice = firstArraySlice;
			Texture1DArray.ArraySize = arraySize;
			return *this;
		}

		inline RTVDesc& RTVDesc::AsTex2D(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT planeSlice)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			Texture2D.MipSlice = mipSlice;
			Texture2D.PlaneSlice = planeSlice;
			return *this;
		}

		inline RTVDesc& RTVDesc::AsTex2DArray(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstArraySlice,
			UINT arraySize,
			UINT planeSlice)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
			Texture2DArray.MipSlice = mipSlice;
			Texture2DArray.FirstArraySlice = firstArraySlice;
			Texture2DArray.ArraySize = arraySize;
			Texture2DArray.PlaneSlice = planeSlice;
			return *this;
		}

		inline RTVDesc& RTVDesc::AsTex2DMS(DXGI_FORMAT format)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
			return *this;
		}

		inline RTVDesc& RTVDesc::AsTex2DMSArray(
			DXGI_FORMAT format,
			UINT firstArraySlice,
			UINT arraySize)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
			Texture2DMSArray.FirstArraySlice = firstArraySlice;
			Texture2DMSArray.ArraySize = arraySize;
			return *this;
		}

		inline RTVDesc& RTVDesc::AsTex3D(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstWSlice,
			UINT wSize)
		{
			Format = format;
			ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
			Texture3D.MipSlice = mipSlice;
			Texture3D.FirstWSlice = firstWSlice;
			Texture3D.WSize = wSize;
			return *this;
		}

		inline RTVDesc::operator const D3D12_RENDER_TARGET_VIEW_DESC*() const
		{
			return this;
		}

		inline DSVDesc& DSVDesc::AsTex1D(
			DXGI_FORMAT format,
			UINT mipSlice,
			D3D12_DSV_FLAGS flags)
		{
			Format = format;
			ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
			Flags = flags;
			Texture1D.MipSlice = mipSlice;
			return *this;
		}

		inline DSVDesc& DSVDesc::AsTex1DArray(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstArraySlice,
			UINT arraySize,
			D3D12_DSV_FLAGS flags)
		{
			Format = format;
			ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
			Flags = flags;
			Texture1DArray.MipSlice = mipSlice;
			Texture1DArray.FirstArraySlice = firstArraySlice;
			Texture1DArray.ArraySize = arraySize;
			return *this;
		}

		inline DSVDesc& DSVDesc::AsTex2D(
			DXGI_FORMAT format,
			UINT mipSlice,
			D3D12_DSV_FLAGS flags)
		{
			Format = format;
			ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			Flags = flags;
			Texture2D.MipSlice = mipSlice;
			return *this;
		}

		inline DSVDesc& DSVDesc::AsTex2DArray(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstArraySlice,
			UINT arraySize,
			D3D12_DSV_FLAGS flags)
		{
			Format = format;
			ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			Flags = flags;
			Texture2DArray.MipSlice = mipSlice;
			Texture2DArray.FirstArraySlice = firstArraySlice;
			Texture2DArray.ArraySize = arraySize;
			return *this;
		}

		inline DSVDesc& DSVDesc::AsTex2DMS(
			DXGI_FORMAT format,
			D3D12_DSV_FLAGS flags)
		{
			Format = format;
			ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			Flags = flags;
			return *this;
		}

		inline DSVDesc& DSVDesc::AsTex2DMSArray(
			DXGI_FORMAT format,
			UINT firstArraySlice,
			UINT arraySize,
			D3D12_DSV_FLAGS flags)
		{
			Format = format;
			ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
			Flags = flags;
			Texture2DMSArray.FirstArraySlice = firstArraySlice;
			Texture2DMSArray.ArraySize = arraySize;
			return *this;
		}

		inline DSVDesc::operator const D3D12_DEPTH_STENCIL_VIEW_DESC*() const
		{
			return this;
		}

		inline UAVDesc& UAVDesc::AsBuffer(
			DXGI_FORMAT format,
			UINT64 firstElement,
			UINT numElements,
			UINT structureByteStride,
			UINT64 counterOffsetInBytes,
			D3D12_BUFFER_UAV_FLAGS flags)
		{
			Format = format;
			ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			Buffer.FirstElement = firstElement;
			Buffer.NumElements = numElements;
			Buffer.StructureByteStride = structureByteStride;
			Buffer.CounterOffsetInBytes = counterOffsetInBytes;
			Buffer.Flags = flags;
			return *this;
		}

		inline UAVDesc& UAVDesc::AsTex1D(
			DXGI_FORMAT format,
			UINT mipSlice)
		{
			Format = format;
			ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
			Texture1D.MipSlice = mipSlice;
			return *this;
		}

		inline UAVDesc& UAVDesc::AsTex1DArray(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstArraySlice,
			UINT arraySize)
		{
			Format = format;
			ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
			Texture1DArray.MipSlice = mipSlice;
			Texture1DArray.FirstArraySlice = firstArraySlice;
			Texture1DArray.ArraySize = arraySize;
			return *this;
		}

		inline UAVDesc& UAVDesc::AsTex2D(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT planeSlice)
		{
			Format = format;
			ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			Texture2D.MipSlice = mipSlice;
			Texture2D.PlaneSlice = planeSlice;
			return *this;
		}

		inline UAVDesc& UAVDesc::AsTex2DArray(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstArraySlice,
			UINT arraySize,
			UINT planeSlice)
		{
			Format = format;
			ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
			Texture2DArray.MipSlice = mipSlice;
			Texture2DArray.FirstArraySlice = firstArraySlice;
			Texture2DArray.ArraySize = arraySize;
			Texture2DArray.PlaneSlice = planeSlice;
			return *this;
		}

		inline UAVDesc& UAVDesc::AsTex3D(
			DXGI_FORMAT format,
			UINT mipSlice,
			UINT firstWSlice,
			UINT wSize)
		{
			Format = format;
			ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			Texture3D.MipSlice = mipSlice;
			Texture3D.FirstWSlice = firstWSlice;
			Texture3D.WSize = wSize;
			return *this;
		}

		inline UAVDesc::operator const D3D12_UNORDERED_ACCESS_VIEW_DESC*() const
		{
			return this;
		}


		inline RasterizerDesc& RasterizerDesc::As(
			D3D12_FILL_MODE fillMode,
			D3D12_CULL_MODE cullMode,
			BOOL frontCounterClockwise,
			INT depthBias,
			FLOAT depthBiasClamp,
			FLOAT slopeScaledDepthBias,
			BOOL depthClipEnable,
			BOOL multisampleEnable,
			BOOL antialiasedLineEnable,
			UINT forcedSampleCount,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE conservativeRaster)
		{
			FillMode = fillMode;
			CullMode = cullMode;
			FrontCounterClockwise = frontCounterClockwise;
			DepthBias = depthBias;
			DepthBiasClamp = depthBiasClamp;
			SlopeScaledDepthBias = slopeScaledDepthBias;
			DepthClipEnable = depthClipEnable;
			MultisampleEnable = multisampleEnable;
			AntialiasedLineEnable = antialiasedLineEnable;
			ForcedSampleCount = forcedSampleCount;
			ConservativeRaster = conservativeRaster;
			return *this;
		}

		inline DepthStencilDesc& DepthStencilDesc::As(
			BOOL depthEnable,
			D3D12_DEPTH_WRITE_MASK depthWriteMask,
			D3D12_COMPARISON_FUNC depthFunc,
			BOOL stencilEnable,
			UINT8 stencilReadMask,
			UINT8 stencilWriteMask,
			D3D12_STENCIL_OP frontFaceStencilFailOp,
			D3D12_STENCIL_OP frontFaceStencilDepthFailOp,
			D3D12_STENCIL_OP frontFaceStencilPassOp,
			D3D12_COMPARISON_FUNC frontFaceStencilFunc,
			D3D12_STENCIL_OP backFaceStencilFailOp,
			D3D12_STENCIL_OP backFaceStencilDepthFailOp,
			D3D12_STENCIL_OP backFaceStencilPassOp,
			D3D12_COMPARISON_FUNC backFaceStencilFunc)
		{
			DepthEnable = depthEnable;
			DepthWriteMask = depthWriteMask;
			DepthFunc = depthFunc;
			StencilEnable = stencilEnable;
			StencilReadMask = stencilReadMask;
			StencilWriteMask = stencilWriteMask;
			FrontFace.StencilFailOp = frontFaceStencilFailOp;
			FrontFace.StencilDepthFailOp = frontFaceStencilDepthFailOp;
			FrontFace.StencilPassOp = frontFaceStencilPassOp;
			FrontFace.StencilFunc = frontFaceStencilFunc;
			BackFace.StencilFailOp = backFaceStencilFailOp;
			BackFace.StencilDepthFailOp = backFaceStencilDepthFailOp;
			BackFace.StencilPassOp = backFaceStencilPassOp;
			BackFace.StencilFunc = backFaceStencilFunc;
			return *this;
		}

		inline RenderTargetBlendDesc& RenderTargetBlendDesc::As(
			BOOL blendEnable,
			BOOL logicOpEnable,
			D3D12_BLEND srcBlend,
			D3D12_BLEND destBlend,
			D3D12_BLEND_OP blendOp,
			D3D12_BLEND srcBlendAlpha,
			D3D12_BLEND destBlendAlpha,
			D3D12_BLEND_OP blendOpAlpha,
			D3D12_LOGIC_OP logicOp,
			UINT8 renderTargetWriteMask)
		{
			BlendEnable = blendEnable;
			LogicOpEnable = logicOpEnable;
			SrcBlend = srcBlend;
			DestBlend = destBlend;
			BlendOp = blendOp;
			SrcBlendAlpha = srcBlendAlpha;
			DestBlendAlpha = destBlendAlpha;
			BlendOpAlpha = blendOpAlpha;
			LogicOp = logicOp;
			RenderTargetWriteMask = renderTargetWriteMask;
			return *this;
		}

		inline BlendDesc& BlendDesc::As(
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget,
			BOOL alphaToCoverageEnable)
		{
			AlphaToCoverageEnable = alphaToCoverageEnable;
			IndependentBlendEnable = FALSE;
			RenderTarget[0] = renderTarget;
			return *this;
		}

		inline BlendDesc& BlendDesc::AsIndependent(
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget0,
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget1,
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget2,
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget3,
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget4,
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget5,
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget6,
			const D3D12_RENDER_TARGET_BLEND_DESC &renderTarget7,
			BOOL alphaToCoverageEnable)
		{
			AlphaToCoverageEnable = alphaToCoverageEnable;
			IndependentBlendEnable = TRUE;
			RenderTarget[0] = renderTarget0;
			RenderTarget[1] = renderTarget1;
			RenderTarget[2] = renderTarget2;
			RenderTarget[3] = renderTarget3;
			RenderTarget[4] = renderTarget4;
			RenderTarget[5] = renderTarget5;
			RenderTarget[6] = renderTarget6;
			RenderTarget[7] = renderTarget7;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::As(
			D3D12_FILTER filter,
			D3D12_TEXTURE_ADDRESS_MODE addressU,
			D3D12_TEXTURE_ADDRESS_MODE addressV,
			D3D12_TEXTURE_ADDRESS_MODE addressW,
			FLOAT mipLODBias,
			UINT maxAnisotropy,
			D3D12_COMPARISON_FUNC comparisonFunc,
			FLOAT borderColorR,
			FLOAT borderColorG,
			FLOAT borderColorB,
			FLOAT borderColorA,
			FLOAT minLOD,
			FLOAT maxLOD)
		{
			Filter = filter;
			AddressU = addressU;
			AddressV = addressV;
			AddressW = addressW;
			MipLODBias = mipLODBias;
			MaxAnisotropy = maxAnisotropy;
			ComparisonFunc = comparisonFunc;
			BorderColor[0] = borderColorR;
			BorderColor[1] = borderColorG;
			BorderColor[2] = borderColorB;
			BorderColor[3] = borderColorA;
			MinLOD = minLOD;
			MaxLOD = maxLOD;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsPointClamp()
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsPointWrap()
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsPointMirror()
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsLinearClamp()
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsLinearWrap()
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsLinearMirror()
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsAnisoClamp(UINT maxAnisotropy)
		{
			Filter = D3D12_FILTER_ANISOTROPIC;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 16;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsAnisoWrap(UINT maxAnisotropy)
		{
			Filter = D3D12_FILTER_ANISOTROPIC;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 16;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc& SamplerDesc::AsAnisoMirror(UINT maxAnisotropy)
		{
			Filter = D3D12_FILTER_ANISOTROPIC;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			MipLODBias = 0.0f;
			MaxAnisotropy = 16;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor[0] = 0;
			BorderColor[1] = 0;
			BorderColor[2] = 0;
			BorderColor[3] = 0;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			return *this;
		}

		inline SamplerDesc::operator const D3D12_SAMPLER_DESC*() const
		{
			return this;
		}

		inline RootSignatureDesc::RootParameter::RootParameter()
		{
			ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
			DescriptorTable.pDescriptorRanges = nullptr;
		}

		inline RootSignatureDesc::RootParameter::~RootParameter()
		{
			Clear();
		}

		inline void RootSignatureDesc::RootParameter::Clear()
		{
			if (ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
				delete[] DescriptorTable.pDescriptorRanges;
			ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
			DescriptorTable.pDescriptorRanges = nullptr;
		}

		inline void RootSignatureDesc::RootParameter::AsConstants(UINT shaderRegister, UINT numDWords, D3D12_SHADER_VISIBILITY visibility)
		{
			ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			Constants.Num32BitValues = numDWords;
			Constants.RegisterSpace = 0;
			Constants.ShaderRegister = shaderRegister;
			ShaderVisibility = visibility;
		}

		inline void RootSignatureDesc::RootParameter::AsConstantBuffer(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility)
		{
			ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			Descriptor.RegisterSpace = 0;
			Descriptor.ShaderRegister = shaderRegister;
			ShaderVisibility = visibility;
		}

		inline void RootSignatureDesc::RootParameter::AsBufferSRV(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility)
		{
			ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
			Descriptor.RegisterSpace = 0;
			Descriptor.ShaderRegister = shaderRegister;
			ShaderVisibility = visibility;
		}

		inline void RootSignatureDesc::RootParameter::AsBufferUAV(UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility)
		{
			ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
			Descriptor.RegisterSpace = 0;
			Descriptor.ShaderRegister = shaderRegister;
			ShaderVisibility = visibility;
		}

		inline RootSignatureDesc::RootParameter& RootSignatureDesc::RootParameter::AsDescriptorTable(UINT rangeCount, D3D12_SHADER_VISIBILITY visibility)
		{
			Clear();
			ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			DescriptorTable.NumDescriptorRanges = rangeCount;
			DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[rangeCount];
			ShaderVisibility = visibility;
			return *this;
		}

		inline void RootSignatureDesc::RootParameter::SetTableRange(UINT rangeIdx, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT count, UINT space, UINT OffsetFromTableStart)
		{
			IG_D3D_ASSERT(ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE &&
				DescriptorTable.pDescriptorRanges != nullptr,
				L"RootParameters::SetTableRange: Invalid descriptor table, have you called AsDescriptorTable()? ");

			D3D12_DESCRIPTOR_RANGE* ranges = const_cast<D3D12_DESCRIPTOR_RANGE*>(DescriptorTable.pDescriptorRanges);
			ranges[rangeIdx].RangeType = type;
			ranges[rangeIdx].NumDescriptors = count;
			ranges[rangeIdx].BaseShaderRegister = shaderRegister;
			ranges[rangeIdx].RegisterSpace = space;
			ranges[rangeIdx].OffsetInDescriptorsFromTableStart = OffsetFromTableStart;
		}

		// Memory layout should be SAME as D3D12_STATIC_SAMPLER_DESC
		// So no data member is allowed.
		inline void RootSignatureDesc::StaticSampler::As(
			D3D12_FILTER filter,
			D3D12_TEXTURE_ADDRESS_MODE addressU,
			D3D12_TEXTURE_ADDRESS_MODE addressV,
			D3D12_TEXTURE_ADDRESS_MODE addressW,
			FLOAT mipLODBias,
			UINT maxAnisotropy,
			D3D12_COMPARISON_FUNC comparisonFunc,
			D3D12_STATIC_BORDER_COLOR borderColor,
			FLOAT minLOD,
			FLOAT maxLOD,
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = filter;
			AddressU = addressU;
			AddressV = addressV;
			AddressW = addressW;
			MipLODBias = mipLODBias;
			MaxAnisotropy = maxAnisotropy;
			ComparisonFunc = comparisonFunc;
			BorderColor = borderColor;
			MinLOD = minLOD;
			MaxLOD = maxLOD;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsPointClamp(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}
		
		inline void RootSignatureDesc::StaticSampler::AsPointWrap(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsPointMirror(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsLinearClamp(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsLinearWrap(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsLinearMirror(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			MipLODBias = 0.0f;
			MaxAnisotropy = 1;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsAnisoClamp(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_ANISOTROPIC;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 16;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsAnisoWrap(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_ANISOTROPIC;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 16;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		inline void RootSignatureDesc::StaticSampler::AsAnisoMirror(
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility)
		{
			Filter = D3D12_FILTER_ANISOTROPIC;
			AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			MipLODBias = 0.0f;
			MaxAnisotropy = 16;
			ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			MinLOD = -FLT_MAX;
			MaxLOD = FLT_MAX;
			ShaderRegister = shaderRegister;
			RegisterSpace = registerSpace;
			ShaderVisibility = shaderVisibility;
		}

		// Init from D3D12_SAMPLER_DESC
		inline void RootSignatureDesc::StaticSampler::As(
			const D3D12_SAMPLER_DESC *pDesc,
			UINT shaderRegister,
			UINT registerSpace,
			D3D12_SHADER_VISIBILITY visibility)
		{
			AddressU = pDesc->AddressU;
			AddressV = pDesc->AddressV;
			AddressW = pDesc->AddressW;
			BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			ComparisonFunc = pDesc->ComparisonFunc;
			Filter = pDesc->Filter;
			MaxAnisotropy = pDesc->MaxAnisotropy;
			MaxLOD = pDesc->MaxLOD;
			MinLOD = pDesc->MinLOD;
			MipLODBias = pDesc->MipLODBias;
			RegisterSpace = 0;
			ShaderRegister = shaderRegister;
			ShaderVisibility = visibility;

			// for border color
			if (AddressU == D3D12_TEXTURE_ADDRESS_MODE_BORDER ||
				AddressV == D3D12_TEXTURE_ADDRESS_MODE_BORDER ||
				AddressW == D3D12_TEXTURE_ADDRESS_MODE_BORDER)
			{
				IG_D3D_ASSERT(
					// Transparent Black
					pDesc->BorderColor[0] == 0.0f &&
					pDesc->BorderColor[1] == 0.0f &&
					pDesc->BorderColor[2] == 0.0f &&
					pDesc->BorderColor[3] == 0.0f ||
					// Opaque Black
					pDesc->BorderColor[0] == 0.0f &&
					pDesc->BorderColor[1] == 0.0f &&
					pDesc->BorderColor[2] == 0.0f &&
					pDesc->BorderColor[3] == 1.0f ||
					// Opaque White
					pDesc->BorderColor[0] == 1.0f &&
					pDesc->BorderColor[1] == 1.0f &&
					pDesc->BorderColor[2] == 1.0f &&
					pDesc->BorderColor[3] == 1.0f,
					L"StaticSampler::As: Invalid border color settings.");

				if (pDesc->BorderColor[3] == 1.0f)
				{
					if (pDesc->BorderColor[0] == 1.0f)
						BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
					else
						BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
				}
			}
		}

		inline RootSignatureDesc::RootSignatureDesc(UINT numRootParameters, UINT numStaticSamplers, D3D12_ROOT_SIGNATURE_FLAGS flags)
		{
			Reset(numRootParameters, numStaticSamplers, flags);
		}

		inline void RootSignatureDesc::Reset(UINT numRootParameters, UINT numStaticSamplers, D3D12_ROOT_SIGNATURE_FLAGS flags)
		{
			m_RootParams = std::make_unique<RootParameter[]>(numRootParameters);
			m_StaticSamplers = std::make_unique<StaticSampler[]>(numStaticSamplers);
			m_Desc.Flags = flags;
			m_Desc.NumParameters = numRootParameters;
			m_Desc.NumStaticSamplers = numStaticSamplers;

			// Safe since RootParameter & StaticSampler have same layouts as D3D12_ROOT_PARAMETER & D3D12_STATIC_SAMPLER_DESC.
			m_Desc.pParameters = m_RootParams.get();
			m_Desc.pStaticSamplers = m_StaticSamplers.get();
		}

		inline RootSignatureDesc::operator const D3D12_ROOT_SIGNATURE_DESC*() const
		{
			return &m_Desc;
		}

		inline RootSignatureDesc::RootParameter& RootSignatureDesc::Parameter(UINT i) { return m_RootParams[i]; }
		inline const RootSignatureDesc::RootParameter& RootSignatureDesc::Parameter(UINT i) const { return m_RootParams[i]; }

		inline RootSignatureDesc::StaticSampler& RootSignatureDesc::Sampler(UINT i) { return m_StaticSamplers[i]; }
		inline const RootSignatureDesc::StaticSampler& RootSignatureDesc::Sampler(UINT i) const { return m_StaticSamplers[i]; }

		inline D3D12_ROOT_SIGNATURE_FLAGS RootSignatureDesc::Flags() const { return m_Desc.Flags; }

		inline UINT RootSignatureDesc::NumRootParameters() const { return m_Desc.NumParameters; }
		inline UINT RootSignatureDesc::NumStaticSamplers() const { return m_Desc.NumStaticSamplers; }

		inline GraphicsPipelineStateDesc::InputAssemblerStage::InputAssemblerStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc) : m_pDesc(pDesc)
		{
			// Default value
			SetInputLayout();
			SetIBStripCutValue();
			SetPrimitiveTopologyType();
		}
		inline void GraphicsPipelineStateDesc::InputAssemblerStage::SetInputLayout(UINT numElements, const D3D12_INPUT_ELEMENT_DESC* pDescs)
		{
			m_pDesc->InputLayout.NumElements = numElements;
			m_pDesc->InputLayout.pInputElementDescs = pDescs;
			//m_InputLayouts = std::shared_ptr<D3D12_INPUT_ELEMENT_DESC>(new D3D12_INPUT_ELEMENT_DESC[numElements], [](D3D12_INPUT_ELEMENT_DESC* p) {delete[]p; });
			//memcpy(m_InputLayouts.get(), pDescs, numElements * sizeof(D3D12_INPUT_ELEMENT_DESC));
			//m_pDesc->InputLayout.pInputElementDescs = m_InputLayouts.get();
		}
		inline void GraphicsPipelineStateDesc::InputAssemblerStage::SetIBStripCutValue(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE val)
		{
			m_pDesc->IBStripCutValue = val;
		}
		inline void GraphicsPipelineStateDesc::InputAssemblerStage::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type)
		{
			m_pDesc->PrimitiveTopologyType = type;
		}

		inline GraphicsPipelineStateDesc::RasterizerStage::RasterizerStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc) : m_pDesc(pDesc)
		{
			// Default value
			SetRasterizerState();
		}
		inline void GraphicsPipelineStateDesc::RasterizerStage::SetRasterizerState(const D3D12_RASTERIZER_DESC& desc)
		{
			m_pDesc->RasterizerState = desc;
		}

		inline GraphicsPipelineStateDesc::OutputMergerStage::OutputMergerStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc) : m_pDesc(pDesc)
		{
			// Default value
			SetBlendState();
			SetBlendSampleMask();
			SetDepthStencilState();
			SetRenderTargetFormat();
		}
		inline void GraphicsPipelineStateDesc::OutputMergerStage::SetBlendState(const D3D12_BLEND_DESC& desc)
		{
			m_pDesc->BlendState = desc;
		}
		inline void GraphicsPipelineStateDesc::OutputMergerStage::SetBlendSampleMask(UINT mask)
		{
			m_pDesc->SampleMask = mask;
		}
		inline void GraphicsPipelineStateDesc::OutputMergerStage::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& desc)
		{
			m_pDesc->DepthStencilState = desc;
		}
		inline void GraphicsPipelineStateDesc::OutputMergerStage::SetRenderTargetFormat(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
		{
			SetRenderTargetFormats(1, &rtvFormat, dsvFormat, msaaCount, msaaQuality);
		}
		inline void GraphicsPipelineStateDesc::OutputMergerStage::SetRenderTargetFormats(UINT numRTVs, const DXGI_FORMAT* pRtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
		{
			for (UINT i = 0; i < numRTVs; ++i)
				m_pDesc->RTVFormats[i] = pRtvFormats[i];

			for (UINT i = numRTVs; i < 8; ++i)
				m_pDesc->RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
			m_pDesc->NumRenderTargets = numRTVs;
			m_pDesc->DSVFormat = dsvFormat;
			m_pDesc->SampleDesc.Count = msaaCount;
			m_pDesc->SampleDesc.Quality = msaaQuality;
		}

		inline GraphicsPipelineStateDesc::StreamOutStage::StreamOutStage(D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc) : m_pDesc(pDesc)
		{
			// Default value
			D3D12_STREAM_OUTPUT_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			SetStreamOut(desc);
		}
		inline void GraphicsPipelineStateDesc::StreamOutStage::SetStreamOut(const D3D12_STREAM_OUTPUT_DESC &desc)
		{
			m_pDesc->StreamOutput = desc;
		}

		inline GraphicsPipelineStateDesc::ShaderStage::ShaderStage(D3D12_SHADER_BYTECODE *pShader) : m_pShader(pShader)
		{
			// Default value
			SetShader();
		}
		inline void GraphicsPipelineStateDesc::ShaderStage::SetShader(const void *pShaderBytecode, SIZE_T bytecodeLength)
		{
			m_pShader->pShaderBytecode = pShaderBytecode;
			m_pShader->BytecodeLength = bytecodeLength;
		}


		inline GraphicsPipelineStateDesc::GraphicsPipelineStateDesc() : IA(&m_Desc), RS(&m_Desc), OM(&m_Desc), SO(&m_Desc),
			VS(&m_Desc.VS), HS(&m_Desc.HS), DS(&m_Desc.DS),
			GS(&m_Desc.GS), PS(&m_Desc.PS)
		{
			// Set default value					
			SetFlag();
			SetNodeMask();
			SetRootSignature();
			SetCachedPSO();
		}

		inline void GraphicsPipelineStateDesc::SetNodeMask(UINT mask)
		{
			m_Desc.NodeMask = mask;
		}
		inline void GraphicsPipelineStateDesc::SetRootSignature(ID3D12RootSignature *pSig)
		{
			m_Desc.pRootSignature = pSig;
		}
		inline void GraphicsPipelineStateDesc::SetFlag(D3D12_PIPELINE_STATE_FLAGS flags)
		{
			m_Desc.Flags = flags;
		}
		inline void GraphicsPipelineStateDesc::SetCachedPSO(const void *pBlob, SIZE_T nSize)
		{
			m_Desc.CachedPSO.pCachedBlob = pBlob;
			m_Desc.CachedPSO.CachedBlobSizeInBytes = nSize;
		}

		inline GraphicsPipelineStateDesc::operator const D3D12_GRAPHICS_PIPELINE_STATE_DESC*() const
		{
			return &m_Desc;
		}

		inline ComputePipelineStateDesc::ComputePipelineStateDesc()
		{
			SetFlag();
			SetNodeMask();
			SetRootSignature();
			SetComputeShader();
			SetCachedPSO();
		}

		inline void ComputePipelineStateDesc::SetNodeMask(UINT mask)
		{
			m_Desc.NodeMask = mask;
		}
		inline void ComputePipelineStateDesc::SetRootSignature(ID3D12RootSignature *pSig)
		{
			m_Desc.pRootSignature = pSig;
		}
		inline void ComputePipelineStateDesc::SetFlag(D3D12_PIPELINE_STATE_FLAGS flags)
		{
			m_Desc.Flags = flags;
		}
		inline void ComputePipelineStateDesc::SetCachedPSO(const void *pBlob, SIZE_T nSize)
		{
			m_Desc.CachedPSO.pCachedBlob = pBlob;
			m_Desc.CachedPSO.CachedBlobSizeInBytes = nSize;
		}

		inline void ComputePipelineStateDesc::SetComputeShader(const void *pShaderBytecode, SIZE_T bytecodeLength)
		{
			m_Desc.CS.pShaderBytecode = pShaderBytecode;
			m_Desc.CS.BytecodeLength = bytecodeLength;
		}
		inline D3D12_COMPUTE_PIPELINE_STATE_DESC& ComputePipelineStateDesc::GetDesc() { return m_Desc; }
		inline const D3D12_COMPUTE_PIPELINE_STATE_DESC& ComputePipelineStateDesc::GetDesc() const { return m_Desc; }

		inline ComputePipelineStateDesc::operator const D3D12_COMPUTE_PIPELINE_STATE_DESC*() const
		{
			return &m_Desc;
		}

		inline Viewport::Viewport(
			float topLeftX, float topLeftY,
			float w, float h,
			float minDepth,
			float maxDepth)
		{
			TopLeftX = topLeftX;
			TopLeftY = topLeftY;
			Width = w;
			Height = h;
			MinDepth = minDepth;
			MaxDepth = maxDepth;
		}

		inline Viewport::operator const D3D12_VIEWPORT*() const
		{
			return this;
		}
#ifdef NTDDI_WIN10_RS5
		inline DispatchRaysDesc::DispatchRaysDesc(UINT width, UINT height, UINT depth)
		{
			ZeroMemory(this, sizeof(DispatchRaysDesc));
			Width = width;
			Height = height;
			Depth = depth;
		}

		inline void DispatchRaysDesc::SetRayGenerationShaderRecord(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes)
		{
			RayGenerationShaderRecord.StartAddress = startAddress;
			RayGenerationShaderRecord.SizeInBytes = sizeInBytes;
		}

		inline void DispatchRaysDesc::SetHitGroupTable(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes, UINT64 strideInBytes)
		{
			HitGroupTable.StartAddress = startAddress;
			HitGroupTable.SizeInBytes = sizeInBytes;
			HitGroupTable.StrideInBytes = strideInBytes;
		}

		inline void DispatchRaysDesc::SetMissShaderTable(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes, UINT64 strideInBytes)
		{
			MissShaderTable.StartAddress = startAddress;
			MissShaderTable.SizeInBytes = sizeInBytes;
			MissShaderTable.StrideInBytes = strideInBytes;
		}

		inline void DispatchRaysDesc::SetCallableShaderTable(D3D12_GPU_VIRTUAL_ADDRESS startAddress, UINT64 sizeInBytes, UINT64 strideInBytes)
		{
			CallableShaderTable.StartAddress = startAddress;
			CallableShaderTable.SizeInBytes = sizeInBytes;
			CallableShaderTable.StrideInBytes = strideInBytes;
		}

		
		inline StateObjectDesc::SubObjectBase::SubObjectBase(D3D12_STATE_SUBOBJECT_TYPE subObjectType, const void *pSubObjectDesc) :
			m_Type(subObjectType), m_SubObject({subObjectType, pSubObjectDesc}) {}


		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType, typename T>		
		inline StateObjectDesc::SubObject<SubObjectType, T>::SubObject() : SubObjectBase(SubObjectType, nullptr)
		{}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY>::type>::SubObject() : 
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY>::type>::SetLibrary(D3D12_SHADER_BYTECODE* pCode)
		{
			m_Desc.DXILLibrary.BytecodeLength = 0;
			m_Desc.DXILLibrary.pShaderBytecode = nullptr;
			if (pCode) m_Desc.DXILLibrary = *pCode;
		}


		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY>::type>::SetLibrary(const void *pShaderBytecode, SIZE_T BytecodeLength)
		{
			m_Desc.DXILLibrary.BytecodeLength = BytecodeLength;
			m_Desc.DXILLibrary.pShaderBytecode = pShaderBytecode;
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY>::type>::AddExport(
				LPCWSTR Name,
				LPCWSTR ExportToRename,
				D3D12_EXPORT_FLAGS Flags)
		{
			if (Name != nullptr)
			{
				D3D12_EXPORT_DESC curExport = {};
				m_ExportStrings.push_back(Name);
				curExport.Name = m_ExportStrings.back().c_str();
				if (ExportToRename != nullptr)
				{
					m_ExportStrings.push_back(ExportToRename);
					curExport.ExportToRename = m_ExportStrings.back().c_str();
				}
				curExport.Flags = Flags;
				m_Exports.push_back(curExport);
				m_Desc.pExports = m_Exports.data();
				m_Desc.NumExports = (UINT)m_Exports.size();
			}
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION>::type>::SetCollection(ID3D12StateObject* pExistingCollection)
		{
			m_Desc.pExistingCollection = pExistingCollection;
			m_pExistingCollection = pExistingCollection;
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION>::type>::AddExport(
				LPCWSTR Name,
				LPCWSTR ExportToRename,
				D3D12_EXPORT_FLAGS Flags)
		{
			if (Name != nullptr)
			{
				D3D12_EXPORT_DESC curExport = {};
				m_ExportStrings.push_back(Name);
				curExport.Name = m_ExportStrings.back().c_str();
				if (ExportToRename != nullptr)
				{
					m_ExportStrings.push_back(ExportToRename);
					curExport.ExportToRename = m_ExportStrings.back().c_str();
				}
				curExport.Flags = Flags;
				m_Exports.push_back(curExport);
				m_Desc.pExports = m_Exports.data();
				m_Desc.NumExports = (UINT)m_Exports.size();
			}
		}

			
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type>::SetSubobject(const D3D12_STATE_SUBOBJECT *pSubobjectToAssociate)
		{
			m_Desc.pSubobjectToAssociate = pSubobjectToAssociate;
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type>::AddExport(LPCWSTR Export)
		{
			if (Export != nullptr)
			{
				m_ExportStrings.push_back(Export);
				m_Exports.push_back(m_ExportStrings.back().c_str());
				m_Desc.NumExports = (UINT)m_Exports.size();
				m_Desc.pExports = m_Exports.data();
			}
		}
		
			
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type>::SetSubobject(LPCWSTR subobjectName)
		{
			m_Desc.SubobjectToAssociate = nullptr;
			if (subobjectName != nullptr)
			{
				m_sSubObjectName = m_Desc.SubobjectToAssociate;
				m_Desc.SubobjectToAssociate = m_sSubObjectName.c_str();
			}
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION>::type>::AddExport(LPCWSTR Export)
		{
			if (Export != nullptr)
			{
				m_ExportStrings.push_back(Export);
				m_Exports.push_back(m_ExportStrings.back().c_str());
				m_Desc.NumExports = (UINT)m_Exports.size();
				m_Desc.pExports = m_Exports.data();
			}
		}

			
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP>::type>::SetHitGroup(
				D3D12_HIT_GROUP_TYPE type,
				LPCWSTR strExportName,
				LPCWSTR strAnyHitShaderImport,
				LPCWSTR strClosestHitShaderImport,
				LPCWSTR strIntersectionShaderImport)
		{
			m_Desc.Type = type;
			m_Desc.HitGroupExport = nullptr;
			m_Desc.AnyHitShaderImport = nullptr;
			m_Desc.ClosestHitShaderImport = nullptr;
			m_Desc.IntersectionShaderImport = nullptr;

			if (strExportName != nullptr)
			{
				m_sHitGroupExport = strExportName;
				m_Desc.HitGroupExport = m_sHitGroupExport.c_str();
			}
			if (strAnyHitShaderImport != nullptr)
			{
				m_sAnyHitShaderImport = strAnyHitShaderImport;
				m_Desc.AnyHitShaderImport = m_sAnyHitShaderImport.c_str();
			}
			if (strClosestHitShaderImport != nullptr)
			{
				m_sClosestHitShaderImport = strClosestHitShaderImport;
				m_Desc.ClosestHitShaderImport = m_sClosestHitShaderImport.c_str();
			}
			if (strIntersectionShaderImport != nullptr)
			{
				m_sIntersectionShaderImport = strIntersectionShaderImport;
				m_Desc.IntersectionShaderImport = m_sIntersectionShaderImport.c_str();
			}

		}
		
			
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG>::type>::Config(UINT MaxPayloadSizeInBytes, UINT MaxAttributeSizeInBytes)
		{
			m_Desc.MaxPayloadSizeInBytes = MaxPayloadSizeInBytes;
			m_Desc.MaxAttributeSizeInBytes = MaxAttributeSizeInBytes;
		}
				
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG>::type>::Config(UINT MaxTraceRecursionDepth)
		{
			m_Desc.MaxTraceRecursionDepth = MaxTraceRecursionDepth;
		}

			
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE>::type>::SetRootSignature(ID3D12RootSignature* pRootSig)
		{
			m_Desc.pGlobalRootSignature = pRootSig;
			m_pRS = pRootSig;
		}

			
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE>::type>::SetRootSignature(ID3D12RootSignature* pRootSig)
		{
			m_Desc.pLocalRootSignature = pRootSig;
			m_pRS = pRootSig;
		}
	
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG>::type>::SetFlags(D3D12_STATE_OBJECT_FLAGS Flags)
		{
			m_Desc.Flags = Flags;
		}

			
		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK>::type>::SubObject() :
			SubObjectBase(SubObjectType, &m_Desc)
		{
			ZeroMemory(&m_Desc, sizeof(m_Desc));
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline void StateObjectDesc::SubObject<SubObjectType,
			typename std::enable_if<SubObjectType == D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK>::type>::SetNodeMask(UINT nodeMask)
		{
			m_Desc.NodeMask = nodeMask;
		}
			
		inline StateObjectDesc::StateObjectDesc(D3D12_STATE_OBJECT_TYPE type)
		{
			m_Desc.Type = type;
			m_Desc.pSubobjects = nullptr;
			m_Desc.NumSubobjects = 0;
		}

		template<D3D12_STATE_SUBOBJECT_TYPE SubObjectType>
		inline StateObjectDesc::SubObject<SubObjectType> *StateObjectDesc::CreateSubObject()
		{		
			std::unique_ptr<SubObject<SubObjectType>> upSubObject = std::make_unique<SubObject<SubObjectType>>();
			D3D12_STATE_SUBOBJECT subObject;
			subObject.Type = SubObjectType;
			subObject.pDesc = &upSubObject->m_Desc;
			m_SubObjectData.push_back(subObject);

			m_Desc.NumSubobjects = (UINT)m_SubObjectData.size();
			m_Desc.pSubobjects = m_SubObjectData.data();

			SubObject<SubObjectType> *pSubObject = upSubObject.get();		
			m_SubObjects.push_back(std::move(upSubObject));			
			return pSubObject;
		}

		inline StateObjectDesc::operator const D3D12_STATE_OBJECT_DESC*() const
		{
			return &m_Desc;
		}
		inline StateObjectDesc::operator const D3D12_STATE_OBJECT_DESC&() const
		{
			return m_Desc;
		}
#endif
	}
}



