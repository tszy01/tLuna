#pragma once

#include "TVector3.h"
#include "TVector2.h"
#include "TVector4.h"

namespace TLunaEngine{

	// ��Ⱦ����ṹ��
	struct _TLGUI_VERTEX_DEF
	{
		TLunaEngine::TVector3<float> Pos;
		TLunaEngine::TVector2<float> Tex;
		TLunaEngine::TVector4<float> Color;
	};

	// Ctrl����
	enum GUI_CTRL_TYPE
	{
		GUI_CTRL_NONE		= 0,
		GUI_CTRL_PICTURE	= 1,	// ͼƬ��
		GUI_CTRL_TEXT		= 2,	// ������
	};

	// ������������ö��
	enum CONTAINER_ANIME_TYPE_ENUM
	{
		CATE_NONE	= -1,
		CATE_FADE	= 0,
		CATE_PIC_CHANGE	= 1,
		CATE_POS_CHANGE	= 2,

		CATE_COUNT
	};

	// ������������
	enum CONTAINER_ANIME_TYPE
	{
		CAT_NONE	= 0,
		CAT_FADE	= 1 << CATE_FADE,	// ���뵭��Ч��
		CAT_PIC_CHANGE	= 1 << CATE_PIC_CHANGE,	// ͼƬ֡����
		CAT_POS_CHANGE	= 1 << CATE_POS_CHANGE,	// λ��֡����
	};

	// ���뵭������
	enum CONTAINER_FADE_TYPE
	{
		CFF_NONE	= -1,
		CFF_FADE_IN	= 0,	// ����
		CFF_FADE_OUT= 1,	// ����
		CFF_FADE_IN_OUT	= 2,// ���뵭��
	};

}