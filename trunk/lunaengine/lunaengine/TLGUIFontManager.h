#pragma once

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TSingleton.h"
#include <map>
#include "TVector4.h"
using namespace std;
using namespace TLunaEngine;

namespace TLunaEngine{

	// 声明模板
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
	 *	负责维护所有的字体，并处理统一渲染
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
		TLGUIFont* m_pUseFont;	// 当前使用的字体
		TLGUIFont* m_pDebugFont;	// 调试使用字体
		std::map<int,TLGUIFont*> m_FontTable;	// 字体列表
		UINT m_bufferWidth;	// 后缓冲宽
		UINT m_bufferHeight;// 后缓冲高
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
			m_pUseFont = NULL;
			std::map<int,TLunaEngine::TLGUIFont*>::iterator itr = m_FontTable.find(id);
			if(itr!=m_FontTable.end())
			{
				m_pUseFont = itr->second;
			}
		}
		// 添加新字体
		int AddFont(const char* filename,UINT size,UINT texPageSize,int id);
		// 从文件添加字体
		bool AddFontFromFile(const char* filename);
		// 渲染文字
		bool Render(const char* text,size_t len, int x,int y, TVector4<float>& color);
		// 初始化
		bool Init(const char* effectFile,UINT bufferWidth,UINT bufferHeight);
		// 销毁所有
		void DestroyAllFont();
		// 初始化调试字体
		bool initDebugFont(const char* filename, UINT size, UINT texPageSize);
		// 销毁调试字体
		void deleteDebugFont();
		// 渲染调试字体
		bool RenderDebugFont(const char* text, size_t len, int x, int y, TVector4<float>& color);
	private:
		// 读取文件加载字库
		bool LoadFont(FILE* stream);
	};

}