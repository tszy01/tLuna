#ifndef _TLRENDERDEVICEUSEDDEPTHSTENCILSTATE_H_
#define _TLRENDERDEVICEUSEDDEPTHSTENCILSTATE_H_
#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// State
	class RenderDeviceUsedDepthStencilState : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedDepthStencilState():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedDepthStencilState(){};
	};
}

#endif