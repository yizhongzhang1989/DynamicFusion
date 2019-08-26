cbuffer cbParams : register(b0)
{
	matrix g_matDepth2Color;
	float g_fInvXyScale;
	uint g_nWidth;
	uint g_nHeight;
	float g_fZn;
	float g_fZf;
	float g_fNoVThreshold;
	float2 padding;
}

Buffer<int> g_bufPointCloud : register(t0);
Texture2D<int> g_texDepth : register(t1);
Texture2D<int> g_texIr : register(t2);

struct VS_IN
{
	uint2 tex : TEXCOORD0;
	uint vId : SV_VertexID;
};

struct GS_IN
{
	float3 vPos : VPOS;
	float3 pos : PROJPOS;
	uint2 tex : TEXCOORD0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 depthIR : TEXCOORD0;
};
