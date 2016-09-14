#ifndef _TLRENDERDEVICEUSEDDEPTHSTENCILSTATE_D3D11_H_
#define _TLRENDERDEVICEUSEDDEPTHSTENCILSTATE_D3D11_H_

#include "TLIRenderDeviceUsedDepthStencilState.h"
#include <d3d11.h>
namespace TLunaEngine
{
	class TLRenderDeviceUsedDepthStencilState_D3D11 : public TLIRenderDeviceUsedDepthStencilState
	{
	public:
		TLRenderDeviceUsedDepthStencilState_D3D11();
		virtual ~TLRenderDeviceUsedDepthStencilState_D3D11();
	private:
		ID3D11DepthStencilState* mD3DDepthStencilState;
	public:
		// get d3d11 Depth Stencil state
		ID3D11DepthStencilState* getD3DDepthStencilState()
		{
			return mD3DDepthStencilState;
		}
		// init with d3d depth stencil state
		bool initWithD3DDepthStencilState(ID3D11DepthStencilState* d3dState);
	};
}

#endif