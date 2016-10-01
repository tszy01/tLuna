#ifndef _TLGUIDEFINE_H_
#define _TLGUIDEFINE_H_

#include "TLVector3.h"
#include "TLVector2.h"
#include "TLVector4.h"

namespace TLunaEngine{

	// 渲染输入结构体
	struct GUI_VERTEX_DEF
	{
		TLunaEngine::Vector3<TF32> Pos;
		TLunaEngine::Vector2<TF32> Tex;
		TLunaEngine::Vector4<TF32> Color;
	};

	// Ctrl种类
	enum GUI_CTRL_TYPE
	{
		GUI_CTRL_NONE		= 0,
		GUI_CTRL_PICTURE	= 1,	// 图片类
		GUI_CTRL_TEXT		= 2,	// 文字类
	};

	// 容器动画定义枚举
	enum CONTAINER_ANIME_TYPE_ENUM
	{
		CATE_NONE	= -1,
		CATE_FADE	= 0,
		CATE_PIC_CHANGE	= 1,
		CATE_POS_CHANGE	= 2,

		CATE_COUNT
	};

	// 容器动画类型
	enum CONTAINER_ANIME_TYPE
	{
		CAT_NONE	= 0,
		CAT_FADE	= 1 << CATE_FADE,	// 淡入淡出效果
		CAT_PIC_CHANGE	= 1 << CATE_PIC_CHANGE,	// 图片帧动画
		CAT_POS_CHANGE	= 1 << CATE_POS_CHANGE,	// 位置帧动画
	};

	// 淡入淡出类型
	enum CONTAINER_FADE_TYPE
	{
		CFF_NONE	= -1,
		CFF_FADE_IN	= 0,	// 淡入
		CFF_FADE_OUT= 1,	// 淡出
		CFF_FADE_IN_OUT	= 2,// 淡入淡出
	};

}

#endif