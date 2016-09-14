#ifndef _TLIRENDERDEVICEUSEDGS_H_
#define _TLIRENDERDEVICEUSEDGS_H_

#include "TLIRenderDeviceUsed.h"

namespace TLunaEngine
{
	// Shader
	class TLIRenderDeviceUsedGS : public TLIRenderDeviceUsed
	{
	public:
		TLIRenderDeviceUsedGS():TLIRenderDeviceUsed(){};
		virtual ~TLIRenderDeviceUsedGS(){};
	};
}

#endif