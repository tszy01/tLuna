#ifndef TL_RENDER_MGR_H
#define TL_RENDER_MGR_H

#include "TSCommonTypeDef.h"
#include "TSSingleton.h"

namespace TLunaEngine
{
	class RenderDevice;

	class RenderMgr : public TSun::Singleton<RenderMgr>
	{
		friend class TSun::Singleton<RenderMgr>;
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
		RenderDevice* createRenderDevice(TSun::TU32 deviceType);
		// destroy render device
		// only delete device, please call destroy before this
		TSun::TVOID destroyRenderDevice();
		// get the device
		RenderDevice* getDevice()
		{
			return mDevice;
		};
		// resize device buffer
		TSun::TBOOL resizeDeviceBuffer(TSun::TU32 width,TSun::TU32 height);
		// get device type
		DEVEICE_TYPE getDeviceType()
		{
			return mDeviceType;
		}
	};
}

#endif