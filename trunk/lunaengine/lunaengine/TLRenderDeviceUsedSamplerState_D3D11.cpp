#include "stdafx.h"
#include "TLRenderDeviceUsedSamplerState_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedSamplerState_D3D11::TLRenderDeviceUsedSamplerState_D3D11():
		TLIRenderDeviceUsedSamplerState(),mD3DSamplerState(NULL)
	{
	}

	TLRenderDeviceUsedSamplerState_D3D11::~TLRenderDeviceUsedSamplerState_D3D11()
	{
		if(mD3DSamplerState)
		{
			mD3DSamplerState->Release();
			mD3DSamplerState = NULL;
		}
	}

	bool TLRenderDeviceUsedSamplerState_D3D11::initWithD3DSamplerState(ID3D11SamplerState* d3dState)
	{
		if(mD3DSamplerState)
			return false;
		if(!d3dState)
			return false;
		mD3DSamplerState = d3dState;
		return true;
	}
}