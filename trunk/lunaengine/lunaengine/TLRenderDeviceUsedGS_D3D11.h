#ifndef _TLRENDERDEVICEUSEDGS_D3D11_H_
#define _TLRENDERDEVICEUSEDGS_D3D11_H_

#include "TLIRenderDeviceUsedGS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class TLRenderDeviceUsedGS_D3D11 : public TLIRenderDeviceUsedGS
	{
	public:
		TLRenderDeviceUsedGS_D3D11();
		virtual ~TLRenderDeviceUsedGS_D3D11();
	private:
		ID3D11GeometryShader* mD3DGS;
	public:
		// get d3d gs ptr
		ID3D11GeometryShader* getD3DGS()
		{
			return mD3DGS;
		}
		// init with d3d gs
		bool initWithD3DGS(ID3D11GeometryShader* d3dShader);
	};
}

#endif