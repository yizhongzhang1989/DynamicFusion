#define GROUP_SIZE 16

cbuffer cbParams : register(b0)
{
	uint g_nWidth;
	uint g_nHeight;
	bool g_bIsSRGB;
	float padding;
};

SamplerState g_Sampler : register(s0);
Texture2DArray<float4> g_texSrc : register(t0);
RWTexture2DArray<float4> g_texDst : register(u0);

float3 ToSRGB(float3 color)
{
	return color < 0.0031308f ? 12.92f * color : 1.055f * pow(abs(color), 1.0f / 2.4f) - 0.055f;
}

[numthreads(GROUP_SIZE, GROUP_SIZE, 1)]
void CS_MipMap(int3 DTid : SV_DispatchThreadID)
{
	if (DTid.x < (int)g_nWidth && DTid.y < (int)g_nHeight)
	{
		float2 uv = (DTid.xy + 0.5f) / float2((float)g_nWidth, (float)g_nHeight);
		float4 color = g_texSrc.SampleLevel(g_Sampler, float3(uv, 0), 0);
		if (g_bIsSRGB)
		{
			color.xyz = ToSRGB(color.xyz);
		}
		g_texDst[int3(DTid.xy, 0)] = color;
	}
}
