#include "TLRenderDeviceUsedTex1D_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedTex1D_D3D11::RenderDeviceUsedTex1D_D3D11():
		RenderDeviceUsedTex1D(),RenderDeviceUsedResource_D3D11(),mD3DTex1D(TSun::TNULL)
	{
	}

	RenderDeviceUsedTex1D_D3D11::~RenderDeviceUsedTex1D_D3D11()
	{
		if(mD3DTex1D)
		{
			mD3DTex1D->Release();
			mD3DTex1D = TSun::TNULL;
		}
	}

	TSun::TBOOL RenderDeviceUsedTex1D_D3D11::initWithD3DTex1D(ID3D11Texture1D* d3dTex)
	{
		if(mD3DTex1D)
			return TSun::TFALSE;
		if(!d3dTex)
			return TSun::TFALSE;
		mD3DTex1D = d3dTex;
		return TSun::TTRUE;
	}
}