#ifndef _TLRENDERDEVICEUSEDHS_D3D11_H_
#define _TLRENDERDEVICEUSEDHS_D3D11_H_

#include "TLIRenderDeviceUsedHS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class TLRenderDeviceUsedHS_D3D11 : public TLIRenderDeviceUsedHS
	{
	public:
		TLRenderDeviceUsedHS_D3D11();
		virtual ~TLRenderDeviceUsedHS_D3D11();
	private:
		ID3D11HullShader* mD3DHS;
	public:
		// get d3d hs ptr
		ID3D11HullShader* getD3DHS()
		{
			return mD3DHS;
		}
		// init with d3d hs
		bool initWithD3DHS(ID3D11HullShader* d3dShader);
	};
}

#endif