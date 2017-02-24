#ifndef _TLRENDERDEVICEUSEDBUFFER_H_
#define _TLRENDERDEVICEUSEDBUFFER_H_

#include "TLRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// buffer class
	class RenderDeviceUsedBuffer : public RenderDeviceUsedResource
	{
	public:
		RenderDeviceUsedBuffer():RenderDeviceUsedResource(){};
		virtual ~RenderDeviceUsedBuffer(){};
	public:
		// get resource type
		virtual TSun::TVOID getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType)
		{
			if(pType)
			{
				*pType = RENDER_DEVICE_RESOURCE_DIMENSION_BUFFER;
			}
		}
	};
}

#endif