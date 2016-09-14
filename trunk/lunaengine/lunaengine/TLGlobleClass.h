#pragma once

#include "TString.h"
//#include "TD3DFont.h"
#include "TSingleton.h"

using namespace TLunaEngine;

namespace TLunaEngine{

	// 类声明
	class TLTriangle;
	/*
	 *	本类存储全局静态变量，静态方法
	 */
	class TLGlobleClass
	{
	T_SINGLETON_DEF(TLGlobleClass);
	public:
		TLGlobleClass(void);
		~TLGlobleClass(void);
	public:
		// 临时用 -----------------------
		TLTriangle* m_pTri;
		// 保存资源跟路径
		TLunaEngine::TString m_strResDir;
		// DEBUG信息使用字体
		//TLunaEngine::TD3DFont* m_pDebugFont;
		// 是否显示DEBUG信息
		bool m_bShowDebugInfo;
		// 是否为编辑器
		bool m_bEditor;
	public:
		// ----- 方法 -------------------
		// 初始化
		bool InitGlobleClass();
		// 销毁
		void DestroyGlobleClass();
		// 帧更新
		bool OnUpdate(float fTimeElapsed);
		// 帧渲染
		bool OnRender(float fTimeElapsed);
	};

}