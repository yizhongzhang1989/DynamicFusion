#pragma once

// Usage: igD3D11 Common Header
// Created: 2018.4.28
// Last Modified: 2018.4.28
// 2018.4.28: Initial Version, Re-organized from igD3D11 V3 (0.3.x).
// Version: 1.0.180428.1500

// TCHAR
#include <tchar.h>

// D3D
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// WRL
#include <wrl.h>

// shared_ptr
#include <memory>

// ComPtr Defs

namespace ig
{
	namespace D3D11
	{
		//using namespace DirectX;
		using Microsoft::WRL::ComPtr;
		//**************** ComPtr Typedefs ****************//
		// device
		typedef ComPtr<ID3D11Device>				DeviceComPtr;
		typedef ComPtr<ID3D11DeviceContext>			DeviceContextComPtr;

		// IA
		typedef ComPtr<ID3D11InputLayout>			InputLayoutComPtr;

		// resource
		typedef ComPtr<ID3D11Resource>				ResourceComPtr;
		typedef ComPtr<ID3D11Texture1D>				Texture1DComPtr;
		typedef ComPtr<ID3D11Texture2D>				Texture2DComPtr;
		typedef ComPtr<ID3D11Texture3D>				Texture3DComPtr;
		typedef ComPtr<ID3D11Texture2D>				TextureCubeComPtr;
		typedef ComPtr<ID3D11Buffer>				BufferComPtr;

		// resource view
		typedef ComPtr<ID3D11ShaderResourceView>	ShaderResourceViewComPtr;
		typedef ComPtr<ID3D11RenderTargetView>		RenderTargetViewComPtr;
		typedef ComPtr<ID3D11UnorderedAccessView>	UnorderedAccessViewComPtr;
		typedef ComPtr<ID3D11DepthStencilView>		DepthStencilViewComPtr;

		// shaders
		typedef ComPtr<ID3D11VertexShader>			VertexShaderComPtr;
		typedef ComPtr<ID3D11HullShader>			HullShaderComPtr;
		typedef ComPtr<ID3D11DomainShader>			DomainShaderComPtr;
		typedef ComPtr<ID3D11GeometryShader>		GeometryShaderComPtr;
		typedef ComPtr<ID3D11PixelShader>			PixelShaderComPtr;
		typedef ComPtr<ID3D11ComputeShader>			ComputeShaderComPtr;
		typedef ComPtr<ID3DBlob>					BlobComPtr;
		typedef ComPtr<ID3D11ClassLinkage>			ClassLinkageComPtr;

		// state
		typedef ComPtr<ID3D11SamplerState>			SamplerStateComPtr;
		typedef ComPtr<ID3D11RasterizerState>		RasterizerStateComPtr;
		typedef ComPtr<ID3D11DepthStencilState>		DepthStencilStateComPtr;
		typedef ComPtr<ID3D11BlendState>			BlendStateComPtr;

		// others
		typedef ComPtr<ID3D11Query>					QueryComPtr;
	}
}

#include "igD3DCommon.h"
#include "igDXGI.h"

