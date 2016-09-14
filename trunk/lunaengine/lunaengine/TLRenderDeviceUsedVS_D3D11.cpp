#include "stdafx.h"
#include "TLRenderDeviceUsedVS_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedVS_D3D11::TLRenderDeviceUsedVS_D3D11():TLIRenderDeviceUsedVS(),mD3DVS(NULL)
	{
	}

	TLRenderDeviceUsedVS_D3D11::~TLRenderDeviceUsedVS_D3D11()
	{
		if(mD3DVS)
		{
			mD3DVS->Release();
			mD3DVS = NULL;
		}
	}

	bool TLRenderDeviceUsedVS_D3D11::initWithD3DVS(ID3D11VertexShader* d3dShader)
	{
		if(mD3DVS)
			return false;
		if(!d3dShader)
			return false;
		mD3DVS = d3dShader;
		return true;
	}
}