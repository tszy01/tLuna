#ifndef _TLIRENDERDEVICEUSEDVIEW_H_
#define _TLIRENDERDEVICEUSEDVIEW_H_

#include "TLIRenderDeviceUsed.h"

namespace TLunaEngine
{
	// View
	class TLIRenderDeviceUsedView : public TLIRenderDeviceUsed
	{
	public:
		TLIRenderDeviceUsedView():TLIRenderDeviceUsed(){};
		virtual ~TLIRenderDeviceUsedView(){};
	};
}

#endif