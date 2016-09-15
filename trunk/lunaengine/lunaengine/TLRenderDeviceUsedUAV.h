#ifndef _TLRENDERDEVICEUSEDUAV_H_
#define _TLRENDERDEVICEUSEDUAV_H_

#include "TLRenderDeviceUsedView.h"

namespace TLunaEngine
{
	class RenderDeviceUsedUAV : public RenderDeviceUsedView
	{
	public:
		RenderDeviceUsedUAV():RenderDeviceUsedView(){};
		virtual ~RenderDeviceUsedUAV(){};
	};
}

#endif