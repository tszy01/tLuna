#ifndef _TLGUILISTENER_H_
#define _TLGUILISTENER_H_
#include "TLCommonTypeDef.h"

namespace TLunaEngine{
	/*
	 *	UI������Ϣ���ýӿڣ��̳�����Ҫ��д��Ϣ���������ɾ����Container����
	 *	�����ֻ�нӿں������壬����ʵ���κδ�����������κγ�Ա�������Ǵ��ӿ�
	 */
	class GUIListener
	{
	public:

		GUIListener(void)
		{
		}

		virtual ~GUIListener(void)
		{
		}

	public:
		// ����������Ϸ���
		virtual void OnAnimePlayedOver(int iContainerID,TUByte yAnimeType) = 0;
	};

}

#endif