#pragma once


namespace TLunaEngine{
	/*
	 *	UI������Ϣ���ýӿڣ��̳�����Ҫ��д��Ϣ���������ɾ����Container����
	 *	�����ֻ�нӿں������壬����ʵ���κδ�����������κγ�Ա�������Ǵ��ӿ�
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
		// ����������Ϸ���
		virtual void OnAnimePlayedOver(int iContainerID,BYTE yAnimeType) = 0;
	};

}