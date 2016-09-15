#ifndef _TLRENDERDEVICEUSEDRESOURCE_D3D11_H_
#define _TLRENDERDEVICEUSEDRESOURCE_D3D11_H_

#include <d3d11.h>

namespace TLunaEngine
{
	// resource interface for d3d11
	class RenderDeviceUsedResource_D3D11
	{
	public:
		RenderDeviceUsedResource_D3D11(){};
		virtual ~RenderDeviceUsedResource_D3D11(){};
	public:
		// get d3d resource ptr
		virtual ID3D11Resource* getD3DResource() = 0;
	};
}
#endif