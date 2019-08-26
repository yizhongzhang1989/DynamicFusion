//#include "igCode.h"
#include "GPUUndistorter.h"
#include "UndistortColor_CS.h"
#include "UndistortDepthIR_VS.h"
#include "UndistortDepthIR_GS.h"
#include "UndistortDepthIR_PS.h"

HRESULT GPUUndistorter::InitD3D()
{
	// Create d3d11 device.
	if (FAILED(m_Device.Create(nullptr, D3D_DRIVER_TYPE_HARDWARE, D3D_FEATURE_LEVEL_11_0)))
	{
		IG_D3D_FAILED_RETURN(m_Device.Create(nullptr, D3D_DRIVER_TYPE_WARP, D3D_FEATURE_LEVEL_11_0), 
			L"GPUUndistorter::InitD3D: Failed to create D3D11 device.");
	}

	// For undistort color.
	{
		// Create shaders
		IG_D3D_FAILED_RETURN(m_UndistortColorCS.Create(m_Device, g_CS_UndistortColor, sizeof(g_CS_UndistortColor)),
			L"GPUUndistorter::InitD3D: Failed to create undistort color CS.");

		// Create constant buffers
		IG_D3D_FAILED_RETURN(m_UndistortColorCB.Create(m_Device, sizeof(m_UndistortColorParams), DXGI_FORMAT_UNKNOWN,
			D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, &m_UndistortColorParams),
			L"GPUUndistorter::InitD3D: Failed to create undistort color CB.");

		// Create textures
		IG_D3D_FAILED_RETURN(m_DstColorTex.Create(m_Device, frame_width, frame_height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DEFAULT),
			L"GPUUndistorter::InitD3D: Failed to create dst color texture.");

		IG_D3D_FAILED_RETURN(m_DstColorTex_CPU.Create(m_Device, frame_width, frame_height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM,
			0, D3D11_USAGE_STAGING),
			L"GPUUndistorter::InitD3D: Failed to create dst color texture(CPU).");

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = color_lookup_uv.data();
		initData.SysMemPitch = frame_width * sizeof(float) * 2;
		initData.SysMemSlicePitch = 0;
		IG_D3D_FAILED_RETURN(m_ColorLookupUVTex.Create(m_Device, frame_width, frame_height, 1, 1, DXGI_FORMAT_R32G32_FLOAT,
			D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, &initData),
			L"GPUUndistorter::InitD3D: Failed to create color lookup texture.");

		IG_D3D_ASSERT_SUCCEEDED(RecreateUndistortColorD3DResourcesOnDemand(2048, 1536));
	}
	// For undistort depth/IR.
	{

		IG_D3D_FAILED_RETURN(m_ResultDepthTex.Create(m_Device, frame_width, frame_height, 1, 1, DXGI_FORMAT_R16_UINT,
			D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT), L"GPUUndistorter::InitD3D: Failed to create result depth texture.");

		IG_D3D_FAILED_RETURN(m_ResultIRTex.Create(m_Device, frame_width, frame_height, 1, 1, DXGI_FORMAT_R16_UINT,
			D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT), L"GPUUndistorter::InitD3D: Failed to create result IR texture.");

		IG_D3D_FAILED_RETURN(m_ResultDepthIRTex_CPU.Create(m_Device, frame_width, frame_height, 1, 1, DXGI_FORMAT_R16_UINT,
			0, D3D11_USAGE_STAGING), L"GPUUndistorter::InitD3D: Failed to create result depthIR texture (CPU).");

		IG_D3D_FAILED_RETURN(m_ResultDepthIRTex_DS.Create(m_Device, frame_width, frame_height, 1, 1, DXGI_FORMAT_D32_FLOAT,
			D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT), L"GPUUndistorter::InitD3D: Failed to create result depthIR-DS texture.");

		IG_D3D_FAILED_RETURN(m_UndistortDepthIRVS.Create(m_Device, g_VS_UndistortDepthIR, sizeof(g_VS_UndistortDepthIR)),
			L"GPUUndistorter::InitD3D: Failed to create undistort depthIR VS.");
		IG_D3D_FAILED_RETURN(m_UndistortDepthIRGS.Create(m_Device, g_GS_UndistortDepthIR, sizeof(g_GS_UndistortDepthIR)),
			L"GPUUndistorter::InitD3D: Failed to create undistort depthIR GS.");
		IG_D3D_FAILED_RETURN(m_UndistortDepthIRPS.Create(m_Device, g_PS_UndistortDepthIR, sizeof(g_PS_UndistortDepthIR)),
			L"GPUUndistorter::InitD3D: Failed to create undistort depthIR PS.");

		IG_D3D_FAILED_RETURN(m_UndistortDepthIRCB.Create(m_Device, sizeof(m_UndistortDepthIRParams), DXGI_FORMAT_UNKNOWN,
			D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, &m_UndistortDepthIRParams),
			L"GPUUndistorter::InitD3D: Failed to create undistort depthIR CB.");
		

		D3D11_INPUT_ELEMENT_DESC PCLayout[] = { { "TEXCOORD", 0, DXGI_FORMAT_R32G32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } };	
		IG_D3D_FAILED_RETURN(m_UndistortDepthIRIA.Create(m_Device, PCLayout, 1, m_UndistortDepthIRVS), L"GPUUndistorter::InitD3D: Failed to create IA.");
		
		IG_D3D_ASSERT_SUCCEEDED(RecreateUndistortDepthIRD3DResourcesOnDemand(640, 576));
	}


	return S_OK;
}

