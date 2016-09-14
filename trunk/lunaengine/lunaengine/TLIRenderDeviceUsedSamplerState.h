#ifndef _TLIRENDERDEVICEUSEDSAMPLERSTATE_H_
#define _TLIRENDERDEVICEUSEDSAMPLERSTATE_H_
#include "TLIRenderDeviceUsed.h"

namespace TLunaEngine
{
	// State
	class TLIRenderDeviceUsedSamplerState : public TLIRenderDeviceUsed
	{
	public:
		TLIRenderDeviceUsedSamplerState():TLIRenderDeviceUsed(){};
		virtual ~TLIRenderDeviceUsedSamplerState(){};
	};
}

#endif