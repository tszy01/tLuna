#ifndef _TLRENDERDEVICEUSED_H_
#define _TLRENDERDEVICEUSED_H_

namespace TLunaEngine
{
	// 设备使用
	class RenderDeviceUsed
	{
	public:
		RenderDeviceUsed(){};
		virtual ~RenderDeviceUsed(){};
	public:
		// destroy used resource
		//virtual void release() = 0;
	};
}

#endif