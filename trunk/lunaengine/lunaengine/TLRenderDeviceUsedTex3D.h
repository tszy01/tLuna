#ifndef _TLRENDERDEVICEUSEDTEX3D_H_
#define _TLRENDERDEVICEUSEDTEX3D_H_

#include "TLRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// texture 3d class
	class RenderDeviceUsedTex3D : public RenderDeviceUsedResource
	{
	public:
		RenderDeviceUsedTex3D():RenderDeviceUsedResource(){};
		virtual ~RenderDeviceUsedTex3D(){};
	public:
		// get resource type
		virtual TVOID getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType)
		{
			if(pType)
			{
				*pType = RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE3D;
			}
		}
	};
}

#endif