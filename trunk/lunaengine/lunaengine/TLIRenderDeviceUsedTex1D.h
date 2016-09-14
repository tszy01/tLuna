#ifndef _TLIRENDERDEVICEUSEDTEX1D_H_
#define _TLIRENDERDEVICEUSEDTEX1D_H_

#include "TLIRenderDeviceUsedResource.h"

namespace TLunaEngine
{
	// texture 1d class
	class TLIRenderDeviceUsedTex1D : public TLIRenderDeviceUsedResource
	{
	public:
		TLIRenderDeviceUsedTex1D():TLIRenderDeviceUsedResource(){};
		virtual ~TLIRenderDeviceUsedTex1D(){};
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