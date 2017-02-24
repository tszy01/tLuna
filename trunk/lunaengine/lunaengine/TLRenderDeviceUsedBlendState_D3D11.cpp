#include "TLRenderDeviceUsedBlendState_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedBlendState_D3D11::RenderDeviceUsedBlendState_D3D11():RenderDeviceUsedBlendState(),mD3DBlendState(0)
	{
	}

	RenderDeviceUsedBlendState_D3D11::~RenderDeviceUsedBlendState_D3D11()
	{
		if(mD3DBlendState)
		{
			mD3DBlendState->Release();
			mD3DBlendState = 0;
		}
	}

	TSun::TBOOL RenderDeviceUsedBlendState_D3D11::initWithD3DBlendState(ID3D11BlendState* d3dState)
	{
		if(mD3DBlendState)
			return TSun::TFALSE;
		if(!d3dState)
			return TSun::TFALSE;
		mD3DBlendState = d3dState;
		return TSun::TTRUE;
	}
}