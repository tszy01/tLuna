#include "stdafx.h"
#include "TLRenderDeviceUsedInputLayout_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedInputLayout_D3D11::TLRenderDeviceUsedInputLayout_D3D11():
		TLIRenderDeviceUsedInputLayout(),mD3DInputLayout(NULL)
	{
	}

	TLRenderDeviceUsedInputLayout_D3D11::~TLRenderDeviceUsedInputLayout_D3D11()
	{
		if(mD3DInputLayout)
		{
			mD3DInputLayout->Release();
			mD3DInputLayout = NULL;
		}
	}

	bool TLRenderDeviceUsedInputLayout_D3D11::initWithD3DInputLayout(ID3D11InputLayout* d3dInputLayout)
	{
		if(mD3DInputLayout)
			return true;
		if(!d3dInputLayout)
			return false;
		mD3DInputLayout = d3dInputLayout;
		return true;
	}
}