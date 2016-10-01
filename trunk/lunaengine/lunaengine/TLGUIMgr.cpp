#include "TLGUIMgr.h"
#include "TLTxtFileReader.h"
#include "TLString.h"
#include "TLLog.h"

namespace TLunaEngine{
	GUIMgr* Singleton<GUIMgr>::m_Ptr = 0;

	GUIMgr::GUIMgr(TVOID) : m_pRootContainer(TNULL),m_pSceneListener(TNULL)
	{
	}

	GUIMgr::~GUIMgr(TVOID)
	{
		if (m_pRootContainer)
		{
			delete m_pRootContainer;
			m_pRootContainer = 0;
		}
	}

	TBOOL GUIMgr::LoadFromFile(const TCHAR* file)
	{
		FILE* stream = TNULL;
		String strFile(file);
		if (!TxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return TFALSE;
		}
		// 匹配第一行字符
		TBOOL bEqual = TFALSE;
		TCHAR strCmp[16] = {0};
		sprintf_s(strCmp,16,"TUI_CTRL_100");
		TCHAR strResult[1024] = {0};
		TS32 nCount = 1024;
		if(!TxtFileReader::ReadLineString(strResult,stream,strCmp,&bEqual,nCount,TNULL))
		{
			TxtFileReader::CloseTxtFile(stream);
			return TFALSE;
		}
		if(!bEqual)
		{
			TxtFileReader::CloseTxtFile(stream);
			return TFALSE;
		}
		// 开始加载容器
		if (m_pRootContainer)
		{
			Log::WriteLine(Log::LOG_LEVEL_ERROR,TTRUE,__FILE__,__LINE__,"Root Container is not null");
			return TFALSE;
		}
		if (!LoadContainer(stream,TNULL,&m_pRootContainer))
		{
			Log::WriteLine(Log::LOG_LEVEL_ERROR,TTRUE,__FILE__,__LINE__,"Load failed");
			TxtFileReader::CloseTxtFile(stream);
			return TFALSE;
		}
		TxtFileReader::CloseTxtFile(stream);
		return TTRUE;
	}

	TBOOL GUIMgr::LoadContainer(FILE* stream,GUIContainer* pParentContainer,GUIContainer** ppContainer)
	{
		if (!stream || !ppContainer)
		{
			return TFALSE;
		}
		// 准备变量
		TS32 containerID = -1;
		TS32 rc[4] = {0};
		TS32 ctrlNum = 0;
		TS32 subContainerNum = 0;
		TS32 iAnimeType = 0;
		// 临时变量
		TCHAR strResult[1024] = {0};
		TS32 nCount = 1024;
		TBOOL bEqual = TFALSE;
		// Container_Begin
		TxtFileReader::ReadLineString(strResult,stream,"Container_Begin",&bEqual,1024,TNULL);
		if (!bEqual)
		{
			return TFALSE;
		}
		// Id
		TxtFileReader::ReadLineInteger(&containerID,stream,1,' ');
		if (containerID < 0)
		{
			return TFALSE;
		}
		// Rect
		TxtFileReader::ReadLineInteger(rc,stream,4,',');
		// AnimeType
		TxtFileReader::ReadLineInteger(&iAnimeType,stream,1,' ');
		// 生成Container
		GUIContainer* pNewContainer = new GUIContainer();
		(*ppContainer) = pNewContainer;
		// 生成一个加一个
		// 如果失败，统一销毁，防止内存泄漏
		if (pParentContainer)
		{
			pParentContainer->AddContainer(pNewContainer);
		}
		if (!pNewContainer->InitContainer(containerID,pParentContainer,(TS32)rc[0],(TS32)rc[1],(TS32)rc[2],(TS32)rc[3],(TUByte)iAnimeType,this))
		{
			return TFALSE;
		}
		// SubCtrl
		TxtFileReader::ReadLineInteger(&ctrlNum,stream,1,' ');
		for (TS32 i=0;i<ctrlNum;i++)
		{
			GUICtrl* pSubCtrl = TNULL;
			if (!LoadCtrl(stream,pNewContainer,&pSubCtrl))
			{
				return TFALSE;
			}
		}
		// SubContainer
		TxtFileReader::ReadLineInteger(&subContainerNum,stream,1,' ');
		for (TS32 i=0;i<subContainerNum;i++)
		{
			GUIContainer* pSubContainer = TNULL;
			if (!LoadContainer(stream,pNewContainer,&pSubContainer))
			{
				return TFALSE;
			}
		}
		// 根据动画类型加载动画
		if (!LoadAnime(stream,pNewContainer,(TUByte)iAnimeType))
		{
			return TFALSE;
		}
		// Container_End
		TxtFileReader::ReadLineString(strResult,stream,"Container_End",&bEqual,1024,TNULL);
		return TTRUE;
	}

