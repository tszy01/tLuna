#ifndef _TLGUIDEFINE_H_
#define _TLGUIDEFINE_H_

#include "TLVector3.h"
#include "TLVector2.h"
#include "TLVector4.h"

namespace TLunaEngine{

	// ��Ⱦ����ṹ��
	struct GUI_VERTEX_DEF
	{
		TLunaEngine::Vector3<float> Pos;
		TLunaEngine::Vector2<float> Tex;
		TLunaEngine::Vector4<float> Color;
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

#endif