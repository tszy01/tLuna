#ifndef _TLGUIFONTMANAGER_H_
#define _TLGUIFONTMANAGER_H_

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TLSingleton.h"
#include <map>
#include "TLVector4.h"

namespace TLunaEngine{

	// ����ģ��
	template class Vector4<float>;
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
	 *	����ά�����е����壬������ͳһ��Ⱦ
	 */
	class GUIFontManager : public Singleton<GUIFontManager>
	{
		friend class Singleton<GUIFontManager>;
	protected:
		GUIFontManager(void);
		~GUIFontManager(void);
	private:
		RenderDeviceUsedBuffer* mVB;
		RenderDeviceUsedBuffer* mVBSet;
		RenderDeviceUsedVS* mVS;
		RenderDeviceUsedPS* mPS;
		RenderDeviceUsedDepthStencilState* mDepthStencilState;
		RenderDeviceUsedBlendState* mBlendState;
		RenderDeviceUsedSamplerState* mSamplerState;
		RenderDeviceUsedInputLayout* mInputLayout;
		GUIFont* m_pUseFont;	// ��ǰʹ�õ�����
		GUIFont* m_pDebugFont;	// ����ʹ������
		std::map<int,GUIFont*> m_FontTable;	// �����б�
		TU32 m_bufferWidth;	// �󻺳��
		TU32 m_bufferHeight;// �󻺳��
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
			m_pUseFont = TNULL;
			std::map<int,TLunaEngine::GUIFont*>::iterator itr = m_FontTable.find(id);
			if(itr!=m_FontTable.end())
			{
				m_pUseFont = itr->second;
			}
		}
		// ���������
		int AddFont(const char* filename,TU32 size,TU32 texPageSize,int id);
		// ���ļ��������
		bool AddFontFromFile(const char* filename);
		// ��Ⱦ����
		bool Render(const char* text,size_t len, int x,int y, Vector4<float>& color);
		// ��ʼ��
		bool Init(const char* effectFile,TU32 bufferWidth,TU32 bufferHeight);
		// ��������
		void DestroyAllFont();
		// ��ʼ����������
		bool initDebugFont(const char* filename, TU32 size, TU32 texPageSize);
		// ���ٵ�������
		void deleteDebugFont();
		// ��Ⱦ��������
		bool RenderDebugFont(const char* text, size_t len, int x, int y, Vector4<float>& color);
	private:
		// ��ȡ�ļ������ֿ�
		bool LoadFont(FILE* stream);
	};

}

#endif