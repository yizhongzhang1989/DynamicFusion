#include "igD3D11DeviceContext.h"

using namespace ig::D3D11;

template class CDeviceContext::ShaderStage<ID3D11VertexShader>;
template class CDeviceContext::ShaderStage<ID3D11HullShader>;
template class CDeviceContext::ShaderStage<ID3D11DomainShader>;
template class CDeviceContext::ShaderStage<ID3D11GeometryShader>;
template class CDeviceContext::ShaderStage<ID3D11PixelShader>;
template class CDeviceContext::ShaderStage<ID3D11ComputeShader>;

void CDeviceContext::Create(ID3D11Device *pDevice, ID3D11DeviceContext *pDC)
{
	m_pDevice = pDevice; m_pDeviceContext = pDC;
	IA.Create(pDevice, pDC);
	RS.Create(pDevice, pDC);
	OM.Create(pDevice, pDC);
	VS.Create(pDevice, pDC);
	HS.Create(pDevice, pDC);
	DS.Create(pDevice, pDC);
	GS.Create(pDevice, pDC);
	PS.Create(pDevice, pDC);
	CS.Create(pDevice, pDC);
}
void CDeviceContext::Destroy()
{
	m_pDevice = nullptr; m_pDeviceContext = nullptr;
	IA.Destroy();
	RS.Destroy();
	OM.Destroy();
	VS.Destroy();
	HS.Destroy();
	DS.Destroy();
	GS.Destroy();
	PS.Destroy();
	CS.Destroy();
}