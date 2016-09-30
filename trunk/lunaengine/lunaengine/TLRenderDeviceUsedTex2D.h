#ifndef _TLRENDERDEVICEUSEDTEX2D_H_
#define _TLRENDERDEVICEUSEDTEX2D_H_

#include "TLRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// texture 2d class
	class RenderDeviceUsedTex2D : public RenderDeviceUsedResource
	{
	public:
		RenderDeviceUsedTex2D():RenderDeviceUsedResource(){};
		virtual ~RenderDeviceUsedTex2D(){};
	public:
		// get resource type
		virtual TVOID getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType)
		{
			if(pType)
			{
				*pType = RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE2D;
			}
		}
	};
}

#endif