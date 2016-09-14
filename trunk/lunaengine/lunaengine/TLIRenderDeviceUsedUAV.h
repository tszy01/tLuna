#ifndef _TLIRENDERDEVICEUSEDUAV_H_
#define _TLIRENDERDEVICEUSEDUAV_H_

#include "TLIRenderDeviceUsedView.h"

namespace TLunaEngine
{
	class TLIRenderDeviceUsedUAV : public TLIRenderDeviceUsedView
	{
	public:
		TLIRenderDeviceUsedUAV():TLIRenderDeviceUsedView(){};
		virtual ~TLIRenderDeviceUsedUAV(){};
	};
}

#endif