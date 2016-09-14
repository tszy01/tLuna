#ifndef _TLIRENDERDEVICEUSEDRESOURCE_H_
#define _TLIRENDERDEVICEUSEDRESOURCE_H_

#include "TLIRenderDeviceUsed.h"
#include "TLRenderDef.h"

namespace TLunaEngine
{
	// Resource
	class TLIRenderDeviceUsedResource : public TLIRenderDeviceUsed
	{
	public:
		TLIRenderDeviceUsedResource():TLIRenderDeviceUsed(){};
		virtual ~TLIRenderDeviceUsedResource(){};
	public:
		// get resource type
		virtual void getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType) = 0;
	};
}

#endif