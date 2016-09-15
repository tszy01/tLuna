#ifndef _TLRENDERDEVICEUSEDDSV_H_
#define _TLRENDERDEVICEUSEDDSV_H_

#include "TLRenderDeviceUsedView.h"

namespace TLunaEngine
{
	class RenderDeviceUsedDSV : public RenderDeviceUsedView
	{
	public:
		RenderDeviceUsedDSV():RenderDeviceUsedView(){};
		virtual ~RenderDeviceUsedDSV(){};
	};
}

#endif