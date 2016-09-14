#pragma once

#include "TLGUIDefine.h"
#include <map>
#include "TLGUICtrl.h"
#include <list>
using namespace std;
namespace TLunaEngine{

	class TLGUICtrl;
	class TLGUIContainer;
	class TLIGUIListener;

	/*
	 *	容器类，可以被继承，容器包含其它容器，包含控件
	 */
	class TLGUIContainer
	{
	public:
		TLGUIContainer(void);
		~TLGUIContainer(void);
	private:
		std::map<int,TLGUIContainer*> m_SubContainerTable;		// 子容器链表
		std::vector<TLGUICtrl*> m_CtrlList;								// 控件链表
		int m_iID;													// 容器ID
		TLGUIContainer* m_pParent;									// 父容器
		TLIGUIListener* m_pListener;								// 消息上层捕获
		bool m_bShow;												// 是否显示

		// ------ 以下是相对于父容器的位置和大小 --------
		// 父容器如果没有，那就是相对于整个窗口的
		LONG m_posX;
		LONG m_posY;
		LONG m_width;
		LONG m_height;

		// ------ 以下是计算出的绝对位置和大小 ----------
		LONG m_posXParent;
		LONG m_posYParent;
		LONG m_posXFinal;
		LONG m_posYFinal;
		bool m_bReCal;		// 是否重新计算

		// ----- 以下是动画扩展部分 ---------------------
		// ----- 除了AnimeType剩下的都不构造了和析构了 --
		// 动画种类
		// 这是个枚举并集，可能含有多种动画
		BYTE m_yAnimeType;
		// 动画播放次数
		// -1无限循环，0停在第一帧，播放完后停在最后一帧
		// CATE_COUNT 0为淡入淡出，1为图片动画，2为位置动画
		int m_nAnimeTimes[CATE_COUNT];
		// 当前动画是第几次播放，从0开始，播完一次+1
		int m_iNowAnimeTimes[CATE_COUNT];
		// 每秒变换的浮点数值
		// 每秒ALPHA变化值 -- 淡入淡出效果
		// 每秒图片变换几个 -- 图片帧动画
		// 每秒位置改变几个像素 -- 位置帧动画
		float m_nAnimeChangedPerSec[CATE_COUNT];
		// 每秒计数变化
		// 这个计数根据类型不同，重置的方式也不同，要做到满足最小单位+1时才重置
		float m_nAnimeChangedTimeCount[CATE_COUNT];
		// 动画是否播放完成
		bool m_bAnimePlayedOver[CATE_COUNT];

