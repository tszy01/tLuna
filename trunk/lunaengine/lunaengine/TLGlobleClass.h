#ifndef _TLGLOBLECLASS_H_
#define _TLGLOBLECLASS_H_

#include "TLString.h"
//#include "TD3DFont.h"
#include "TLSingleton.h"

namespace TLunaEngine{

	// 类声明
	class TestTriangle;
	class LoopCtrl;
	/*
	 *	本类存储全局静态变量，静态方法
	 */
	class GlobleClass : public Singleton<GlobleClass>
	{
		friend class Singleton<GlobleClass>;
	protected:
		GlobleClass(TVOID);
		~GlobleClass(TVOID);
	public:
		// 临时用 -----------------------
		TestTriangle* m_pTri;
		// 保存资源跟路径
		TLunaEngine::String m_strResDir;
		// DEBUG信息使用字体
		//TLunaEngine::TD3DFont* m_pDebugFont;
		// 是否显示DEBUG信息
		TBOOL m_bShowDebugInfo;
		// 是否为编辑器
		TBOOL m_bEditor;
		// loop control
		LoopCtrl* mLoopCtrl;
	public:
		// ----- 方法 -------------------
		// 初始化
		TBOOL InitGlobleClass();
		// 销毁
		TVOID DestroyGlobleClass();
		// 帧更新
		TBOOL OnUpdate(float fTimeElapsed);
		// 帧渲染
		TBOOL OnRender(float fTimeElapsed);
		// update loop control
		TVOID updateLoopCtrl();
		// get loop control renderable
		TBOOL getLoopCtrlCanRender();
		// calc fps
		TVOID calcFPS();
		// get elapsed time
		float getElapsedTime();
	};

}

#endif