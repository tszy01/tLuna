#ifndef _TLRENDERDEVICEUSEDVIEW_D3D11_H_
#define _TLRENDERDEVICEUSEDVIEW_D3D11_H_

#include <d3d11.h>

namespace TLunaEngine
{
	// view interface for d3d11
	class RenderDeviceUsedView_D3D11
	{
	public:
		RenderDeviceUsedView_D3D11(){};
		virtual ~RenderDeviceUsedView_D3D11(){};
	public:
		// get d3d view ptr
		virtual ID3D11View* getD3DView() = 0;
	};
}
#endif