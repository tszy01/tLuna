#ifndef _TLRENDERDEVICEUSEDINPUTLAYOUT_H_
#define _TLRENDERDEVICEUSEDINPUTLAYOUT_H_

#include "TLRenderDeviceUsed.h"

namespace TLunaEngine
{
	// InputLayout�ӿ�
	class RenderDeviceUsedInputLayout : public RenderDeviceUsed
	{
	public:
		RenderDeviceUsedInputLayout():RenderDeviceUsed(){};
		virtual ~RenderDeviceUsedInputLayout(){};
	};
}

#endif