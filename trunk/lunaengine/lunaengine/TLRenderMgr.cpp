#include "TLRenderMgr.h"
#include "TLRenderDevice_D3D11.h"

namespace TLunaEngine
{
	RenderMgr* Singleton<RenderMgr>::m_Ptr = 0;

	RenderMgr::RenderMgr():mDevice(0),mDeviceType(DT_NONE)
	{
	}

	RenderMgr::~RenderMgr()
	{
	}

	RenderDevice* RenderMgr::createRenderDevice(TU32 deviceType)
	{
		if(mDevice!=0)
		{
			return mDevice;
		}
		if(deviceType == DT_D3D11)
		{
			mDevice = new RenderDevice_D3D11();
			mDeviceType = DT_D3D11;
			return mDevice;
		}
		else
		{
			return TNULL;
		}
		return TNULL;
	}

	TVOID RenderMgr::destroyRenderDevice()
	{
		if(mDevice)
		{
			delete mDevice;
			mDevice = TNULL;
		}
	}

	TBOOL RenderMgr::resizeDeviceBuffer(TU32 width,TU32 height)
	{
		if(mDevice)
		{
			if(!mDevice->resizeBuffer(width,height))
				return TFALSE;
		}
		return TTRUE;
	}
}