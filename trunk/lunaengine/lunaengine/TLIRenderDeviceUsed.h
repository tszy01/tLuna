#ifndef _TLIRENDERDEVICEUSED_H_
#define _TLIRENDERDEVICEUSED_H_

namespace TLunaEngine
{
	// �豸ʹ��
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