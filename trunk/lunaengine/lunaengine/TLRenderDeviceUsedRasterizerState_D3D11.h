#ifndef _TLRENDERDEVICEUSEDRASTERIZERSTATE_D3D11_H_
#define _TLRENDERDEVICEUSEDRASTERIZERSTATE_D3D11_H_

#include "TLCommonTypeDef.h"
#include "TLRenderDeviceUsedRasterizerState.h"
#include <d3d11.h>
namespace TLunaEngine
{
	class RenderDeviceUsedRasterizerState_D3D11 : public RenderDeviceUsedRasterizerState
	{
	public:
		RenderDeviceUsedRasterizerState_D3D11();
		virtual ~RenderDeviceUsedRasterizerState_D3D11();
	private:
		ID3D11RasterizerState* mD3DRasterizerState;
	public:
		// get d3d11 Rasterizer state
		ID3D11RasterizerState* getD3DRasterizerState()
		{
			return mD3DRasterizerState;
		}
		// init with d3d rasterizer state
		TBOOL initWithD3DRasterizerState(ID3D11RasterizerState* d3dState);
	};
}

#endif