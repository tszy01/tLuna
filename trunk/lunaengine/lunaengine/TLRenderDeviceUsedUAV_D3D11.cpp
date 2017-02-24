#include "TLRenderDeviceUsedUAV_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedUAV_D3D11::RenderDeviceUsedUAV_D3D11():
		RenderDeviceUsedUAV(),RenderDeviceUsedView_D3D11(),mD3DUAV(0)
	{
	}

	RenderDeviceUsedUAV_D3D11::~RenderDeviceUsedUAV_D3D11()
	{
		if(mD3DUAV)
		{
			mD3DUAV->Release();
			mD3DUAV = 0;
		}
	}

	TSun::TBOOL RenderDeviceUsedUAV_D3D11::initWithD3DUAV(ID3D11UnorderedAccessView* d3dView)
	{
		if(mD3DUAV)
			return TSun::TFALSE;
		if(!d3dView)
			return TSun::TFALSE;
		mD3DUAV = d3dView;
		return TSun::TTRUE;
	}
}