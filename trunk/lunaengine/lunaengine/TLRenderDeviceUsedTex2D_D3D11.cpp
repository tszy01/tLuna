#include "TLRenderDeviceUsedTex2D_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedTex2D_D3D11::RenderDeviceUsedTex2D_D3D11():
		RenderDeviceUsedTex2D(),RenderDeviceUsedResource_D3D11(),mD3DTex2D(TSun::TNULL)
	{
	}

	RenderDeviceUsedTex2D_D3D11::~RenderDeviceUsedTex2D_D3D11()
	{
		if(mD3DTex2D)
		{
			mD3DTex2D->Release();
			mD3DTex2D = TSun::TNULL;
		}
	}

	TSun::TBOOL RenderDeviceUsedTex2D_D3D11::initWithD3DTex2D(ID3D11Texture2D* d3dTex)
	{
		if(mD3DTex2D)
			return TSun::TFALSE;
		if(!d3dTex)
			return TSun::TFALSE;
		mD3DTex2D = d3dTex;
		return TSun::TTRUE;
	}
}