#include "TLRenderDeviceUsedSRV_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedSRV_D3D11::RenderDeviceUsedSRV_D3D11():
		RenderDeviceUsedSRV(),RenderDeviceUsedView_D3D11(),mD3DSRV(0)
	{
	}

	RenderDeviceUsedSRV_D3D11::~RenderDeviceUsedSRV_D3D11()
	{
		if(mD3DSRV)
		{
			mD3DSRV->Release();
			mD3DSRV = 0;
		}
	}

	TSun::TBOOL RenderDeviceUsedSRV_D3D11::initWithD3DSRV(ID3D11ShaderResourceView* d3dView)
	{
		if(mD3DSRV)
			return TSun::TFALSE;
		if(!d3dView)
			return TSun::TFALSE;
		mD3DSRV = d3dView;
		return TSun::TTRUE;
	}
}