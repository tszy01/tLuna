#ifndef _TLRENDERDEVICEUSEDBLENDSTATE_H_
#define _TLRENDERDEVICEUSEDBLENDSTATE_H_
#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// State
	class RenderDeviceUsedBlendState : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedBlendState():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedBlendState(){};
	};
}

#endif