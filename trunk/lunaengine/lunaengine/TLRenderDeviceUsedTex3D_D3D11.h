#ifndef _TLRENDERDEVICEUSEDTEX3D_D3D11_H_
#define _TLRENDERDEVICEUSEDTEX3D_D3D11_H_

#include "TLRenderDeviceUsedTex3D.h"
#include "TLRenderDeviceUsedResource_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	// texture 3d class
	class RenderDeviceUsedTex3D_D3D11 : public RenderDeviceUsedTex3D, public RenderDeviceUsedResource_D3D11
	{
	public:
		RenderDeviceUsedTex3D_D3D11();
		virtual ~RenderDeviceUsedTex3D_D3D11();
	public:
		// get d3d resource ptr
		virtual ID3D11Resource* getD3DResource()
		{
			return (ID3D11Resource*)mD3DTex3D;
		}
	private:
		ID3D11Texture3D* mD3DTex3D;
	public:
		// get d3d tex ptr
		ID3D11Texture3D* getD3DTex3D()
		{
			return mD3DTex3D;
		}
		// init with d3d tex
		TBOOL initWithD3DTex3D(ID3D11Texture3D* d3dTex);
	};
}

#endif