#ifndef _TLRENDERDEVICEUSEDRESOURCE_H_
#define _TLRENDERDEVICEUSEDRESOURCE_H_

#include "TLRenderDeviceUsed.h"
#include "TLRenderDef.h"

namespace TLunaEngine
{
	// Resource
	class RenderDeviceUsedResource : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedResource():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedResource(){};
	public:
		// get resource type
		virtual void getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType) = 0;
	};
}

#endif