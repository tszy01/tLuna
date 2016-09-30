#ifndef _TLRENDERDEVICEUSEDGS_D3D11_H_
#define _TLRENDERDEVICEUSEDGS_D3D11_H_

#include "TLCommonTypeDef.h"
#include "TLRenderDeviceUsedGS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedGS_D3D11 : public RenderDeviceUsedGS
	{
	public:
		RenderDeviceUsedGS_D3D11();
		virtual ~RenderDeviceUsedGS_D3D11();
	private:
		ID3D11GeometryShader* mD3DGS;
	public:
		// get d3d gs ptr
		ID3D11GeometryShader* getD3DGS()
		{
			return mD3DGS;
		}
		// init with d3d gs
		TBOOL initWithD3DGS(ID3D11GeometryShader* d3dShader);
	};
}

#endif