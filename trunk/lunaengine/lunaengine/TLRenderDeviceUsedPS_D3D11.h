#ifndef _TLRENDERDEVICEUSEDPS_D3D11_H_
#define _TLRENDERDEVICEUSEDPS_D3D11_H_

#include "TLCommonTypeDef.h"
#include "TLRenderDeviceUsedPS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedPS_D3D11 : public RenderDeviceUsedPS
	{
	public:
		RenderDeviceUsedPS_D3D11();
		virtual ~RenderDeviceUsedPS_D3D11();
	private:
		ID3D11PixelShader* mD3DPS;
	public:
		// get d3d ps ptr
		ID3D11PixelShader* getD3DPS()
		{
			return mD3DPS;
		}
		// init with d3d ps
		TBOOL initWithD3DPS(ID3D11PixelShader* d3dShader);
	};
}

#endif