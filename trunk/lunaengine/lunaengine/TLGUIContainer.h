#ifndef _TLGUICONTAINER_H_
#define _TLGUICONTAINER_H_

#include "TLGUIDefine.h"
#include <map>
#include "TLGUICtrl.h"
#include <list>
#include <vector>
namespace TLunaEngine{

	class GUICtrl;
	class GUIContainer;
	class GUIListener;

	/*
	 *	容器类，可以被继承，容器包含其它容器，包含控件
	 */
	class GUIContainer
	{
	public:
		GUIContainer(TVOID);
		~GUIContainer(TVOID);
	private:
		std::map<TS32,GUIContainer*> m_SubContainerTable;		// 子容器链表
		std::vector<GUICtrl*> m_CtrlList;								// 控件链表
		TS32 m_iID;													// 容器ID
		GUIContainer* m_pParent;									// 父容器
		GUIListener* m_pListener;								// 消息上层捕获
		TBOOL m_bShow;												// 是否显示

		// ------ 以下是相对于父容器的位置和大小 --------
		// 父容器如果没有，那就是相对于整个窗口的
		TS32 m_posX;
		TS32 m_posY;
		TS32 m_width;
		TS32 m_height;

		// ------ 以下是计算出的绝对位置和大小 ----------
		TS32 m_posXParent;
		TS32 m_posYParent;
		TS32 m_posXFinal;
		TS32 m_posYFinal;
		TBOOL m_bReCal;		// 是否重新计算

		// ----- 以下是动画扩展部分 ---------------------
		// ----- 除了AnimeType剩下的都不构造了和析构了 --
		// 动画种类
		// 这是个枚举并集，可能含有多种动画
		TUByte m_yAnimeType;
		// 动画播放次数
		// -1无限循环，0停在第一帧，播放完后停在最后一帧
		// CATE_COUNT 0为淡入淡出，1为图片动画，2为位置动画
		TS32 m_nAnimeTimes[CATE_COUNT];
		// 当前动画是第几次播放，从0开始，播完一次+1
		TS32 m_iNowAnimeTimes[CATE_COUNT];
		// 每秒变换的浮点数值
		// 每秒ALPHA变化值 -- 淡入淡出效果
		// 每秒图片变换几个 -- 图片帧动画
		// 每秒位置改变几个像素 -- 位置帧动画
		TF32 m_nAnimeChangedPerSec[CATE_COUNT];
		// 每秒计数变化
		// 这个计数根据类型不同，重置的方式也不同，要做到满足最小单位+1时才重置
		TF32 m_nAnimeChangedTimeCount[CATE_COUNT];
		// 动画是否播放完成
		TBOOL m_bAnimePlayedOver[CATE_COUNT];

