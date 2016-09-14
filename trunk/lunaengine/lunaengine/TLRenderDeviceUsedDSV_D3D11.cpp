#include "stdafx.h"
#include "TLRenderDeviceUsedDSV_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedDSV_D3D11::TLRenderDeviceUsedDSV_D3D11():
		TLIRenderDeviceUsedDSV(),TLIRenderDeviceUsedView_D3D11(),mD3DDSV(NULL)
	{
	}

	TLRenderDeviceUsedDSV_D3D11::~TLRenderDeviceUsedDSV_D3D11()
	{
		if(mD3DDSV)
		{
			mD3DDSV->Release();
			mD3DDSV = NULL;
		}
	}

	bool TLRenderDeviceUsedDSV_D3D11::initWithD3DDSV(ID3D11DepthStencilView* d3dView)
	{
		if(mD3DDSV)
			return false;
		if(!d3dView)
			return false;
		mD3DDSV = d3dView;
		return true;
	}
}