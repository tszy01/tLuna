#include "stdafx.h"
#include "TLRenderDeviceUsedGS_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedGS_D3D11::TLRenderDeviceUsedGS_D3D11():TLIRenderDeviceUsedGS(),mD3DGS(NULL)
	{
	}

	TLRenderDeviceUsedGS_D3D11::~TLRenderDeviceUsedGS_D3D11()
	{
		if(mD3DGS)
		{
			mD3DGS->Release();
			mD3DGS = NULL;
		}
	}

	bool TLRenderDeviceUsedGS_D3D11::initWithD3DGS(ID3D11GeometryShader* d3dShader)
	{
		if(mD3DGS)
			return false;
		if(!d3dShader)
			return false;
		mD3DGS = d3dShader;
		return true;
	}
}