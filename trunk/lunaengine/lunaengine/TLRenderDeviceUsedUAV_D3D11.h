#ifndef _TLRENDERDEVICEUSEDUAV_D3D11_H_
#define _TLRENDERDEVICEUSEDUAV_D3D11_H_

#include "TLCommonTypeDef.h"
#include "TLRenderDeviceUsedUAV.h"
#include "TLRenderDeviceUsedView_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedUAV_D3D11 : public RenderDeviceUsedUAV, public RenderDeviceUsedView_D3D11
	{
	public:
		RenderDeviceUsedUAV_D3D11();
		virtual ~RenderDeviceUsedUAV_D3D11();
	public:
		// get d3d view ptr
		virtual ID3D11View* getD3DView()
		{
			return (ID3D11View*)mD3DUAV;
		}
	private:
		ID3D11UnorderedAccessView* mD3DUAV;
	public:
		// get d3d uav ptr
		ID3D11UnorderedAccessView* getD3DUAV()
		{
			return mD3DUAV;
		}
		// init with d3d uav
		TBOOL initWithD3DUAV(ID3D11UnorderedAccessView* d3dView);
	};
}

#endif