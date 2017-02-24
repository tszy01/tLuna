#ifndef _TLRENDERDEVICEUSEDSRV_D3D11_H_
#define _TLRENDERDEVICEUSEDSRV_D3D11_H_

#include "TSCommonTypeDef.h"
#include "TLRenderDeviceUsedSRV.h"
#include "TLRenderDeviceUsedView_D3D11.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedSRV_D3D11 : public RenderDeviceUsedSRV, public RenderDeviceUsedView_D3D11
	{
	public:
		RenderDeviceUsedSRV_D3D11();
		virtual ~RenderDeviceUsedSRV_D3D11();
	public:
		// get d3d view ptr
		virtual ID3D11View* getD3DView()
		{
			return (ID3D11View*)mD3DSRV;
		}
	private:
		ID3D11ShaderResourceView* mD3DSRV;
	public:
		// get d3d srv ptr
		ID3D11ShaderResourceView* getD3DSRV()
		{
			return mD3DSRV;
		}
		// init with d3d srv ptr
		TSun::TBOOL initWithD3DSRV(ID3D11ShaderResourceView* d3dView);
	};
}

#endif