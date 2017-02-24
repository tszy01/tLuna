#include "TLRenderDeviceUsedCS_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedCS_D3D11::RenderDeviceUsedCS_D3D11():RenderDeviceUsedCS(),mD3DCS(0)
	{
	}

	RenderDeviceUsedCS_D3D11::~RenderDeviceUsedCS_D3D11()
	{
		if(mD3DCS)
		{
			mD3DCS->Release();
			mD3DCS = 0;
		}
	}

	TSun::TBOOL RenderDeviceUsedCS_D3D11::initWithD3DCS(ID3D11ComputeShader* d3dShader)
	{
		if(mD3DCS)
			return TSun::TFALSE;
		if(!d3dShader)
			return TSun::TFALSE;
		mD3DCS = d3dShader;
		return TSun::TTRUE;
	}
}