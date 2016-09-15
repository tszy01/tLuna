#include "TLRenderDeviceUsedPS_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedPS_D3D11::RenderDeviceUsedPS_D3D11():RenderDeviceUsedPS(),mD3DPS(0)
	{
	}

	RenderDeviceUsedPS_D3D11::~RenderDeviceUsedPS_D3D11()
	{
		if(mD3DPS)
		{
			mD3DPS->Release();
			mD3DPS = 0;
		}
	}

	bool RenderDeviceUsedPS_D3D11::initWithD3DPS(ID3D11PixelShader* d3dShader)
	{
		if(mD3DPS)
			return false;
		if(!d3dShader)
			return false;
		mD3DPS = d3dShader;
		return true;
	}
}