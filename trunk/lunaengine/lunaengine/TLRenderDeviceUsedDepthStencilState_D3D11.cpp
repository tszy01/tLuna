#include "stdafx.h"
#include "TLRenderDeviceUsedDepthStencilState_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedDepthStencilState_D3D11::TLRenderDeviceUsedDepthStencilState_D3D11():
		TLIRenderDeviceUsedDepthStencilState(),mD3DDepthStencilState(NULL)
	{
	}

	TLRenderDeviceUsedDepthStencilState_D3D11::~TLRenderDeviceUsedDepthStencilState_D3D11()
	{
		if(mD3DDepthStencilState)
		{
			mD3DDepthStencilState->Release();
			mD3DDepthStencilState = NULL;
		}
	}

	bool TLRenderDeviceUsedDepthStencilState_D3D11::initWithD3DDepthStencilState(ID3D11DepthStencilState* d3dState)
	{
		if(mD3DDepthStencilState)
			return false;
		if(!d3dState)
			return false;
		mD3DDepthStencilState = d3dState;
		return true;
	}
}