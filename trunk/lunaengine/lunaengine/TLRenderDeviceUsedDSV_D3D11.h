#ifndef _TLRENDERDEVICEUSEDDSV_D3D11_H_
#define _TLRENDERDEVICEUSEDDSV_D3D11_H_

#include "TLIRenderDeviceUsedDSV.h"
#include "TLIRenderDeviceUsedView_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class TLRenderDeviceUsedDSV_D3D11 : public TLIRenderDeviceUsedDSV, public TLIRenderDeviceUsedView_D3D11
	{
	public:
		TLRenderDeviceUsedDSV_D3D11();
		virtual ~TLRenderDeviceUsedDSV_D3D11();
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