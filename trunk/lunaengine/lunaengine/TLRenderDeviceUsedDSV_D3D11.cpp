#include "TLRenderDeviceUsedDSV_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedDSV_D3D11::RenderDeviceUsedDSV_D3D11():
		RenderDeviceUsedDSV(),RenderDeviceUsedView_D3D11(),mD3DDSV(0)
	{
	}

	RenderDeviceUsedDSV_D3D11::~RenderDeviceUsedDSV_D3D11()
	{
		if(mD3DDSV)
		{
			mD3DDSV->Release();
			mD3DDSV = 0;
		}
	}

	TSun::TBOOL RenderDeviceUsedDSV_D3D11::initWithD3DDSV(ID3D11DepthStencilView* d3dView)
	{
		if(mD3DDSV)
			return TSun::TFALSE;
		if(!d3dView)
			return TSun::TFALSE;
		mD3DDSV = d3dView;
		return TSun::TTRUE;
	}
}