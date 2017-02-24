#include "TLRenderMgr.h"
#include "TLRenderDevice_D3D11.h"

TLunaEngine::RenderMgr* TSun::Singleton<TLunaEngine::RenderMgr>::m_Ptr = 0;
namespace TLunaEngine
{
	RenderMgr::RenderMgr():mDevice(0),mDeviceType(DT_NONE)
	{
	}

	RenderMgr::~RenderMgr()
	{
	}

	RenderDevice* RenderMgr::createRenderDevice(TSun::TU32 deviceType)
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
			return TSun::TNULL;
		}
		return TSun::TNULL;
	}

	TSun::TVOID RenderMgr::destroyRenderDevice()
	{
		if(mDevice)
		{
			delete mDevice;
			mDevice = TSun::TNULL;
		}
	}

	TSun::TBOOL RenderMgr::resizeDeviceBuffer(TSun::TU32 width,TSun::TU32 height)
	{
		if(mDevice)
		{
			if(!mDevice->resizeBuffer(width,height))
				return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}
}