#ifndef _TLRENDERDEVICEUSEDRASTERIZERSTATE_H_
#define _TLRENDERDEVICEUSEDRASTERIZERSTATE_H_
#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// State
	class RenderDeviceUsedRasterizerState : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedRasterizerState():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedRasterizerState(){};
	};
}

#endif