#include "stdafx.h"
#include "TLRenderDeviceUsedCS_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedCS_D3D11::TLRenderDeviceUsedCS_D3D11():TLIRenderDeviceUsedCS(),mD3DCS(NULL)
	{
	}

	TLRenderDeviceUsedCS_D3D11::~TLRenderDeviceUsedCS_D3D11()
	{
		if(mD3DCS)
		{
			mD3DCS->Release();
			mD3DCS = NULL;
		}
	}

	bool TLRenderDeviceUsedCS_D3D11::initWithD3DCS(ID3D11ComputeShader* d3dShader)
	{
		if(mD3DCS)
			return false;
		if(!d3dShader)
			return false;
		mD3DCS = d3dShader;
		return true;
	}
}