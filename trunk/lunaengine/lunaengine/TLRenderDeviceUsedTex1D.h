#ifndef _TLRENDERDEVICEUSEDTEX1D_H_
#define _TLRENDERDEVICEUSEDTEX1D_H_

#include "TLRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// texture 1d class
	class RenderDeviceUsedTex1D : public RenderDeviceUsedResource
	{
	public:
		RenderDeviceUsedTex1D():RenderDeviceUsedResource(){};
		virtual ~RenderDeviceUsedTex1D(){};
	public:
		// get resource type
		virtual void getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType)
		{
			if(pType)
			{
				*pType = RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE1D;
			}
		}
	};
}

#endif