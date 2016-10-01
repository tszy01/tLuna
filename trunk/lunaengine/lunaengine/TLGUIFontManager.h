#ifndef _TLGUIFONTMANAGER_H_
#define _TLGUIFONTMANAGER_H_

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TLSingleton.h"
#include "TLMap.h"
#include "TLVector4.h"

namespace TLunaEngine{

	// 声明模板
	template class Vector4<TF32>;
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
		GUIFontManager(TVOID);
		~GUIFontManager(TVOID);
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
		Map<TS32,GUIFont*> m_FontTable;	// 字体列表
		TU32 m_bufferWidth;	// 后缓冲宽
		TU32 m_bufferHeight;// 后缓冲高
		TWCHAR* m_pRenderText;	// 渲染时的内容
		TU32 m_nRenderTextLen;	// 渲染时的内容的长度
		FT_Library	library;
	public:
		// 设置当前使用字体
		inline TVOID UseFont(TS32 id)
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
			Map<TS32,TLunaEngine::GUIFont*>::Iterator itr = m_FontTable.find(id);
			if(itr!=m_FontTable.end())
			{
				m_pUseFont = itr->Value;
			}
		}
		// 添加新字体
		TU32 AddFont(const TCHAR* filename,TU32 size,TU32 texPageSize,TS32 id);
		// 从文件添加字体
		TBOOL AddFontFromFile(const TCHAR* filename);
		// 渲染文字
		TBOOL Render(const TCHAR* text,TU32 len, TS32 x,TS32 y, Vector4<TF32>& color);
		// 初始化
		TBOOL Init(const TCHAR* effectFile,TU32 bufferWidth,TU32 bufferHeight);
		// 销毁所有
		TVOID DestroyAllFont();
		// 初始化调试字体
		TBOOL initDebugFont(const TCHAR* filename, TU32 size, TU32 texPageSize);
		// 销毁调试字体
		TVOID deleteDebugFont();
		// 渲染调试字体
		TBOOL RenderDebugFont(const TCHAR* text, TU32 len, TS32 x, TS32 y, Vector4<TF32>& color);
	private:
		// 读取文件加载字库
		TBOOL LoadFont(FILE* stream);
	};

}

#endif