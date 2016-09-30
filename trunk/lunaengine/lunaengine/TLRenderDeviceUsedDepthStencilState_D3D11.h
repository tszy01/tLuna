#ifndef _TLRENDERDEVICEUSEDDEPTHSTENCILSTATE_D3D11_H_
#define _TLRENDERDEVICEUSEDDEPTHSTENCILSTATE_D3D11_H_

#include "TLCommonTypeDef.h"
#include "TLRenderDeviceUsedDepthStencilState.h"
#include <d3d11.h>
namespace TLunaEngine
{
	class RenderDeviceUsedDepthStencilState_D3D11 : public RenderDeviceUsedDepthStencilState
	{
	public:
		RenderDeviceUsedDepthStencilState_D3D11();
		virtual ~RenderDeviceUsedDepthStencilState_D3D11();
	private:
		ID3D11DepthStencilState* mD3DDepthStencilState;
	public:
		// get d3d11 Depth Stencil state
		ID3D11DepthStencilState* getD3DDepthStencilState()
		{
			return mD3DDepthStencilState;
		}
		// init with d3d depth stencil state
		TBOOL initWithD3DDepthStencilState(ID3D11DepthStencilState* d3dState);
	};
}

#endif