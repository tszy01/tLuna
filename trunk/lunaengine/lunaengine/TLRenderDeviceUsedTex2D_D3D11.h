#ifndef _TLRENDERDEVICEUSEDTEX2D_D3D11_H_
#define _TLRENDERDEVICEUSEDTEX2D_D3D11_H_

#include "TLIRenderDeviceUsedTex2D.h"
#include "TLIRenderDeviceUsedResource_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	// texture 2d class
	class TLRenderDeviceUsedTex2D_D3D11 : public TLIRenderDeviceUsedTex2D, public TLIRenderDeviceUsedResource_D3D11
	{
	public:
		TLRenderDeviceUsedTex2D_D3D11();
		virtual ~TLRenderDeviceUsedTex2D_D3D11();
	public:
		// get d3d resource ptr
		virtual ID3D11Resource* getD3DResource()
		{
			return (ID3D11Resource*)mD3DTex2D;
		}
	private:
		ID3D11Texture2D* mD3DTex2D;
	public:
		// get d3d tex ptr
		ID3D11Texture2D* getD3DTex2D()
		{
			return mD3DTex2D;
		}
		// init with d3d tex
		bool initWithD3DTex2D(ID3D11Texture2D* d3dTex);
	};
}

#endif