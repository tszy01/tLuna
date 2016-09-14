#pragma once

namespace TLunaEngine{

	/*
	 *	��Դ����״̬
	 */
	enum RES_OBJ_STATE
	{
		RES_OBJ_STATE_NONE	= 0,		// ��ǰû���κ�״̬
		RES_OBJ_STATE_WAIT_LOAD	= 1,	// ��ǰ�ȴ�����
		RES_OBJ_STATE_WAIT_DESTROY	= 2,// ��ǰ�ȴ�����
		RES_OBJ_STATE_LOADING	= 3,	// ��ǰ���ڼ���
		RES_OBJ_STATE_DESTROYING	= 4,// ��ǰ��������
		RES_OBJ_STATE_WAIT_DELETE	= 5,// ��ǰ�ȴ���������ɾ��
		RES_OBJ_STATE_HAS_LOADED	= 6,// ��ǰ�������
		RES_OBJ_STATE_HAS_DESTROYED	= 7,// ��ǰ�Ѿ�����
	};

	/*
	 *	��Դ����ӿ�
	 *	������Ϸ�п��Կ�����ʵ�嶼��һ����Դ���壬�����ṩ�ӿ����ڼ��غ��ͷ�
	 */
	class TLIResObject
	{
	public:
		TLIResObject(void);
		virtual ~TLIResObject(void);
	public:
		// --------------- ��Ҫ��д�ķ��� -------------------------

		// ������Դ
		virtual bool InitResObject() = 0;
		// �ͷ���Դ
		virtual void DestroyResObject() = 0;

	public:
		// --------------- �ⲿ�ɵ��÷��� -------------------------
		// �õ���ǰ״̬
		inline RES_OBJ_STATE GetResState(){return m_eResState;}
	protected:
		// --------------- ������÷��� ---------------------------
		// ���õ�ǰ״̬
		inline void SetResState(RES_OBJ_STATE eResState){m_eResState = eResState;}
	private:
		// --------------- ��Ա -----------------------------------
		RES_OBJ_STATE m_eResState;	// ��Դ��ǰ״̬
	};

}