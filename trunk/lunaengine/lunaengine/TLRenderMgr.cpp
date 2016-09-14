#include "stdafx.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice_D3D11.h"

namespace TLunaEngine
{
	T_SINGLETON_IMP(TLRenderMgr);

	TLRenderMgr::TLRenderMgr():mDevice(0),mDeviceType(DT_NONE)
	{
	}

	TLRenderMgr::~TLRenderMgr()
	{
	}

	TLIRenderDevice* TLRenderMgr::createRenderDevice(UINT deviceType)
	{
		if(mDevice!=0)
		{
			return mDevice;
		}
		if(deviceType == DT_D3D11)
		{
			mDevice = new TLRenderDevice_D3D11();
			mDeviceType = DT_D3D11;
			return mDevice;
		}
		else
		{
			return NULL;
		}
		return NULL;
	}

	void TLRenderMgr::destroyRenderDevice()
	{
		if(mDevice)
		{
			delete mDevice;
			mDevice = NULL;
		}
	}

	bool TLRenderMgr::resizeDeviceBuffer(UINT width,UINT height)
	{
		if(mDevice)
		{
			if(!mDevice->resizeBuffer(width,height))
				return false;
		}
		return true;
	}
}