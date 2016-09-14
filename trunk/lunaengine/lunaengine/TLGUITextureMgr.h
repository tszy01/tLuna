#pragma once

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TSingleton.h"
#include <map>
using namespace TLunaEngine;
using namespace std;

namespace TLunaEngine{
	class TLIRenderDeviceUsedBuffer;
	class TLIRenderDeviceUsedSRV;
	class TLIRenderDeviceUsedVS;
	class TLIRenderDeviceUsedPS;
	class TLIRenderDeviceUsedGS;
	class TLIRenderDeviceUsedDepthStencilState;
	class TLIRenderDeviceUsedBlendState;
	class TLIRenderDeviceUsedSamplerState;
	class TLIRenderDeviceUsedInputLayout;
	/*
	 *	����GUI����Ⱦ��ͳһ������Դ
	 */
	class TLGUITextureMgr
	{
	T_SINGLETON_DEF(TLGUITextureMgr);
	public:
		TLGUITextureMgr(void);
		~TLGUITextureMgr(void);
	private:
		UINT m_bufferWidth;	// �󻺳��
		UINT m_bufferHeight;// �󻺳��
		TLIRenderDeviceUsedBuffer* mVB;
		TLIRenderDeviceUsedBuffer* mVBSet;
		TLIRenderDeviceUsedVS* mVS;
		TLIRenderDeviceUsedPS* mPS;
		TLIRenderDeviceUsedDepthStencilState* mDepthStencilState;
		TLIRenderDeviceUsedBlendState* mBlendState;
		TLIRenderDeviceUsedSamplerState* mSamplerState;
		TLIRenderDeviceUsedInputLayout* mInputLayout;
		std::map<int,TLIRenderDeviceUsedSRV*> mSRVList;
	public:
		// ��ʼ��
		BOOL Init(UINT bufferWidth,UINT bufferHeight,const char* effectFile);
		// ��������
		BOOL LoadTexFromFile(const char* file);
		// ����
		void DestroyAllTex();
		// ��Ⱦ
		// ����滭�����������Ļ�ģ��������괫����������ӳ��ֵ��0-1��
		bool DrawGUICtrl(LONG x,LONG y,LONG width,LONG height,float texX,float texY,float texR,float texB,int texId,float alpha);
	private:
		// ����D3D��Դ
		BOOL InitD3DObj(const char* effectFile);
		// ��ȡ����ID���ļ���
		BOOL LoadTexArray(FILE* stream);
	};

}