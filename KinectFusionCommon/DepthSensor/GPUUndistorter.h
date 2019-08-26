#pragma once
#include <vector>
#include "igD3D/igD3D.h"
#include <DirectXMath.h>
class GPUUndistorter 
{
public:
	void SetupUndistorter(
		float fov,
		int width,
		int height,
		float* color_lookup_uv_ptr,
		float  rot[],
		float  trans[],
		float normal_view_angle_threshold = 85.f);

	void UndistortColor(
		int* undistort_bgrx_ptr,
		int raw_color_width,
		int raw_color_height,
		int raw_color_stride,
		const char* raw_color_bgrx_ptr);

	void UndistortDepthIR(
		unsigned short* undistort_depth_ptr,
		unsigned short* undistort_ir_ptr,
		int raw_depth_width,
		int raw_depth_height,
		int raw_depth_stride,
		int raw_ir_stride,
		const char* raw_depth_ptr,
		const char*  raw_ir_ptr,
		const short* point_cloud_depth_ptr);

	void DebugInit();

private:
	HRESULT InitD3D();
	HRESULT RecreateUndistortColorD3DResourcesOnDemand(uint32_t rawWidth, uint32_t rawHeight);
	HRESULT RecreateUndistortDepthIRD3DResourcesOnDemand(uint32_t rawWidth, uint32_t rawHeight);

private:
	ig::D3D11::CDevice m_Device;

	// For undistort color
	ig::D3D11::CTexture2D m_ColorLookupUVTex;
	ig::D3D11::CTexture2D m_SrcColorTex;
	ig::D3D11::CTexture2D m_DstColorTex;
	ig::D3D11::CTexture2D m_DstColorTex_CPU;
	ig::D3D11::CComputeShader m_UndistortColorCS;
	struct UndistortColorParams
	{
		uint32_t srcWidth;
		uint32_t srcHeight;
		uint32_t dstWidth;
		uint32_t dstHeight;
	} m_UndistortColorParams;
	ig::D3D11::CBuffer m_UndistortColorCB;

	// For undistort depth/ir (point based)
	ig::D3D11::CTexture2D m_RawDepthTex;
	ig::D3D11::CTexture2D m_RawIRTex;
	ig::D3D11::CBuffer m_PointCloudBuffer;
	ig::D3D11::CBuffer m_PointCloud2MeshVB;
	ig::D3D11::CBuffer m_PointCloud2MeshIB;
	uint32_t m_nRawWidth = 0;
	uint32_t m_nRawHeight = 0;

	ig::D3D11::CInputLayout m_UndistortDepthIRIA;
	ig::D3D11::CVertexShader m_UndistortDepthIRVS;
	ig::D3D11::CGeometryShader m_UndistortDepthIRGS;
	ig::D3D11::CPixelShader m_UndistortDepthIRPS;
	ig::D3D11::CTexture2D m_ResultDepthTex;
	ig::D3D11::CTexture2D m_ResultIRTex;
	ig::D3D11::CTexture2D m_ResultDepthIRTex_DS;
	ig::D3D11::CTexture2D m_ResultDepthIRTex_CPU;
	struct UndistortDepthIRParams
	{
		DirectX::XMFLOAT4X4 matDepth2Color;
		float fInvXyScale;
		uint32_t nWidth;
		uint32_t nHeight;
		float fZn;
		float fZf;
		float fNoVThreshold;
		DirectX::XMFLOAT2 padding;

		UndistortDepthIRParams()
		{
			DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
			DirectX::XMStoreFloat4x4(&matDepth2Color, identity);
			fInvXyScale = 1.f;
			nWidth = 640;
			nHeight = 480;
			fZn = 100.f;
			fZf = 10000.f;
			fNoVThreshold = 0;
		}
	} m_UndistortDepthIRParams;
	ig::D3D11::CBuffer m_UndistortDepthIRCB;

public:
	float h_fov;
	float n_v_angle_threshold;
	int	frame_width, frame_height;
	std::vector<float> color_lookup_uv;
	float R[9], T[3];

};



