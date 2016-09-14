#pragma once


namespace TLunaEngine{
	/*
	 *	UI返回消息调用接口，继承者需要重写消息捕获函数，由具体的Container返回
	 *	这个类只有接口函数定义，不能实现任何处理，不能添加任何成员变量，是纯接口
	 */
	class TLIGUIListener
	{
	public:

		TLIGUIListener(void)
		{
		}

		virtual ~TLIGUIListener(void)
		{
		}

	public:
		// 动画播放完毕发送
		virtual void OnAnimePlayedOver(int iContainerID,BYTE yAnimeType) = 0;
	};

}