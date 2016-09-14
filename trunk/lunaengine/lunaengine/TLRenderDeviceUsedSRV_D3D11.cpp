#include "stdafx.h"
#include "TLRenderDeviceUsedSRV_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedSRV_D3D11::TLRenderDeviceUsedSRV_D3D11():
		TLIRenderDeviceUsedSRV(),TLIRenderDeviceUsedView_D3D11(),mD3DSRV(NULL)
	{
	}

	TLRenderDeviceUsedSRV_D3D11::~TLRenderDeviceUsedSRV_D3D11()
	{
		if(mD3DSRV)
		{
			mD3DSRV->Release();
			mD3DSRV = NULL;
		}
	}

	bool TLRenderDeviceUsedSRV_D3D11::initWithD3DSRV(ID3D11ShaderResourceView* d3dView)
	{
		if(mD3DSRV)
			return false;
		if(!d3dView)
			return false;
		mD3DSRV = d3dView;
		return true;
	}
}