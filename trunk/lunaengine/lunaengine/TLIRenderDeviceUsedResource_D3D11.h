#ifndef _TLIRENDERDEVICEUSEDRESOURCE_D3D11_H_
#define _TLIRENDERDEVICEUSEDRESOURCE_D3D11_H_

#include <d3d11.h>

namespace TLunaEngine
{
	// resource interface for d3d11
	class TLIRenderDeviceUsedResource_D3D11
	{
	public:
		TLIRenderDeviceUsedResource_D3D11(){};
		virtual ~TLIRenderDeviceUsedResource_D3D11(){};
	public:
		// get d3d resource ptr
		virtual ID3D11Resource* getD3DResource() = 0;
	};
}
#endif