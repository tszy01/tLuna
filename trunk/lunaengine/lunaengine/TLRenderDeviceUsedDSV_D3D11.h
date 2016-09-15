#ifndef _TLRENDERDEVICEUSEDDSV_D3D11_H_
#define _TLRENDERDEVICEUSEDDSV_D3D11_H_

#include "TLRenderDeviceUsedDSV.h"
#include "TLRenderDeviceUsedView_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedDSV_D3D11 : public RenderDeviceUsedDSV, public RenderDeviceUsedView_D3D11
	{
	public:
		RenderDeviceUsedDSV_D3D11();
		virtual ~RenderDeviceUsedDSV_D3D11();
	public:
		// get d3d view ptr
		virtual ID3D11View* getD3DView()
		{
			return (ID3D11View*)mD3DDSV;
		}
	private:
		ID3D11DepthStencilView* mD3DDSV;
	public:
		// get d3d dsv ptr
		ID3D11DepthStencilView* getD3DDSV()
		{
			return mD3DDSV;
		}
		// init with d3d dsv
		bool initWithD3DDSV(ID3D11DepthStencilView* d3dView);
	};
}

#endif