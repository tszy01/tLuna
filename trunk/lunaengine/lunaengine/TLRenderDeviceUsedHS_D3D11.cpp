#include "TLRenderDeviceUsedHS_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedHS_D3D11::RenderDeviceUsedHS_D3D11():RenderDeviceUsedHS(),mD3DHS(0)
	{
	}

	RenderDeviceUsedHS_D3D11::~RenderDeviceUsedHS_D3D11()
	{
		if(mD3DHS)
		{
			mD3DHS->Release();
			mD3DHS = 0;
		}
	}

	TBOOL RenderDeviceUsedHS_D3D11::initWithD3DHS(ID3D11HullShader* d3dShader)
	{
		if(mD3DHS)
			return TFALSE;
		if(!d3dShader)
			return TFALSE;
		mD3DHS = d3dShader;
		return TTRUE;
	}
}