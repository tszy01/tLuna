#ifndef _TLIRENDERDEVICEUSEDBUFFER_H_
#define _TLIRENDERDEVICEUSEDBUFFER_H_

#include "TLIRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// buffer class
	class TLIRenderDeviceUsedBuffer : public TLIRenderDeviceUsedResource
	{
	public:
		TLIRenderDeviceUsedBuffer():TLIRenderDeviceUsedResource(){};
		virtual ~TLIRenderDeviceUsedBuffer(){};
	public:
		// get resource type
		virtual void getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType)
		{
			if(pType)
			{
				*pType = RENDER_DEVICE_RESOURCE_DIMENSION_BUFFER;
			}
		}
	};
}

#endif