		// ------ 以下是淡入淡出相关 --------------------
		// 淡入淡出种类
		CONTAINER_FADE_TYPE m_eAnimeFadeType;
		// 当前正在哪种状态
		// -1未在动画中，0淡入，1淡出
		TS32 m_iNowFadeType;
		// 当前的ALPHA值
		TF32 m_fNowAlpha;
		// ----- 图片帧动画相关 -------------------------
		// 起始控件索引
		TS32 m_iPicChangeStartIndex;
		// 结束控件索引，和开始之间要是连续的
		TS32 m_iPicChangeEndIndex;
		// 现在播放的控件索引
		// -1未在动画中
		TS32 m_iNowPicChangeIndex;
		// ---- 位置帧动画 ------------------------------
		// 开始位置
		TS32 m_posChangeStartX;
		TS32 m_posChangeStartY;
		// 结束位置
		TS32 m_posChangeEndX;
		TS32 m_posChangeEndY;
	public:
		// ------ 以下是方法 ----------------------------
		// 初始化
		TBOOL InitContainer(TS32 ID,GUIContainer* pParent,TS32 x,TS32 y,TS32 width,TS32 height,TUByte yAnimeType,GUIListener* pListener);
		// 初始化动画
		TBOOL InitFadeAnime(CONTAINER_FADE_TYPE eFadeType, TF32 fChangedPerSec,TS32 nTimes);
		TBOOL InitPicChangeAnime(TS32 iStartIndex,TS32 iEndIndex, TF32 fChangedPerSec,TS32 nTimes);
		TBOOL InitPosChangeAnime(TS32 startX,TS32 startY,TS32 endX,TS32 endY, TF32 fChangedPerSec,TS32 nTimes);
		// 重置动画
		TVOID ResetFadeAnime();
		TVOID ResetPicChangeAnime();
		TVOID ResetPosChangeAnime();
		// 更新帧计算动画
		TVOID UpdateFadeAnime(TF32 fTimeElapsed);
		TVOID UpdatePicChangeAnime(TF32 fTimeElapsed);
		TVOID UpdatePosChangeAnime(TF32 fTimeElapsed);
		// 重新计算子控件绝对位置
		TVOID ReCalSubRect(TS32 parentFinalX,TS32 parentFinalY);
		// 添加控件
		TBOOL AddCtrl(GUICtrl* pCtrl);
		// 添加容器
		TBOOL AddContainer(GUIContainer* pContainer);
		// 删除容器
		TVOID RemoveSubContainer(TS32 ID);
		// 得到ID
		inline TS32 GetID()
		{
			return m_iID;
		}
		// 得到控件
		inline GUICtrl* GetCtrl(TS32 iIndex)
		{
			GUICtrl* pCtrl = TNULL;
			if(iIndex<0 || iIndex>=(TS32)m_CtrlList.size())
				return pCtrl;
			pCtrl = m_CtrlList[iIndex];
			return pCtrl;
		}
		// 得到容器
		inline GUIContainer* GetSubContainer(TS32 iID)
		{
			GUIContainer* pContainer = TNULL;
			std::map<TS32,TLunaEngine::GUIContainer*>::iterator itr = m_SubContainerTable.find(iID);
			if(itr!=m_SubContainerTable.end())
				return itr->second;
			return pContainer;
		}
		// 递归找容器
		inline GUIContainer* FindContainer(TS32 iID)
		{
			// 先比较自己
			if (m_iID == iID)
			{
				return this;
			}
			// 调用子容器的方法
			std::map<TS32,GUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				GUIContainer* pSubContainer = itr->second;
				GUIContainer* pFind = pSubContainer->FindContainer(iID);
				if (pFind)
				{
					return pFind;
				}
			}
			return TNULL;
		}
		// 删除控件
		inline TVOID ClearCtrls()
		{
			std::vector<GUICtrl*>::iterator itr = m_CtrlList.begin();
			for (;itr!=m_CtrlList.end();itr++)
			{
				GUICtrl* pCtrl = (*itr);
				if (pCtrl)
				{
					delete pCtrl;
					pCtrl = 0;
				}
			}
			m_CtrlList.clear();
		}
		// 删除子容器
		inline TVOID ClearSubContainers()
		{
			std::map<TS32,GUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				GUIContainer* pSubContainer = itr->second;
				if (pSubContainer)
				{
					delete pSubContainer;
					pSubContainer = 0;
				}
			}
			m_SubContainerTable.clear();
		}
		// 是容器重新计算绝对位置
		inline TVOID MakeReCal(TBOOL bReCal = TTRUE)
		{
			m_bReCal = bReCal;
			std::map<TS32,GUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				GUIContainer* pSubContainer = itr->second;
				pSubContainer->MakeReCal(bReCal);
			}
		}
		// 得到是否显示
		inline TBOOL IsShow()
		{
			return m_bShow;
		}
		// 设置是否显示
		TVOID ShowContainer(TBOOL bShow = TTRUE);
		// 设置位置
		// 只能设置位置，不能设置大小
		TVOID SetPosition(TS32 x,TS32 y)
		{
			m_posX = x;
			m_posY = y;
			MakeReCal(TTRUE);
		}
		// 设置控件ALPHA
		TVOID SetCtrlAlpha(TF32 fAlpha);
		// 设置动画类型
		inline TVOID SetAnimeType(TUByte yType)
		{
			m_yAnimeType = yType;
		}
		// 得到动画类型
		inline TUByte GetAnimeType()
		{
			return m_yAnimeType;
		}
		// 动画结束时调用的函数
		TVOID OnAnimePlayedOver();
	public:
		// 销毁
		TVOID DestroyContainer();
		// 更新
		TBOOL Update(TF32 fTimeElapsed);
		// 渲染
		TBOOL Render(TF32 fTimeElapsed);
	};

}

#endif