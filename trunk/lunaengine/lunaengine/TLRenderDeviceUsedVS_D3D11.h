#ifndef _TLRENDERDEVICEUSEDVS_D3D11_H_
#define _TLRENDERDEVICEUSEDVS_D3D11_H_

#include "TLRenderDeviceUsedVS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedVS_D3D11 : public RenderDeviceUsedVS
	{
	public:
		RenderDeviceUsedVS_D3D11();
		virtual ~RenderDeviceUsedVS_D3D11();
	private:
		ID3D11VertexShader* mD3DVS;
	public:
		// get d3d vs ptr
		ID3D11VertexShader* getD3DVS()
		{
			return mD3DVS;
		}
		// init with d3d vs
		bool initWithD3DVS(ID3D11VertexShader* d3dShader);
	};
}

#endif