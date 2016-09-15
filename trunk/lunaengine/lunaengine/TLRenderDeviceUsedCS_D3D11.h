#ifndef _TLRENDERDEVICEUSEDCS_D3D11_H_
#define _TLRENDERDEVICEUSEDCS_D3D11_H_

#include "TLRenderDeviceUsedCS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedCS_D3D11 : public RenderDeviceUsedCS
	{
	public:
		RenderDeviceUsedCS_D3D11();
		virtual ~RenderDeviceUsedCS_D3D11();
	private:
		ID3D11ComputeShader* mD3DCS;
	public:
		// get d3d cs ptr
		ID3D11ComputeShader* getD3DCS()
		{
			return mD3DCS;
		}
		// init with d3d cs
		bool initWithD3DCS(ID3D11ComputeShader* d3dShader);
	};
}

#endif