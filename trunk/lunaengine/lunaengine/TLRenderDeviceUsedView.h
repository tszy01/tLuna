#ifndef _TLRENDERDEVICEUSEDVIEW_H_
#define _TLRENDERDEVICEUSEDVIEW_H_

#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// View
	class RenderDeviceUsedView : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedView():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedView(){};
	};
}

#endif