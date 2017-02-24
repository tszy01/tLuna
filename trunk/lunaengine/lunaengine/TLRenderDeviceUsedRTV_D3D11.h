#ifndef _TLRENDERDEVICEUSEDRTV_D3D11_H_
#define _TLRENDERDEVICEUSEDRTV_D3D11_H_

#include "TSCommonTypeDef.h"
#include "TLRenderDeviceUsedRTV.h"
#include "TLRenderDeviceUsedView_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedRTV_D3D11 : public RenderDeviceUsedRTV, public RenderDeviceUsedView_D3D11
	{
	public:
		RenderDeviceUsedRTV_D3D11();
		virtual ~RenderDeviceUsedRTV_D3D11();
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
		TSun::TBOOL initWithD3DRTV(ID3D11RenderTargetView* d3dView);
	};
}

#endif