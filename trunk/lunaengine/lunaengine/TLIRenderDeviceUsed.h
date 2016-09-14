#ifndef _TLIRENDERDEVICEUSED_H_
#define _TLIRENDERDEVICEUSED_H_

namespace TLunaEngine
{
	// 设备使用
	class TLIRenderDeviceUsed
	{
	public:
		TLIRenderDeviceUsed(){};
		virtual ~TLIRenderDeviceUsed(){};
	public:
		// destroy used resource
		//virtual void release() = 0;
	};
}

#endif