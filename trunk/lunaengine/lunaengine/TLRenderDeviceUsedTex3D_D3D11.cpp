#include "TLRenderDeviceUsedTex3D_D3D11.h"

namespace TLunaEngine
{
	RenderDeviceUsedTex3D_D3D11::RenderDeviceUsedTex3D_D3D11():
		RenderDeviceUsedTex3D(),RenderDeviceUsedResource_D3D11(),mD3DTex3D(TSun::TNULL)
	{
	}

	RenderDeviceUsedTex3D_D3D11::~RenderDeviceUsedTex3D_D3D11()
	{
		if(mD3DTex3D)
		{
			mD3DTex3D->Release();
			mD3DTex3D = TSun::TNULL;
		}
	}

	TSun::TBOOL RenderDeviceUsedTex3D_D3D11::initWithD3DTex3D(ID3D11Texture3D* d3dTex)
	{
		if(mD3DTex3D)
			return TSun::TFALSE;
		if(!d3dTex)
			return TSun::TFALSE;
		mD3DTex3D = d3dTex;
		return TSun::TTRUE;
	}
}