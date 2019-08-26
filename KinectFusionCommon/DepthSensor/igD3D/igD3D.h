#pragma once

// igD3D: Shallow wrappers of DirectX components.

#include "igDXGI.h"
#include "igD3D11.h"
#include "igD3D12.h"

#include "igD2D1.h"

#ifdef _DEBUG
#pragma comment(lib, "igD3Dd.lib")
#else
#pragma comment(lib, "igD3D.lib")
#endif
