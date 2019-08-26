#pragma once
#include "igD3D12Defs.h"

namespace ig
{
	namespace D3D12
	{
		class CResource;
		class CComputeCommandContext;

		class MipMapGen
		{
			friend class CComputeCommandContext;
		private:
			MipMapGen(ID3D12Device *pDevice, UINT nodeMask = 0);
			void Run(CComputeCommandContext *pComputeContext, CResource *pTex);
			
		private:
			DeviceComPtr m_pDevice;
			RootSignatureComPtr m_pRootSignature;
			PipelineStateComPtr m_pPSO;
		};
	}
}
