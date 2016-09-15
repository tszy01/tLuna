#ifndef _TLRENDERDEVICEUSEDSAMPLERSTATE_H_
#define _TLRENDERDEVICEUSEDSAMPLERSTATE_H_
#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// State
	class RenderDeviceUsedSamplerState : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedSamplerState():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedSamplerState(){};
	};
}

#endif