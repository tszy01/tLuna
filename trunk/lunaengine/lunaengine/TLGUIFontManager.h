#pragma once

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TSingleton.h"
#include <map>
#include "TVector4.h"
using namespace std;
using namespace TLunaEngine;

namespace TLunaEngine{

	// ����ģ��
	template class TVector4<float>;
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
	 *	����ά�����е����壬������ͳһ��Ⱦ
	 */
	class TLGUIFontManager
	{
	T_SINGLETON_DEF(TLGUIFontManager);
	public:
		TLGUIFontManager(void);
		~TLGUIFontManager(void);
	private:
		TLIRenderDeviceUsedBuffer* mVB;
		TLIRenderDeviceUsedBuffer* mVBSet;
		TLIRenderDeviceUsedVS* mVS;
		TLIRenderDeviceUsedPS* mPS;
		TLIRenderDeviceUsedDepthStencilState* mDepthStencilState;
		TLIRenderDeviceUsedBlendState* mBlendState;
		TLIRenderDeviceUsedSamplerState* mSamplerState;
		TLIRenderDeviceUsedInputLayout* mInputLayout;
		TLGUIFont* m_pUseFont;	// ��ǰʹ�õ�����
		TLGUIFont* m_pDebugFont;	// ����ʹ������
		std::map<int,TLGUIFont*> m_FontTable;	// �����б�
		UINT m_bufferWidth;	// �󻺳��
		UINT m_bufferHeight;// �󻺳��
		wchar_t* m_pRenderText;	// ��Ⱦʱ������
		int m_nRenderTextLen;	// ��Ⱦʱ�����ݵĳ���
		FT_Library	library;
	public:
		// ���õ�ǰʹ������
		inline void UseFont(int id)
		{
			// �������ʹ�õľ��ǵ�ǰ����Ͳ���������
			if (m_pUseFont)
			{
				if (m_pUseFont->GetId() == id)
				{
					return;
				}
			}
			m_pUseFont = NULL;
			std::map<int,TLunaEngine::TLGUIFont*>::iterator itr = m_FontTable.find(id);
			if(itr!=m_FontTable.end())
			{
				m_pUseFont = itr->second;
			}
		}
		// ���������
		int AddFont(const char* filename,UINT size,UINT texPageSize,int id);
		// ���ļ��������
		bool AddFontFromFile(const char* filename);
		// ��Ⱦ����
		bool Render(const char* text,size_t len, int x,int y, TVector4<float>& color);
		// ��ʼ��
		bool Init(const char* effectFile,UINT bufferWidth,UINT bufferHeight);
		// ��������
		void DestroyAllFont();
		// ��ʼ����������
		bool initDebugFont(const char* filename, UINT size, UINT texPageSize);
		// ���ٵ�������
		void deleteDebugFont();
		// ��Ⱦ��������
		bool RenderDebugFont(const char* text, size_t len, int x, int y, TVector4<float>& color);
	private:
		// ��ȡ�ļ������ֿ�
		bool LoadFont(FILE* stream);
	};

}