#include "stdafx.h"
#include "TLRenderDeviceUsedBlendState_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedBlendState_D3D11::TLRenderDeviceUsedBlendState_D3D11():TLIRenderDeviceUsedBlendState(),mD3DBlendState(NULL)
	{
	}

	TLRenderDeviceUsedBlendState_D3D11::~TLRenderDeviceUsedBlendState_D3D11()
	{
		if(mD3DBlendState)
		{
			mD3DBlendState->Release();
			mD3DBlendState = NULL;
		}
	}

	bool TLRenderDeviceUsedBlendState_D3D11::initWithD3DBlendState(ID3D11BlendState* d3dState)
	{
		if(mD3DBlendState)
			return false;
		if(!d3dState)
			return false;
		mD3DBlendState = d3dState;
		return true;
	}
}