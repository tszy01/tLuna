#ifndef TL_RENDER_MGR_H
#define TL_RENDER_MGR_H

#include "TLCommonTypeDef.h"
#include "TLSingleton.h"

namespace TLunaEngine
{
	class RenderDevice;

	class RenderMgr : public Singleton<RenderMgr>
	{
		friend class Singleton<RenderMgr>;
	public:
		enum DEVEICE_TYPE
		{
			DT_NONE,
			DT_D3D11,
		};
	protected:
		RenderMgr();
		~RenderMgr();
	private:
		// 保存创建的Device
		RenderDevice* mDevice;
		// Used Device Type
		DEVEICE_TYPE mDeviceType;
	public:
		// create render device
		// only new device, please call init after this
		RenderDevice* createRenderDevice(TU32 deviceType);
		// destroy render device
		// only delete device, please call destroy before this
		TVOID destroyRenderDevice();
		// get the device
		RenderDevice* getDevice()
		{
			return mDevice;
		};
		// resize device buffer
		TBOOL resizeDeviceBuffer(TU32 width,TU32 height);
		// get device type
		DEVEICE_TYPE getDeviceType()
		{
			return mDeviceType;
		}
	};
}

#endif