		// ------ 以下是淡入淡出相关 --------------------
		// 淡入淡出种类
		CONTAINER_FADE_TYPE m_eAnimeFadeType;
		// 当前正在哪种状态
		// -1未在动画中，0淡入，1淡出
		int m_iNowFadeType;
		// 当前的ALPHA值
		float m_fNowAlpha;
		// ----- 图片帧动画相关 -------------------------
		// 起始控件索引
		int m_iPicChangeStartIndex;
		// 结束控件索引，和开始之间要是连续的
		int m_iPicChangeEndIndex;
		// 现在播放的控件索引
		// -1未在动画中
		int m_iNowPicChangeIndex;
		// ---- 位置帧动画 ------------------------------
		// 开始位置
		LONG m_posChangeStartX;
		LONG m_posChangeStartY;
		// 结束位置
		LONG m_posChangeEndX;
		LONG m_posChangeEndY;
	public:
		// ------ 以下是方法 ----------------------------
		// 初始化
		bool InitContainer(int ID,TLGUIContainer* pParent,LONG x,LONG y,LONG width,LONG height,BYTE yAnimeType,TLIGUIListener* pListener);
		// 初始化动画
		bool InitFadeAnime(CONTAINER_FADE_TYPE eFadeType,float fChangedPerSec,int nTimes);
		bool InitPicChangeAnime(int iStartIndex,int iEndIndex,float fChangedPerSec,int nTimes);
		bool InitPosChangeAnime(LONG startX,LONG startY,LONG endX,LONG endY,float fChangedPerSec,int nTimes);
		// 重置动画
		void ResetFadeAnime();
		void ResetPicChangeAnime();
		void ResetPosChangeAnime();
		// 更新帧计算动画
		void UpdateFadeAnime(float fTimeElapsed);
		void UpdatePicChangeAnime(float fTimeElapsed);
		void UpdatePosChangeAnime(float fTimeElapsed);
		// 重新计算子控件绝对位置
		void ReCalSubRect(LONG parentFinalX,LONG parentFinalY);
		// 添加控件
		bool AddCtrl(TLGUICtrl* pCtrl);
		// 添加容器
		bool AddContainer(TLGUIContainer* pContainer);
		// 删除容器
		void RemoveSubContainer(int ID);
		// 得到ID
		inline int GetID()
		{
			return m_iID;
		}
		// 得到控件
		inline TLGUICtrl* GetCtrl(int iIndex)
		{
			TLGUICtrl* pCtrl = NULL;
			if(iIndex<0 || iIndex>=(int)m_CtrlList.size())
				return pCtrl;
			pCtrl = m_CtrlList[iIndex];
			return pCtrl;
		}
		// 得到容器
		inline TLGUIContainer* GetSubContainer(int iID)
		{
			TLGUIContainer* pContainer = NULL;
			std::map<int,TLunaEngine::TLGUIContainer*>::iterator itr = m_SubContainerTable.find(iID);
			if(itr!=m_SubContainerTable.end())
				return itr->second;
			return pContainer;
		}
		// 递归找容器
		inline TLGUIContainer* FindContainer(int iID)
		{
			// 先比较自己
			if (m_iID == iID)
			{
				return this;
			}
			// 调用子容器的方法
			std::map<int,TLGUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				TLGUIContainer* pSubContainer = itr->second;
				TLGUIContainer* pFind = pSubContainer->FindContainer(iID);
				if (pFind)
				{
					return pFind;
				}
			}
			return NULL;
		}
		// 删除控件
		inline void ClearCtrls()
		{
			std::vector<TLGUICtrl*>::iterator itr = m_CtrlList.begin();
			for (;itr!=m_CtrlList.end();itr++)
			{
				TLGUICtrl* pCtrl = (*itr);
				SAFE_DELETE(pCtrl);
			}
			m_CtrlList.clear();
		}
		// 删除子容器
		inline void ClearSubContainers()
		{
			std::map<int,TLGUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				TLGUIContainer* pSubContainer = itr->second;
				SAFE_DELETE(pSubContainer);
			}
			m_SubContainerTable.clear();
		}
		// 是容器重新计算绝对位置
		inline void MakeReCal(bool bReCal = true)
		{
			m_bReCal = bReCal;
			std::map<int,TLGUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				TLGUIContainer* pSubContainer = itr->second;
				pSubContainer->MakeReCal(bReCal);
			}
		}
		// 得到是否显示
		inline bool IsShow()
		{
			return m_bShow;
		}
		// 设置是否显示
		void ShowContainer(bool bShow = true);
		// 设置位置
		// 只能设置位置，不能设置大小
		void SetPosition(LONG x,LONG y)
		{
			m_posX = x;
			m_posY = y;
			MakeReCal(true);
		}
		// 设置控件ALPHA
		void SetCtrlAlpha(float fAlpha);
		// 设置动画类型
		inline void SetAnimeType(BYTE yType)
		{
			m_yAnimeType = yType;
		}
		// 得到动画类型
		inline BYTE GetAnimeType()
		{
			return m_yAnimeType;
		}
		// 动画结束时调用的函数
		void OnAnimePlayedOver();
	public:
		// 销毁
		void DestroyContainer();
		// 更新
		bool Update(float fTimeElapsed);
		// 渲染
		bool Render(float fTimeElapsed);
	};

}