HRESULT GPUUndistorter::RecreateUndistortColorD3DResourcesOnDemand(uint32_t rawWidth, uint32_t rawHeight)
{
	if (!m_SrcColorTex.IsValid() ||
		m_SrcColorTex.Desc().Width != rawWidth ||
		m_SrcColorTex.Desc().Height != rawHeight)
	{
		// Need to create texture.
		IG_D3D_ASSERT_SUCCEEDED(m_SrcColorTex.Create(m_Device, rawWidth, rawHeight, 1, 1,
			DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
			L"GPUUndistorter::RecreateUndistortColorD3DResourcesOnDemand: failed to create src color tex.");
	}
	return S_OK;
}

HRESULT GPUUndistorter::RecreateUndistortDepthIRD3DResourcesOnDemand(uint32_t rawWidth, uint32_t rawHeight)
{
	if (rawWidth == 0 || rawHeight == 0)
		return E_INVALIDARG;

	if (rawWidth == m_nRawWidth && rawHeight == m_nRawHeight)
		return S_OK;
	// Recreate VB and IB
	std::vector<DirectX::XMUINT2> tex;
	std::vector<uint32_t> idxBuf;
	tex.resize(rawWidth * rawHeight);
	for (uint32_t y = 0; y < rawHeight; ++y)
	{
		for (uint32_t x = 0; x < rawWidth; ++x)
		{
			tex[x + y * rawWidth] = DirectX::XMUINT2(x, y);
		}
	}
	idxBuf.resize((rawWidth - 1) * (rawHeight - 1) * 6);
	for (uint32_t y = 0; y < rawHeight - 1; ++y)
	{
		for (uint32_t x = 0; x < rawWidth - 1; ++x)
		{
			uint32_t baseIdx = (x + y * (rawWidth - 1)) * 6;
			uint32_t v0 = x + y * rawWidth;
			uint32_t v1 = x + 1 + y * rawWidth;
			uint32_t v2 = x + 1 + (y + 1) * rawWidth;
			uint32_t v3 = x + (y + 1) * rawWidth;
			idxBuf[baseIdx] = v0;
			idxBuf[baseIdx + 1] = v1;
			idxBuf[baseIdx + 2] = v2;
			idxBuf[baseIdx + 3] = v2;
			idxBuf[baseIdx + 4] = v3;
			idxBuf[baseIdx + 5] = v0;
		}
	}
	IG_D3D_FAILED_RETURN(m_PointCloud2MeshVB.Create(m_Device, sizeof(DirectX::XMUINT2) * rawWidth * rawHeight,
		DXGI_FORMAT_UNKNOWN, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, tex.data()),
		L"GPUUndistorter::RecreateUndistortDepthD3DResourcesOnDemand: Failed to create VB.");
	IG_D3D_FAILED_RETURN(m_PointCloud2MeshIB.Create(m_Device, sizeof(uint32_t) *(rawWidth - 1) * (rawHeight - 1) * 6, 
		DXGI_FORMAT_R32_UINT, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, idxBuf.data()),
		L"GPUUndistorter::RecreateUndistortDepthD3DResourcesOnDemand: Failed to create IB.");

	// Need to create texture and buffer
	IG_D3D_FAILED_RETURN(m_RawDepthTex.Create(m_Device, rawWidth, rawHeight, 1, 1,
		DXGI_FORMAT_R16_SINT, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
		L"GPUUndistorter::RecreateUndistortDepthD3DResourcesOnDemand: failed to create raw-depth tex.");
	IG_D3D_FAILED_RETURN(m_RawIRTex.Create(m_Device, rawWidth, rawHeight, 1, 1,
		DXGI_FORMAT_R16_SINT, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
		L"GPUUndistorter::RecreateUndistortDepthD3DResourcesOnDemand: failed to create raw-ir tex.");
	IG_D3D_FAILED_RETURN(m_PointCloudBuffer.Create(m_Device, sizeof(short) * 3 * rawWidth * rawHeight,
		DXGI_FORMAT_R16_SINT, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC),
		L"GPUUndistorter::RecreateUndistortDepthD3DResourcesOnDemand: failed to create point-cloud buffer.");
	m_nRawWidth = rawWidth;
	m_nRawHeight = rawHeight;
	return S_OK;
}

void GPUUndistorter::SetupUndistorter(
	float fov,
	int width,
	int height,
	float* color_lookup_uv_ptr,
	float rot[],
	float trans[],
	float normal_view_angle_threshold)
{
	h_fov = fov;
	frame_width = width;
	frame_height = height;
	n_v_angle_threshold = normal_view_angle_threshold;
	color_lookup_uv.resize(width*height * 2);
	color_lookup_uv.assign(color_lookup_uv_ptr, color_lookup_uv_ptr + color_lookup_uv.size());

	for (int i = 0; i < 9; i++)
		R[i] = rot[i];
	for (int i = 0; i < 3; i++)
		T[i] = trans[i];

	// Init D3D stups
	IG_D3D_ASSERT_SUCCEEDED(InitD3D(), L"GPUUndistorter::SetupUndistorter: Failed to init D3D.");
	
}

void GPUUndistorter::UndistortColor(
	int* undistort_bgrx_ptr,
	int raw_color_width,
	int raw_color_height,
	int raw_color_stride,
	const char* raw_color_bgrx_ptr) 
{
	IG_D3D_ASSERT_SUCCEEDED(RecreateUndistortColorD3DResourcesOnDemand(raw_color_width, raw_color_height));

	// Upload src color texture data.
	m_SrcColorTex.Upload(0, 0, 0, 0, raw_color_width, raw_color_height, raw_color_bgrx_ptr, raw_color_stride);

	auto pContext = &m_Device.ImmediateContext();

	// Set shader
	pContext->CS.SetShader(m_UndistortColorCS);
	// Set contant buffer
	m_UndistortColorParams.srcWidth = raw_color_width;
	m_UndistortColorParams.srcHeight = raw_color_height;
	m_UndistortColorParams.dstWidth = frame_width;
	m_UndistortColorParams.dstHeight = frame_height;
	m_UndistortColorCB.Upload(0, sizeof(m_UndistortColorParams), &m_UndistortColorParams);
	pContext->CS.SetConstantBuffer(0, m_UndistortColorCB);
	
	// Set sampler and textures
	pContext->CS.SetLinearClampSampler(0);
	pContext->CS.SetShaderResource(0, m_ColorLookupUVTex.SRV());
	pContext->CS.SetShaderResource(1, m_SrcColorTex.SRV());
	pContext->CS.SetUnorderedAccessView(0, m_DstColorTex.UAV());
	
	// Dispatch
	UINT gx = (frame_width + 15) / 16;
	UINT gy = (frame_height + 15) / 16;
	pContext->Dispatch(gx, gy, 1);
	// Clear
	pContext->CS.SetShaderResource(0, nullptr);
	pContext->CS.SetShaderResource(1, nullptr);
	pContext->CS.SetUnorderedAccessView(0, nullptr);

	//ig::D3D11::SaveTexture2DToFile(m_DstColorTex, 0, 0, L"D:\\save_texture.pfm");

	// Read-back data v2
	m_DstColorTex_CPU.CopyFrom(m_DstColorTex);
	void *pData = nullptr;
	UINT rowPitch = 0;
	IG_D3D_ASSERT_SUCCEEDED(m_DstColorTex_CPU.Map(0, 0, D3D11_MAP_READ, &pData, &rowPitch),
		L"GPUUndistorter::UndistortColor: failed to map result data.");

	/*ig::CImage_4c8u resultTexData;
	resultTexData.Create(frame_width, frame_height, rowPitch, pData);
	ig::CImage_4c8u dstImg;
	dstImg.Create(frame_width, frame_height, frame_width * sizeof(int), (void*)undistort_bgrx_ptr);
	dstImg.CopyFrom(resultTexData);*/

	for (int32_t y = 0; y < frame_height; y++)
	{
		void *pDstData = (char*)undistort_bgrx_ptr + y * frame_width * sizeof(int);
		const void *pSrcData = (const char*)pData + y * rowPitch;
		memcpy_s(pDstData, frame_width * sizeof(int), pSrcData, frame_width * sizeof(int));
	}

	m_DstColorTex_CPU.Unmap(0, 0);
}

void GPUUndistorter::UndistortDepthIR(
	unsigned short*      undistort_depth_ptr,
	unsigned short*      undistort_ir_ptr,
	int                        raw_depth_width,
	int                        raw_depth_height,
	int                        raw_depth_stride,
	int                        raw_ir_stride,
	const char*          raw_depth_ptr,
	const char*          raw_ir_ptr,
	const short*  point_cloud_depth_ptr) 
{
	IG_D3D_ASSERT_SUCCEEDED(RecreateUndistortDepthIRD3DResourcesOnDemand(raw_depth_width, raw_depth_height));

	// Upload data.
	m_RawDepthTex.Upload(0, 0, 0, 0, raw_depth_width, raw_depth_height, raw_depth_ptr, raw_depth_stride);
	m_RawIRTex.Upload(0, 0, 0, 0, raw_depth_width, raw_depth_height, raw_ir_ptr, raw_ir_stride);
	m_PointCloudBuffer.Upload(0, sizeof(short) * 3 * raw_depth_width * raw_depth_height, point_cloud_depth_ptr);

	// Update constant buffer.
	const float DegreesToRadians = 3.14159265359f / 180.0f;
	float fov = h_fov;
	float xyScale = tanf(fov * DegreesToRadians * 0.5f) / (frame_width * 0.5f);
	float inv_xyScale = 1.0f / xyScale;
	float NoVThreshold = cos(n_v_angle_threshold * DegreesToRadians);

	m_UndistortDepthIRParams.fInvXyScale = inv_xyScale;
	m_UndistortDepthIRParams.nWidth = frame_width;
	m_UndistortDepthIRParams.nHeight = frame_height;
	m_UndistortDepthIRParams.matDepth2Color = { R[0], R[1], R[2], T[0],R[3], R[4], R[5], T[1], R[6], R[7], R[8], T[2],0, 0, 0, 1 };
	
	// Setup pipeline.
	auto *pContext = &m_Device.ImmediateContext();
	pContext->ClearDepthStencil(m_ResultDepthIRTex_DS.DSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
	// IA
	pContext->IA.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IA.SetVertexBuffer(0, m_PointCloud2MeshVB, sizeof(DirectX::XMUINT2), 0);
	pContext->IA.SetIndexBuffer(m_PointCloud2MeshIB);
	pContext->IA.SetInputLayout(m_UndistortDepthIRIA);

	// RS
	CD3D11_VIEWPORT vp(0.f, 0.f, (float)frame_width, (float)frame_height, 0.f, 1.f);
	pContext->RS.SetViewport(vp);
	//pContext->RS.SetRasterizerTwoSided();
	pContext->RS.SetRasterizerDefault();
	// OM
	pContext->OM.DisableBlend();

	// RTVs
	ID3D11RenderTargetView* pRTVs[2] = { m_ResultDepthTex.RTV(), m_ResultIRTex.RTV() };
	pContext->OM.SetRenderTargets(2, pRTVs, m_ResultDepthIRTex_DS.DSV());

	// Shaders
	pContext->VS.SetConstantBuffer(0, m_UndistortDepthIRCB);
	pContext->GS.SetConstantBuffer(0, m_UndistortDepthIRCB);
	pContext->PS.SetConstantBuffer(0, m_UndistortDepthIRCB);
	pContext->VS.SetShaderResource(0, m_PointCloudBuffer.SRV());
	pContext->GS.SetShaderResource(0, m_PointCloudBuffer.SRV());
	pContext->PS.SetShaderResource(0, m_PointCloudBuffer.SRV());
	pContext->VS.SetShaderResource(1, m_RawDepthTex.SRV());
	pContext->GS.SetShaderResource(1, m_RawDepthTex.SRV());
	pContext->PS.SetShaderResource(1, m_RawDepthTex.SRV());
	pContext->VS.SetShaderResource(2, m_RawIRTex.SRV());
	pContext->GS.SetShaderResource(2, m_RawIRTex.SRV());
	pContext->PS.SetShaderResource(2, m_RawIRTex.SRV());
	pContext->VS.SetShader(m_UndistortDepthIRVS);
	pContext->GS.SetShader(m_UndistortDepthIRGS);
	pContext->PS.SetShader(m_UndistortDepthIRPS);
	pContext->HS.Disable();
	pContext->DS.Disable();
	pContext->CS.Disable();

	// 2 render passes:
	//#1: Set novthreshold to zero, enable depth-write
	//#2 Set novthreshold to desired value, set depth to read-only
	auto RenderPass = [&](float novThreshold, bool bEnableDepthWrite)->void
	{
		m_UndistortDepthIRParams.fNoVThreshold = novThreshold;
		m_UndistortDepthIRCB.Upload(0, sizeof(m_UndistortDepthIRParams), &m_UndistortDepthIRParams);
		
		if (bEnableDepthWrite)
		{
			pContext->OM.SetDepthStencilDefault();
		}
		else
		{
			pContext->OM.SetDepthStencilReadOnly();
		}
		pContext->ClearRenderTarget(m_ResultDepthTex.RTV(), 0.f, 0.f, 0.f, 0.f);
		pContext->ClearRenderTarget(m_ResultIRTex.RTV(), 0.f, 0.f, 0.f, 0.f);
		// Render
		pContext->DrawIndexed((m_nRawWidth - 1) * (m_nRawHeight - 1) * 6, 0, 0);
	};

	RenderPass(0, true);
	RenderPass(NoVThreshold, false);

	// Unbind RTVs
	pContext->VS.SetShaderResource(0, nullptr);
	pContext->GS.SetShaderResource(0, nullptr);
	pContext->PS.SetShaderResource(0, nullptr);

	pContext->VS.SetShaderResource(1, nullptr);
	pContext->GS.SetShaderResource(1, nullptr);
	pContext->PS.SetShaderResource(1, nullptr);

	pContext->VS.SetShaderResource(2, nullptr);
	pContext->GS.SetShaderResource(2, nullptr);
	pContext->PS.SetShaderResource(2, nullptr);
	pContext->OM.SetRenderTarget(nullptr, nullptr);

	// Read-back data v2
	{
		m_ResultDepthIRTex_CPU.CopyFrom(m_ResultDepthTex);
		void *pData = nullptr;
		UINT rowPitch = 0;
		IG_D3D_ASSERT_SUCCEEDED(m_ResultDepthIRTex_CPU.Map(0, 0, D3D11_MAP_READ, &pData, &rowPitch),
			L"GPUUndistorter::UndistortDepthIR: failed to map result depth data.");

		/*ig::CImage<ig::CVec<uint16_t, 1>> resultTexData;
		resultTexData.Create(frame_width, frame_height, rowPitch, pData);
		ig::CImage<ig::CVec<uint16_t, 1>> dstImg;
		dstImg.Create(frame_width, frame_height, frame_width * sizeof(uint16_t), (void*)undistort_depth_ptr);
		dstImg.CopyFrom(resultTexData);*/

		for (int32_t y = 0; y < frame_height; y++)
		{
			void *pDstData = (char*)undistort_depth_ptr + y * frame_width * sizeof(uint16_t);
			const void *pSrcData = (const char*)pData + y * rowPitch;
			memcpy_s(pDstData, frame_width * sizeof(uint16_t), pSrcData, frame_width * sizeof(uint16_t));
		}

		m_ResultDepthIRTex_CPU.Unmap(0, 0);
	}
	{
		m_ResultDepthIRTex_CPU.CopyFrom(m_ResultIRTex);
		void *pData = nullptr;
		UINT rowPitch = 0;
		IG_D3D_ASSERT_SUCCEEDED(m_ResultDepthIRTex_CPU.Map(0, 0, D3D11_MAP_READ, &pData, &rowPitch),
			L"GPUUndistorter::UndistortDepthIR: failed to map result IR data.");

		/*ig::CImage<ig::CVec<uint16_t, 1>> resultTexData;
		resultTexData.Create(frame_width, frame_height, rowPitch, pData);
		ig::CImage<ig::CVec<uint16_t, 1>> dstImg;
		dstImg.Create(frame_width, frame_height, frame_width * sizeof(uint16_t), (void*)undistort_ir_ptr);
		dstImg.CopyFrom(resultTexData);*/

		for (int32_t y = 0; y < frame_height; y++)
		{
			void *pDstData = (char*)undistort_ir_ptr + y * frame_width * sizeof(uint16_t);
			const void *pSrcData = (const char*)pData + y * rowPitch;
			memcpy_s(pDstData, frame_width * sizeof(uint16_t), pSrcData, frame_width * sizeof(uint16_t));
		}
		m_ResultDepthIRTex_CPU.Unmap(0, 0);
	}
}


void GPUUndistorter::DebugInit() 
{
	h_fov = 80;
	frame_width = 640;
	frame_height = 480;

	color_lookup_uv.resize(frame_width*frame_height * 2);
	for (int j = 0; j < frame_height; j++) 
	{
		for (int i = 0; i < frame_width; i++) 
		{
			color_lookup_uv[(j*frame_width + i) * 2] = (float)i;
			color_lookup_uv[(j*frame_width + i) * 2 + 1] = (float)j;
		}
	}

	R[0] = 1.0;
	R[1] = 0.0;
	R[2] = 0.0;
	R[3] = 0.0;
	R[4] = 1.0;
	R[5] = 0.0;
	R[6] = 0.0;
	R[7] = 0.0;
	R[8] = 1.0;
	T[0] = 0.0;
	T[1] = 0.0;
	T[2] = 0.0;

	// Init D3D stups
	IG_D3D_ASSERT_SUCCEEDED(InitD3D(), L"GPUUndistorter::SetupUndistorter: Failed to init D3D.");
}




