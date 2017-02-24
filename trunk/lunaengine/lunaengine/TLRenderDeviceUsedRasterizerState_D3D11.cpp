#include "TLRenderDeviceUsedRasterizerState_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedRasterizerState_D3D11::RenderDeviceUsedRasterizerState_D3D11():
		RenderDeviceUsedRasterizerState(),mD3DRasterizerState(0)
	{
	}

	RenderDeviceUsedRasterizerState_D3D11::~RenderDeviceUsedRasterizerState_D3D11()
	{
		if(mD3DRasterizerState)
		{
			mD3DRasterizerState->Release();
			mD3DRasterizerState = 0;
		}
	}

	TSun::TBOOL RenderDeviceUsedRasterizerState_D3D11::initWithD3DRasterizerState(ID3D11RasterizerState* d3dState)
	{
		if(mD3DRasterizerState)
			return TSun::TFALSE;
		if(!d3dState)
			return TSun::TFALSE;
		mD3DRasterizerState = d3dState;
		return TSun::TTRUE;
	}
}