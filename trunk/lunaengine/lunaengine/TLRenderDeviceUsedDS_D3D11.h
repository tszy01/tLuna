#ifndef _TLRENDERDEVICEUSEDDS_D3D11_H_
#define _TLRENDERDEVICEUSEDDS_D3D11_H_

#include "TLRenderDeviceUsedDS.h"
#include <d3d11.h>

namespace TLunaEngine
{
	class RenderDeviceUsedDS_D3D11 : public RenderDeviceUsedDS
	{
	public:
		RenderDeviceUsedDS_D3D11();
		virtual ~RenderDeviceUsedDS_D3D11();
	private:
		ID3D11DomainShader* mD3DDS;
	public:
		// get d3d ds ptr
		ID3D11DomainShader* getD3DDS()
		{
			return mD3DDS;
		}
		// init with d3d ds
		bool initWithD3DDS(ID3D11DomainShader* d3dShader);
	};
}

#endif