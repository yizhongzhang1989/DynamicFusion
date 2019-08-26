cbuffer cbParams : register(b0)
{
	uint g_nSrcWidth;
	uint g_nSrcHeight;
	uint g_nDstWidth;
	uint g_nDstHeight;
}

SamplerState g_Sampler : register(s0);

Texture2D<float2> g_texColorLookUp : register(t0);
Texture2D<float4> g_texColorSrc : register(t1);
RWTexture2D<float4> g_texColorDst : register(u0);

[numthreads(16, 16, 1)]
void CS_UndistortColor(uint3 DTid : SV_DispatchThreadID)
{
	if (DTid.x < g_nDstWidth && DTid.y < g_nDstHeight)
	{	
		float2 uv = g_texColorLookUp[int2(DTid.xy)].xy + float2(0.5f, 0.5f);
		uv /= float2((float)g_nSrcWidth, (float)g_nSrcHeight);
		g_texColorDst[int2(DTid.xy)] = g_texColorSrc.SampleLevel(g_Sampler, uv, 0);
	}
}
