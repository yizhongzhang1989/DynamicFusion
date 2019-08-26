#include "UndistortDepthIR_Common.fxh"

struct PS_OUT
{
	uint depth : SV_Target0;
	uint ir : SV_Target1;
};

PS_OUT PS_UndistortDepthIR(PS_IN input)
{
	PS_OUT ret = (PS_OUT)0;
	ret.depth = (uint)round(input.depthIR.x);
	ret.ir = (uint)round(input.depthIR.y);
	return ret;
}

