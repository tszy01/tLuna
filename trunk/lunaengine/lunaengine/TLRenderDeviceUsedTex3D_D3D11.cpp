#include "stdafx.h"
#include "TLRenderDeviceUsedTex3D_D3D11.h"

namespace TLunaEngine
{
	TLRenderDeviceUsedTex3D_D3D11::TLRenderDeviceUsedTex3D_D3D11():
		TLIRenderDeviceUsedTex3D(),TLIRenderDeviceUsedResource_D3D11(),mD3DTex3D(NULL)
	{
	}

	TLRenderDeviceUsedTex3D_D3D11::~TLRenderDeviceUsedTex3D_D3D11()
	{
		if(mD3DTex3D)
		{
			mD3DTex3D->Release();
			mD3DTex3D = NULL;
		}
	}

	bool TLRenderDeviceUsedTex3D_D3D11::initWithD3DTex3D(ID3D11Texture3D* d3dTex)
	{
		if(mD3DTex3D)
			return false;
		if(!d3dTex)
			return false;
		mD3DTex3D = d3dTex;
		return true;
	}
}