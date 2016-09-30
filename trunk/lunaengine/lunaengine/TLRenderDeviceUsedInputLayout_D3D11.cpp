#include "TLRenderDeviceUsedInputLayout_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedInputLayout_D3D11::RenderDeviceUsedInputLayout_D3D11():
		RenderDeviceUsedInputLayout(),mD3DInputLayout(0)
	{
	}

	RenderDeviceUsedInputLayout_D3D11::~RenderDeviceUsedInputLayout_D3D11()
	{
		if(mD3DInputLayout)
		{
			mD3DInputLayout->Release();
			mD3DInputLayout = 0;
		}
	}

	TBOOL RenderDeviceUsedInputLayout_D3D11::initWithD3DInputLayout(ID3D11InputLayout* d3dInputLayout)
	{
		if(mD3DInputLayout)
			return TTRUE;
		if(!d3dInputLayout)
			return TFALSE;
		mD3DInputLayout = d3dInputLayout;
		return TTRUE;
	}
}