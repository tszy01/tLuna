#ifndef _TLRENDERDEVICEUSEDPS_H_
#define _TLRENDERDEVICEUSEDPS_H_

#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// Shader
	class RenderDeviceUsedPS : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedPS():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedPS(){};
	};
}

#endif