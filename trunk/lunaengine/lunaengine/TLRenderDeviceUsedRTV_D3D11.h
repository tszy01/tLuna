#ifndef _TLRENDERDEVICEUSEDRTV_D3D11_H_
#define _TLRENDERDEVICEUSEDRTV_D3D11_H_

#include "TLIRenderDeviceUsedRTV.h"
#include "TLIRenderDeviceUsedView_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class TLRenderDeviceUsedRTV_D3D11 : public TLIRenderDeviceUsedRTV, public TLIRenderDeviceUsedView_D3D11
	{
	public:
		TLRenderDeviceUsedRTV_D3D11();
		virtual ~TLRenderDeviceUsedRTV_D3D11();
	public:
		// get d3d view ptr
		virtual ID3D11View* getD3DView()
		{
			return (ID3D11View*)mD3DRTV;
		}
	private:
		ID3D11RenderTargetView* mD3DRTV;
	public:
		// get d3d rtv ptr
		ID3D11RenderTargetView* getD3DRTV()
		{
			return mD3DRTV;
		}
		// init with d3d rtv
		bool initWithD3DRTV(ID3D11RenderTargetView* d3dView);
	};
}

#endif