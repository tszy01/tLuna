#ifndef _TLGLOBLECLASS_H_
#define _TLGLOBLECLASS_H_

#include "TSString.h"
//#include "TD3DFont.h"
#include "TSSingleton.h"

namespace TSun
{
	class LangDict;
};

namespace TLunaEngine{

	// 类声明
	class TestTriangle;
	class LoopCtrl;
	
	/*
	 *	本类存储全局静态变量，静态方法
	 */
	class GlobleClass : public TSun::Singleton<GlobleClass>
	{
		friend class TSun::Singleton<GlobleClass>;
	protected:
		GlobleClass(TSun::TVOID);
		~GlobleClass(TSun::TVOID);
	public:
		// 临时用 -----------------------
		TestTriangle* m_pTri;
		// 保存资源跟路径
		TSun::String m_strResDir;
		// DEBUG信息使用字体
		//TLunaEngine::TD3DFont* m_pDebugFont;
		// 是否显示DEBUG信息
		TSun::TBOOL m_bShowDebugInfo;
		// 是否为编辑器
		TSun::TBOOL m_bEditor;
		// loop control
		LoopCtrl* mLoopCtrl;
		// language dictionary
		TSun::LangDict* mLangDict;
		// language dictionary file
		TSun::String mLangDictFile;
	public:
		// ----- 方法 -------------------
		// 初始化
		TSun::TBOOL InitGlobleClass();
		// 销毁
		TSun::TVOID DestroyGlobleClass();
		// 帧更新
		TSun::TBOOL OnUpdate(TSun::TF32 fTimeElapsed);
		// 帧渲染
		TSun::TBOOL OnRender(TSun::TF32 fTimeElapsed);
		// update loop control
		TSun::TVOID updateLoopCtrl();
		// get loop control renderable
		TSun::TBOOL getLoopCtrlCanRender();
		// calc fps
		TSun::TVOID calcFPS();
		// get elapsed time
		TSun::TF32 getElapsedTime();
	};

}

#endif