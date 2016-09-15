#include "TLRenderDeviceUsedRTV_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedRTV_D3D11::RenderDeviceUsedRTV_D3D11():
		RenderDeviceUsedRTV(),RenderDeviceUsedView_D3D11(),mD3DRTV(0)
	{
	}

	RenderDeviceUsedRTV_D3D11::~RenderDeviceUsedRTV_D3D11()
	{
		if(mD3DRTV)
		{
			mD3DRTV->Release();
			mD3DRTV = 0;
		}
	}

	bool RenderDeviceUsedRTV_D3D11::initWithD3DRTV(ID3D11RenderTargetView* d3dView)
	{
		if(mD3DRTV)
			return false;
		if(!d3dView)
			return false;
		mD3DRTV = d3dView;
		return true;
	}
}