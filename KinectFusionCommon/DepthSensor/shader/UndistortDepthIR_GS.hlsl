#include "UndistortDepthIR_Common.fxh"

[maxvertexcount(3)]
void GS_UndistortDepthIR(triangle GS_IN tri[3], inout TriangleStream<PS_IN> triStream)
{
	float2 depthIR[3];
	uint i;

	// discard triangles with N dot V < threshold
	float3 e0 = tri[1].vPos - tri[0].vPos;
	float3 e1 = tri[2].vPos - tri[0].vPos;
	float3 nrm = normalize(cross(e0, e1)); // triangle normal
	float3 v = normalize(float3(0, 0, 0) - tri[0].vPos); // view direction
	if (abs(dot(v, nrm)) < g_fNoVThreshold)
		return;

	// discard triangles with invalid depth
	for (i = 0; i < 3; ++i)
	{
		float depth = (float)g_texDepth.Load(int3(tri[i].tex, 0)).x;
		if (depth == 0)
			return;
		float ir = (float)g_texIr.Load(int3(tri[i].tex, 0)).x;
		depthIR[i] = float2(depth, ir);
	}

	// emmit triangles
	for (i = 0; i < 3; ++i)
	{
		PS_IN output = (PS_IN)0;
		output.pos = float4(tri[i].pos, 1);
		output.depthIR = depthIR[i];
		triStream.Append(output);
	}
}
