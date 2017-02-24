#ifndef _TLGUIFONTMANAGER_H_
#define _TLGUIFONTMANAGER_H_

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TSSingleton.h"
#include "TSMap.h"
#include "TSVector4.h"

namespace TLunaEngine{

	// 声明模板
	template class TSun::Vector4<TSun::TF32>;
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
	class GUIFontManager : public TSun::Singleton<GUIFontManager>
	{
		friend class TSun::Singleton<GUIFontManager>;
	protected:
		GUIFontManager(TSun::TVOID);
		~GUIFontManager(TSun::TVOID);
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
		TSun::Map<TSun::TS32,GUIFont*> m_FontTable;	// 字体列表
		TSun::TU32 m_bufferWidth;	// 后缓冲宽
		TSun::TU32 m_bufferHeight;// 后缓冲高
		TSun::TWCHAR* m_pRenderText;	// 渲染时的内容
		TSun::TU32 m_nRenderTextLen;	// 渲染时的内容的长度
		FT_Library	library;
	public:
		// 设置当前使用字体
		inline TSun::TVOID UseFont(TSun::TS32 id)
		{
			// 如果现在使用的就是当前字体就不再设置了
			if (m_pUseFont)
			{
				if (m_pUseFont->GetId() == id)
				{
					return;
				}
			}
			m_pUseFont = TSun::TNULL;
			TSun::Map<TSun::TS32,TLunaEngine::GUIFont*>::Iterator itr = m_FontTable.find(id);
			if(itr!=m_FontTable.end())
			{
				m_pUseFont = itr->Value;
			}
		}
		// 添加新字体
		TSun::TU32 AddFont(const TSun::TCHAR* filename,TSun::TU32 size,TSun::TU32 texPageSize,TSun::TS32 id);
		// 从文件添加字体
		TSun::TBOOL AddFontFromFile(const TSun::TCHAR* filename);
		// 渲染文字
		TSun::TBOOL Render(const TSun::TWCHAR* text,TSun::TU32 len, TSun::TS32 x,TSun::TS32 y, TSun::Vector4<TSun::TF32>& color);
		// 初始化
		TSun::TBOOL Init(const TSun::TCHAR* effectFile,TSun::TU32 bufferWidth,TSun::TU32 bufferHeight);
		// 销毁所有
		TSun::TVOID DestroyAllFont();
		// 初始化调试字体
		TSun::TBOOL initDebugFont(const TSun::TCHAR* filename, TSun::TU32 size, TSun::TU32 texPageSize);
		// 销毁调试字体
		TSun::TVOID deleteDebugFont();
		// 渲染调试字体
		TSun::TBOOL RenderDebugFont(const TSun::TWCHAR* text, TSun::TU32 len, TSun::TS32 x, TSun::TS32 y, TSun::Vector4<TSun::TF32>& color);
	private:
		// 读取文件加载字库
		TSun::TBOOL LoadFont(FILE* stream);
	};

}

#endif