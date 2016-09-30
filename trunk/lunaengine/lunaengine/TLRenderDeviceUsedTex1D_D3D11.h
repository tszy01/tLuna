#ifndef _TLRENDERDEVICEUSEDTEX1D_D3D11_H_
#define _TLRENDERDEVICEUSEDTEX1D_D3D11_H_

#include "TLRenderDeviceUsedTex1D.h"
#include "TLRenderDeviceUsedResource_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	// texture 1d class
	class RenderDeviceUsedTex1D_D3D11 : public RenderDeviceUsedTex1D, public RenderDeviceUsedResource_D3D11
	{
	public:
		RenderDeviceUsedTex1D_D3D11();
		virtual ~RenderDeviceUsedTex1D_D3D11();
	public:
		// get d3d resource ptr
		virtual ID3D11Resource* getD3DResource()
		{
			return (ID3D11Resource*)mD3DTex1D;
		}
	private:
		ID3D11Texture1D* mD3DTex1D;
	public:
		// get d3d tex ptr
		ID3D11Texture1D* getD3DTex1D()
		{
			return mD3DTex1D;
		}
		// init with d3d tex
		TBOOL initWithD3DTex1D(ID3D11Texture1D* d3dTex);
	};
}

#endif