#include "TLRenderDeviceUsedDepthStencilState_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedDepthStencilState_D3D11::RenderDeviceUsedDepthStencilState_D3D11():
		RenderDeviceUsedDepthStencilState(),mD3DDepthStencilState(0)
	{
	}

	RenderDeviceUsedDepthStencilState_D3D11::~RenderDeviceUsedDepthStencilState_D3D11()
	{
		if(mD3DDepthStencilState)
		{
			mD3DDepthStencilState->Release();
			mD3DDepthStencilState = 0;
		}
	}

	bool RenderDeviceUsedDepthStencilState_D3D11::initWithD3DDepthStencilState(ID3D11DepthStencilState* d3dState)
	{
		if(mD3DDepthStencilState)
			return false;
		if(!d3dState)
			return false;
		mD3DDepthStencilState = d3dState;
		return true;
	}
}