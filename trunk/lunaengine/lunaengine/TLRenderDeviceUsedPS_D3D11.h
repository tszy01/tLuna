#ifndef _TLRENDERDEVICEUSEDPS_D3D11_H_
#define _TLRENDERDEVICEUSEDPS_D3D11_H_

#include "TLIRenderDeviceUsedPS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class TLRenderDeviceUsedPS_D3D11 : public TLIRenderDeviceUsedPS
	{
	public:
		TLRenderDeviceUsedPS_D3D11();
		virtual ~TLRenderDeviceUsedPS_D3D11();
	private:
		ID3D11PixelShader* mD3DPS;
	public:
		// get d3d ps ptr
		ID3D11PixelShader* getD3DPS()
		{
			return mD3DPS;
		}
		// init with d3d ps
		bool initWithD3DPS(ID3D11PixelShader* d3dShader);
	};
}

#endif