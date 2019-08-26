#pragma once

// igD3D11: Shallow wrappers of D3D11 components.

// Important note:
// Copy / assign behavior: Non-copyable, movable only.

// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

#include "igD3D11Device.h"
#include "igD3D11DeviceContext.h"

#include "igD3D11BlendState.h"
#include "igD3D11Buffer.h"
#include "igD3D11DepthStencilState.h"
#include "igD3D11InputLayout.h"
#include "igD3D11RasterizerState.h"
#include "igD3D11SamplerState.h"
#include "igD3D11Shader.h"
#include "igD3D11SwapChain.h"
#include "igD3D11Texture1D.h"
#include "igD3D11Texture2D.h"
#include "igD3D11Texture3D.h"
#include "igD3D11TextureCube.h"
#include "igD3D11Query.h"