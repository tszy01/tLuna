#ifndef _TLRENDERDEVICEUSEDSRV_H_
#define _TLRENDERDEVICEUSEDSRV_H_

#include "TLRenderDeviceUsedView.h"

namespace TLunaEngine
{
	// shader resource view
	class RenderDeviceUsedSRV : public RenderDeviceUsedView
	{
	public:
		RenderDeviceUsedSRV():RenderDeviceUsedView(){};
		virtual ~RenderDeviceUsedSRV(){};
	};
}

#endif