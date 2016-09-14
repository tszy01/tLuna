#ifndef TL_RENDER_MGR_H
#define TL_RENDER_MGR_H

#include "TSingleton.h"

namespace TLunaEngine
{
	class TLIRenderDevice;

	class TLRenderMgr
	{
		T_SINGLETON_DEF(TLRenderMgr);
	public:
		enum DEVEICE_TYPE
		{
			DT_NONE,
			DT_D3D11,
		};
	public:
		TLRenderMgr();
		~TLRenderMgr();
	private:
		// 保存创建的Device
		TLIRenderDevice* mDevice;
		// Used Device Type
		DEVEICE_TYPE mDeviceType;
	public:
		// create render device
		// only new device, please call init after this
		TLIRenderDevice* createRenderDevice(UINT deviceType);
		// destroy render device
		// only delete device, please call destroy before this
		void destroyRenderDevice();
		// get the device
		TLIRenderDevice* getDevice()
		{
			return mDevice;
		};
		// resize device buffer
		bool resizeDeviceBuffer(UINT width,UINT height);
		// get device type
		DEVEICE_TYPE getDeviceType()
		{
			return mDeviceType;
		}
	};
}

#endif