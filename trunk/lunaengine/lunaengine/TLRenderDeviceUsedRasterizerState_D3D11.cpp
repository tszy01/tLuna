#include "stdafx.h"
#include "TLRenderDeviceUsedRasterizerState_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedRasterizerState_D3D11::TLRenderDeviceUsedRasterizerState_D3D11():
		TLIRenderDeviceUsedRasterizerState(),mD3DRasterizerState(NULL)
	{
	}

	TLRenderDeviceUsedRasterizerState_D3D11::~TLRenderDeviceUsedRasterizerState_D3D11()
	{
		if(mD3DRasterizerState)
		{
			mD3DRasterizerState->Release();
			mD3DRasterizerState = NULL;
		}
	}

	bool TLRenderDeviceUsedRasterizerState_D3D11::initWithD3DRasterizerState(ID3D11RasterizerState* d3dState)
	{
		if(mD3DRasterizerState)
			return false;
		if(!d3dState)
			return false;
		mD3DRasterizerState = d3dState;
		return true;
	}
}