	TBOOL GUIMgr::LoadAnime(FILE* stream,GUIContainer* pNewContainer,TUByte yAnimeType)
	{
		if (!pNewContainer || !stream)
		{
			return TFALSE;
		}
		if (yAnimeType != 0)
		{
			TF32 fChangedPerSec = 0;
			TS32 nAnimeTimes = 0;
			if (yAnimeType & CAT_FADE)
			{
				// 淡入淡出
				TS32 iFadeType = -1;
				TxtFileReader::ReadLineInteger(&iFadeType,stream,1,' ');
				TxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitFadeAnime((CONTAINER_FADE_TYPE)iFadeType,fChangedPerSec,nAnimeTimes))
				{
					return TFALSE;
				}
			}
			if (yAnimeType & CAT_PIC_CHANGE)
			{
				// 图片帧动画
				TS32 iStartIndex = -1;
				TS32 iEndIndex = -1;
				TxtFileReader::ReadLineInteger(&iStartIndex,stream,1,' ');
				TxtFileReader::ReadLineInteger(&iEndIndex,stream,1, ' ');
				TxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitPicChangeAnime(iStartIndex,iEndIndex,fChangedPerSec,nAnimeTimes))
				{
					return TFALSE;
				}
			}
			if (yAnimeType & CAT_POS_CHANGE)
			{
				// 位置帧动画
				TS32 iStartPos[2] = {0};
				TS32 iEndPos[2] = {0};
				TxtFileReader::ReadLineInteger(iStartPos,stream,2,',');
				TxtFileReader::ReadLineInteger(iEndPos,stream,2,',');
				TxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitPosChangeAnime(iStartPos[0],iStartPos[1],iEndPos[0],iEndPos[1],fChangedPerSec,nAnimeTimes))
				{
					return TFALSE;
				}
			}
		}
		return TTRUE;
	}

	TBOOL GUIMgr::LoadCtrl(FILE *stream, TLunaEngine::GUIContainer *pParentContainer, TLunaEngine::GUICtrl **ppCtrl)
	{
		if (!stream || !ppCtrl || !pParentContainer)
		{
			return TFALSE;
		}
		// 准备变量
		TS32 index = -1;
		TS32 rc[4] = {0};
		TS32 type = 0;
		// 临时变量
		TCHAR strResult[1024] = {0};
		TS32 nCount = 1024;
		TBOOL bEqual = TFALSE;
		// Container_Begin
		TxtFileReader::ReadLineString(strResult,stream,"Ctrl_Begin",&bEqual,1024,TNULL);
		if (!bEqual)
		{
			return TFALSE;
		}
		// Id
		TxtFileReader::ReadLineInteger(&index,stream,1,' ');
		if (index < 0)
		{
			return TFALSE;
		}
		// Rect
		TxtFileReader::ReadLineInteger(rc,stream,4,',');
		// Type
		TxtFileReader::ReadLineInteger(&type,stream,1,' ');
		// 生成Ctrl
		if (type == GUI_CTRL_PICTURE)
		{
			GUICtrl* pNewCtrl = new GUIPicture();
			(*ppCtrl) = pNewCtrl;
			pParentContainer->AddCtrl(pNewCtrl);
			TS32 texID = -1;
			TF32 texRC[4] = {0};
			// 注意这里没有判断TEXID的合法性
			TxtFileReader::ReadLineInteger(&texID,stream,1,' ');
			TxtFileReader::ReadLineFloat(texRC,stream,4,',');
			GUIPicture* pPicture = (GUIPicture*)pNewCtrl;
			if (!pPicture->InitGUIPicture(index,pParentContainer,(TS32)rc[0],
				(TS32)rc[1],(TS32)rc[2],(TS32)rc[3],texID,texRC[0],texRC[1],texRC[2],texRC[3]))
			{
				return TFALSE;
			}
		}
		else if(type == GUI_CTRL_TEXT)
		{
			GUICtrl* pNewCtrl = new GUIText();
			(*ppCtrl) = pNewCtrl;
			pParentContainer->AddCtrl(pNewCtrl);
			TS32 fontID = -1;
			TF32 fontColor[4] = {0};
			// 注意这里没有判断fontID的合法性
			TxtFileReader::ReadLineInteger(&fontID,stream,1,' ');
			TxtFileReader::ReadLineFloat(fontColor,stream,4,',');
			GUIText* pText = (GUIText*)pNewCtrl;
			if (!pText->InitGUIText(index,pParentContainer,(TS32)rc[0],(TS32)rc[1],(TS32)rc[2],
				(TS32)rc[3],fontID,TLunaEngine::Vector4<TF32>(fontColor[0],fontColor[1],fontColor[2],fontColor[3])))
			{
				return TFALSE;
			}
		}
		else
		{
			return TFALSE;
		}
		// Ctrl_End
		TxtFileReader::ReadLineString(strResult,stream,"Ctrl_End",&bEqual,1024,TNULL);
		return TTRUE;
	}

}