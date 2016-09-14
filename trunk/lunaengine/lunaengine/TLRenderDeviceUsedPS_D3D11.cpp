#include "stdafx.h"
#include "TLRenderDeviceUsedPS_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedPS_D3D11::TLRenderDeviceUsedPS_D3D11():TLIRenderDeviceUsedPS(),mD3DPS(NULL)
	{
	}

	TLRenderDeviceUsedPS_D3D11::~TLRenderDeviceUsedPS_D3D11()
	{
		if(mD3DPS)
		{
			mD3DPS->Release();
			mD3DPS = NULL;
		}
	}

	bool TLRenderDeviceUsedPS_D3D11::initWithD3DPS(ID3D11PixelShader* d3dShader)
	{
		if(mD3DPS)
			return false;
		if(!d3dShader)
			return false;
		mD3DPS = d3dShader;
		return true;
	}
}