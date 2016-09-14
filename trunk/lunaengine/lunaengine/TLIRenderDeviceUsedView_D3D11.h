#ifndef _TLIRENDERDEVICEUSEDVIEW_D3D11_H_
#define _TLIRENDERDEVICEUSEDVIEW_D3D11_H_

#include <d3d11.h>

namespace TLunaEngine
{
	// view interface for d3d11
	class TLIRenderDeviceUsedView_D3D11
	{
	public:
		TLIRenderDeviceUsedView_D3D11(){};
		virtual ~TLIRenderDeviceUsedView_D3D11(){};
	public:
		// get d3d view ptr
		virtual ID3D11View* getD3DView() = 0;
	};
}
#endif