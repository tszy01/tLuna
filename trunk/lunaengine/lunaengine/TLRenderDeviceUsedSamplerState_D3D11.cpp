#include "TLRenderDeviceUsedSamplerState_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedSamplerState_D3D11::RenderDeviceUsedSamplerState_D3D11():
		RenderDeviceUsedSamplerState(),mD3DSamplerState(0)
	{
	}

	RenderDeviceUsedSamplerState_D3D11::~RenderDeviceUsedSamplerState_D3D11()
	{
		if(mD3DSamplerState)
		{
			mD3DSamplerState->Release();
			mD3DSamplerState = 0;
		}
	}

	bool RenderDeviceUsedSamplerState_D3D11::initWithD3DSamplerState(ID3D11SamplerState* d3dState)
	{
		if(mD3DSamplerState)
			return false;
		if(!d3dState)
			return false;
		mD3DSamplerState = d3dState;
		return true;
	}
}