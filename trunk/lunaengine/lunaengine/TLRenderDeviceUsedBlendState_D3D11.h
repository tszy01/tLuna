#ifndef _TLRENDERDEVICEUSEDBLENDSTATE_D3D11_H_
#define _TLRENDERDEVICEUSEDBLENDSTATE_D3D11_H_

#include "TLIRenderDeviceUsedBlendState.h"
#include <d3d11.h>
namespace TLunaEngine
{
	class TLRenderDeviceUsedBlendState_D3D11 : public TLIRenderDeviceUsedBlendState
	{
	public:
		TLRenderDeviceUsedBlendState_D3D11();
		virtual ~TLRenderDeviceUsedBlendState_D3D11();
	private:
		ID3D11BlendState* mD3DBlendState;
	public:
		// get d3d11 Blend state
		ID3D11BlendState* getD3DBlendState()
		{
			return mD3DBlendState;
		}
		// init using d3d ptr
		bool initWithD3DBlendState(ID3D11BlendState* d3dState);
	};
}

#endif