#ifndef _TLIRENDERDEVICEUSEDTEX2D_H_
#define _TLIRENDERDEVICEUSEDTEX2D_H_

#include "TLIRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// texture 2d class
	class TLIRenderDeviceUsedTex2D : public TLIRenderDeviceUsedResource
	{
	public:
		TLIRenderDeviceUsedTex2D():TLIRenderDeviceUsedResource(){};
		virtual ~TLIRenderDeviceUsedTex2D(){};
	public:
		// get resource type
		virtual void getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType)
		{
			if(pType)
			{
				*pType = RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE2D;
			}
		}
	};
}

#endif