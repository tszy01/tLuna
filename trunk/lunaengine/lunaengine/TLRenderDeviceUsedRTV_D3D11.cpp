#include "stdafx.h"
#include "TLRenderDeviceUsedRTV_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedRTV_D3D11::TLRenderDeviceUsedRTV_D3D11():
		TLIRenderDeviceUsedRTV(),TLIRenderDeviceUsedView_D3D11(),mD3DRTV(NULL)
	{
	}

	TLRenderDeviceUsedRTV_D3D11::~TLRenderDeviceUsedRTV_D3D11()
	{
		if(mD3DRTV)
		{
			mD3DRTV->Release();
			mD3DRTV = NULL;
		}
	}

	bool TLRenderDeviceUsedRTV_D3D11::initWithD3DRTV(ID3D11RenderTargetView* d3dView)
	{
		if(mD3DRTV)
			return false;
		if(!d3dView)
			return false;
		mD3DRTV = d3dView;
		return true;
	}
}