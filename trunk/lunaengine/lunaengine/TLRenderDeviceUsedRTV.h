#ifndef _TLRENDERDEVICEUSEDRTV_H_
#define _TLRENDERDEVICEUSEDRTV_H_

#include "TLRenderDeviceUsedView.h"

namespace TLunaEngine
{
	class RenderDeviceUsedRTV : public RenderDeviceUsedView
	{
	public:
		RenderDeviceUsedRTV():RenderDeviceUsedView(){};
		virtual ~RenderDeviceUsedRTV(){};
	};
}

#endif