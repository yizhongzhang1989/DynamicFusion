#include "UndistortDepthIR_Common.fxh"

GS_IN VS_UndistortDepthIR(VS_IN input)
{
	GS_IN output = (GS_IN)0;
	float x = (float)g_bufPointCloud[input.vId * 3 + 0];
	float y = (float)g_bufPointCloud[input.vId * 3 + 1];
	float z = (float)g_bufPointCloud[input.vId * 3 + 2];
	// color space position
	float3 posColor = mul(float4(x, y, z, 1), g_matDepth2Color).xyz;

	float u = g_fInvXyScale * posColor.x / posColor.z;
	float v = g_fInvXyScale * posColor.y / posColor.z;
	float pz = (posColor.z - g_fZn) * g_fZf / ((g_fZf - g_fZn) * posColor.z);
	
	output.vPos = float3(x, y, z);
	output.pos.x = 2.0 * u / (float)(g_nWidth);
	output.pos.y = -2.0 * v / (float)(g_nHeight);
	output.pos.z = pz;
	output.tex = input.tex;
	return output;
}
