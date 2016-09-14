#ifndef _TLIRENDERDEVICEUSEDTEX3D_H_
#define _TLIRENDERDEVICEUSEDTEX3D_H_

#include "TLIRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// texture 3d class
	class TLIRenderDeviceUsedTex3D : public TLIRenderDeviceUsedResource
	{
	public:
		TLIRenderDeviceUsedTex3D():TLIRenderDeviceUsedResource(){};
		virtual ~TLIRenderDeviceUsedTex3D(){};
	public:
		// get resource type
		virtual void getResourceType(RENDER_DEVICE_RESOURCE_DIMENSION* pType)
		{
			if(pType)
			{
				*pType = RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE3D;
			}
		}
	};
}

#endif