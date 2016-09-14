#ifndef _TLRENDERDEVICEUSEDUAV_D3D11_H_
#define _TLRENDERDEVICEUSEDUAV_D3D11_H_

#include "TLIRenderDeviceUsedUAV.h"
#include "TLIRenderDeviceUsedView_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class TLRenderDeviceUsedUAV_D3D11 : public TLIRenderDeviceUsedUAV, public TLIRenderDeviceUsedView_D3D11
	{
	public:
		TLRenderDeviceUsedUAV_D3D11();
		virtual ~TLRenderDeviceUsedUAV_D3D11();
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
		bool initWithD3DUAV(ID3D11UnorderedAccessView* d3dView);
	};
}

#endif