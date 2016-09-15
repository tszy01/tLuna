#ifndef _TLRENDERDEVICEUSEDDS_H_
#define _TLRENDERDEVICEUSEDDS_H_

#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// Shader
	class RenderDeviceUsedDS : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedDS():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedDS(){};
	};
}

#endif