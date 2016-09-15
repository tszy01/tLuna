#ifndef _TLRENDERDEVICEUSEDGS_H_
#define _TLRENDERDEVICEUSEDGS_H_

#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// Shader
	class RenderDeviceUsedGS : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedGS():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedGS(){};
	};
}

#endif