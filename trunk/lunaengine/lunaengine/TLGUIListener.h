#ifndef _TLGUILISTENER_H_
#define _TLGUILISTENER_H_
#include "TSCommonTypeDef.h"

namespace TLunaEngine{
	/*
	 *	UI返回消息调用接口，继承者需要重写消息捕获函数，由具体的Container返回
	 *	这个类只有接口函数定义，不能实现任何处理，不能添加任何成员变量，是纯接口
	 */
	class GUIListener
	{
	public:

		GUIListener(TSun::TVOID)
		{
		}

		virtual ~GUIListener(TSun::TVOID)
		{
		}

	public:
		// 动画播放完毕发送
		virtual TSun::TVOID OnAnimePlayedOver(TSun::TS32 iContainerID,TSun::TUByte yAnimeType) = 0;
	};

}

#endif