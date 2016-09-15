#ifndef _TLRENDERDEVICEUSEDCS_H_
#define _TLRENDERDEVICEUSEDCS_H_

#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// Shader
	class RenderDeviceUsedCS : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedCS():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedCS(){};
	};
}

#endif