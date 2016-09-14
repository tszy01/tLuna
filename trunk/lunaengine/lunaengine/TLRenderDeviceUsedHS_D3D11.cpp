#include "stdafx.h"
#include "TLRenderDeviceUsedHS_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedHS_D3D11::TLRenderDeviceUsedHS_D3D11():TLIRenderDeviceUsedHS(),mD3DHS(NULL)
	{
	}

	TLRenderDeviceUsedHS_D3D11::~TLRenderDeviceUsedHS_D3D11()
	{
		if(mD3DHS)
		{
			mD3DHS->Release();
			mD3DHS = NULL;
		}
	}

	bool TLRenderDeviceUsedHS_D3D11::initWithD3DHS(ID3D11HullShader* d3dShader)
	{
		if(mD3DHS)
			return false;
		if(!d3dShader)
			return false;
		mD3DHS = d3dShader;
		return true;
	}
}