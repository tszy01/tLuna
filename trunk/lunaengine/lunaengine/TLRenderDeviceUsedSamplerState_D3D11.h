#ifndef _TLRENDERDEVICEUSEDSAMPLERSTATE_D3D11_H_
#define _TLRENDERDEVICEUSEDSAMPLERSTATE_D3D11_H_

#include "TSCommonTypeDef.h"
#include "TLRenderDeviceUsedSamplerState.h"
#include <d3d11.h>
namespace TLunaEngine
{
	class RenderDeviceUsedSamplerState_D3D11 : public RenderDeviceUsedSamplerState
	{
	public:
		RenderDeviceUsedSamplerState_D3D11();
		virtual ~RenderDeviceUsedSamplerState_D3D11();
	private:
		ID3D11SamplerState* mD3DSamplerState;
	public:
		// get d3d11 sampler state
		ID3D11SamplerState* getD3DSamplerState()
		{
			return mD3DSamplerState;
		}
		// init with d3d sampler state
		TSun::TBOOL initWithD3DSamplerState(ID3D11SamplerState* d3dState);
	};
}

#endif