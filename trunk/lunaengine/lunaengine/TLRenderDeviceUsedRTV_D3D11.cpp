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

	TSun::TBOOL RenderDeviceUsedRTV_D3D11::initWithD3DRTV(ID3D11RenderTargetView* d3dView)
	{
		if(mD3DRTV)
			return TSun::TFALSE;
		if(!d3dView)
			return TSun::TFALSE;
		mD3DRTV = d3dView;
		return TSun::TTRUE;
	}
}