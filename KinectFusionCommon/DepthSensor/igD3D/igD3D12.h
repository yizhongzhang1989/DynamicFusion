#pragma once
// igD3D12: [Heavy] Wrappers of D3D12 components.

// Created: 2018.9.29
// 2018.9.29: Initial Version.
// 2018.10.8: Added igD3D12Sampler.h.
// 2019.6.3: Added DXR related stuffs.
// Version: 0.1.190603.1800

#include "igD3D12Defs.h"
#include "igD3D12Device.h"
#include "igD3D12RootSignature.h"
#include "igD3D12PipelineState.h"
#include "igD3D12Buffer.h"
#include "igD3D12Texture2D.h"
#include "igD3D12TextureCube.h"
#include "igD3D12SwapChain.h"
#include "igD3D12Sampler.h"
#ifdef NTDDI_WIN10_RS5
#include "igD3D12StateObject.h"
#include "igD3D12ShaderTable.h"
#include "igD3D12RayTracingAS.h"
#endif



