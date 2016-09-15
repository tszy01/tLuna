#ifndef _TLGUIFONTMANAGER_H_
#define _TLGUIFONTMANAGER_H_

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TLSingleton.h"
#include <map>
#include "TLVector4.h"

namespace TLunaEngine{

	// 声明模板
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
	 *	负责维护所有的字体，并处理统一渲染
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
		GUIFont* m_pUseFont;	// 当前使用的字体
		GUIFont* m_pDebugFont;	// 调试使用字体
		std::map<int,GUIFont*> m_FontTable;	// 字体列表
		TU32 m_bufferWidth;	// 后缓冲宽
		TU32 m_bufferHeight;// 后缓冲高
		wchar_t* m_pRenderText;	// 渲染时的内容
		int m_nRenderTextLen;	// 渲染时的内容的长度
		FT_Library	library;
	public:
		// 设置当前使用字体
		inline void UseFont(int id)
		{
			// 如果现在使用的就是当前字体就不再设置了
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
		// 添加新字体
		int AddFont(const char* filename,TU32 size,TU32 texPageSize,int id);
		// 从文件添加字体
		bool AddFontFromFile(const char* filename);
		// 渲染文字
		bool Render(const char* text,size_t len, int x,int y, Vector4<float>& color);
		// 初始化
		bool Init(const char* effectFile,TU32 bufferWidth,TU32 bufferHeight);
		// 销毁所有
		void DestroyAllFont();
		// 初始化调试字体
		bool initDebugFont(const char* filename, TU32 size, TU32 texPageSize);
		// 销毁调试字体
		void deleteDebugFont();
		// 渲染调试字体
		bool RenderDebugFont(const char* text, size_t len, int x, int y, Vector4<float>& color);
	private:
		// 读取文件加载字库
		bool LoadFont(FILE* stream);
	};

}

#endif