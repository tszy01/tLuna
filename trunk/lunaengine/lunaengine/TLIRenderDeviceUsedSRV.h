#ifndef _TLIRENDERDEVICEUSEDSRV_H_
#define _TLIRENDERDEVICEUSEDSRV_H_

#include "TLIRenderDeviceUsedView.h"

namespace TLunaEngine
{
	// shader resource view
	class TLIRenderDeviceUsedSRV : public TLIRenderDeviceUsedView
	{
	public:
		TLIRenderDeviceUsedSRV():TLIRenderDeviceUsedView(){};
		virtual ~TLIRenderDeviceUsedSRV(){};
	};
}

#endif