#include "stdafx.h"
#include "TLRenderDeviceUsedUAV_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedUAV_D3D11::TLRenderDeviceUsedUAV_D3D11():
		TLIRenderDeviceUsedUAV(),TLIRenderDeviceUsedView_D3D11(),mD3DUAV(NULL)
	{
	}

	TLRenderDeviceUsedUAV_D3D11::~TLRenderDeviceUsedUAV_D3D11()
	{
		if(mD3DUAV)
		{
			mD3DUAV->Release();
			mD3DUAV = NULL;
		}
	}

	bool TLRenderDeviceUsedUAV_D3D11::initWithD3DUAV(ID3D11UnorderedAccessView* d3dView)
	{
		if(mD3DUAV)
			return false;
		if(!d3dView)
			return false;
		mD3DUAV = d3dView;
		return true;
	}
}