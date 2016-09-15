#ifndef _TLGUITEXTUREMGR_H_
#define _TLGUITEXTUREMGR_H_

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TLSingleton.h"
#include <map>

namespace TLunaEngine{
	class RenderDeviceUsedBuffer;
	class RenderDeviceUsedSRV;
	class RenderDeviceUsedVS;
	class RenderDeviceUsedPS;
	class RenderDeviceUsedGS;
	class RenderDeviceUsedDepthStencilState;
	class RenderDeviceUsedBlendState;
	class RenderDeviceUsedSamplerState;
	class RenderDeviceUsedInputLayout;
	/*
	 *	����GUI����Ⱦ��ͳһ������Դ
	 */
	class GUITextureMgr : public Singleton<GUITextureMgr>
	{
		friend class Singleton<GUITextureMgr>;
	protected:
		GUITextureMgr(void);
		~GUITextureMgr(void);
	private:
		TU32 m_bufferWidth;	// �󻺳��
		TU32 m_bufferHeight;// �󻺳��
		RenderDeviceUsedBuffer* mVB;
		RenderDeviceUsedBuffer* mVBSet;
		RenderDeviceUsedVS* mVS;
		RenderDeviceUsedPS* mPS;
		RenderDeviceUsedDepthStencilState* mDepthStencilState;
		RenderDeviceUsedBlendState* mBlendState;
		RenderDeviceUsedSamplerState* mSamplerState;
		RenderDeviceUsedInputLayout* mInputLayout;
		std::map<int,RenderDeviceUsedSRV*> mSRVList;
	public:
		// ��ʼ��
		TBOOL Init(TU32 bufferWidth,TU32 bufferHeight,const char* effectFile);
		// ��������
		TBOOL LoadTexFromFile(const char* file);
		// ����
		void DestroyAllTex();
		// ��Ⱦ
		// ����滭�����������Ļ�ģ��������괫����������ӳ��ֵ��0-1��
		bool DrawGUICtrl(TS32 x,TS32 y,TS32 width,TS32 height,float texX,float texY,float texR,float texB,int texId,float alpha);
	private:
		// ����D3D��Դ
		TBOOL InitD3DObj(const char* effectFile);
		// ��ȡ����ID���ļ���
		TBOOL LoadTexArray(FILE* stream);
	};

}

#endif