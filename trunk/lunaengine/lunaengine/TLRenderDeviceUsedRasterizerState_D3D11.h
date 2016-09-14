#ifndef _TLRENDERDEVICEUSEDRASTERIZERSTATE_D3D11_H_
#define _TLRENDERDEVICEUSEDRASTERIZERSTATE_D3D11_H_

#include "TLIRenderDeviceUsedRasterizerState.h"
#include <d3d11.h>
namespace TLunaEngine
{
	class TLRenderDeviceUsedRasterizerState_D3D11 : public TLIRenderDeviceUsedRasterizerState
	{
	public:
		TLRenderDeviceUsedRasterizerState_D3D11();
		virtual ~TLRenderDeviceUsedRasterizerState_D3D11();
	private:
		ID3D11RasterizerState* mD3DRasterizerState;
	public:
		// get d3d11 Rasterizer state
		ID3D11RasterizerState* getD3DRasterizerState()
		{
			return mD3DRasterizerState;
		}
		// init with d3d rasterizer state
		bool initWithD3DRasterizerState(ID3D11RasterizerState* d3dState);
	};
}

#endif