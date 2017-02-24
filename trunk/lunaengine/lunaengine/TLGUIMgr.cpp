#include "TLGUIMgr.h"
#include "TSTxtFileReader.h"
#include "TSString.h"
#include "TSLog.h"

TLunaEngine::GUIMgr* TSun::Singleton<TLunaEngine::GUIMgr>::m_Ptr = 0;

namespace TLunaEngine{
	GUIMgr::GUIMgr(TSun::TVOID) : m_pRootContainer(TSun::TNULL),m_pSceneListener(TSun::TNULL)
	{
	}

	GUIMgr::~GUIMgr(TSun::TVOID)
	{
		if (m_pRootContainer)
		{
			delete m_pRootContainer;
			m_pRootContainer = 0;
		}
	}

	TSun::TBOOL GUIMgr::LoadFromFile(const TSun::TCHAR* file)
	{
		FILE* stream = TSun::TNULL;
		TSun::String strFile(file);
		if (!TSun::TxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return TSun::TFALSE;
		}
		// 匹配第一行字符
		TSun::TBOOL bEqual = TSun::TFALSE;
		TSun::TCHAR strCmp[16] = {0};
		sprintf_s(strCmp,16,"TUI_CTRL_100");
		TSun::TCHAR strResult[1024] = {0};
		TSun::TS32 nCount = 1024;
		if(!TSun::TxtFileReader::ReadLineString(strResult,stream,strCmp,&bEqual,nCount,TSun::TNULL))
		{
			TSun::TxtFileReader::CloseTxtFile(stream);
			return TSun::TFALSE;
		}
		if(!bEqual)
		{
			TSun::TxtFileReader::CloseTxtFile(stream);
			return TSun::TFALSE;
		}
		// 开始加载容器
		if (m_pRootContainer)
		{
			TSun::Log::WriteLine(TSun::Log::LOG_LEVEL_ERROR,TSun::TTRUE,L"Root Container is not null",__FILE__,__LINE__);
			return TSun::TFALSE;
		}
		if (!LoadContainer(stream,TSun::TNULL,&m_pRootContainer))
		{
			TSun::Log::WriteLine(TSun::Log::LOG_LEVEL_ERROR,TSun::TTRUE,L"Load failed",__FILE__,__LINE__);
			TSun::TxtFileReader::CloseTxtFile(stream);
			return TSun::TFALSE;
		}
		TSun::TxtFileReader::CloseTxtFile(stream);
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIMgr::LoadContainer(FILE* stream,GUIContainer* pParentContainer,GUIContainer** ppContainer)
	{
		if (!stream || !ppContainer)
		{
			return TSun::TFALSE;
		}
		// 准备变量
		TSun::TS32 containerID = -1;
		TSun::TS32 rc[4] = {0};
		TSun::TS32 ctrlNum = 0;
		TSun::TS32 subContainerNum = 0;
		TSun::TS32 iAnimeType = 0;
		// 临时变量
		TSun::TCHAR strResult[1024] = {0};
		TSun::TS32 nCount = 1024;
		TSun::TBOOL bEqual = TSun::TFALSE;
		// Container_Begin
		TSun::TxtFileReader::ReadLineString(strResult,stream,"Container_Begin",&bEqual,1024,TSun::TNULL);
		if (!bEqual)
		{
			return TSun::TFALSE;
		}
		// Id
		TSun::TxtFileReader::ReadLineInteger(&containerID,stream,1,' ');
		if (containerID < 0)
		{
			return TSun::TFALSE;
		}
		// Rect
		TSun::TxtFileReader::ReadLineInteger(rc,stream,4,',');
		// AnimeType
		TSun::TxtFileReader::ReadLineInteger(&iAnimeType,stream,1,' ');
		// 生成Container
		GUIContainer* pNewContainer = new GUIContainer();
		(*ppContainer) = pNewContainer;
		if (!pNewContainer->InitContainer(containerID,pParentContainer,(TSun::TS32)rc[0],(TSun::TS32)rc[1],(TSun::TS32)rc[2],(TSun::TS32)rc[3],(TSun::TUByte)iAnimeType,this))
		{
			delete pNewContainer;
			return TSun::TFALSE;
		}
		// 生成一个加一个
		// 如果失败，统一销毁，防止内存泄漏
		if (pParentContainer)
		{
			pParentContainer->AddContainer(pNewContainer);
		}
		// SubCtrl
		TSun::TxtFileReader::ReadLineInteger(&ctrlNum,stream,1,' ');
		for (TSun::TS32 i=0;i<ctrlNum;i++)
		{
			GUICtrl* pSubCtrl = TSun::TNULL;
			if (!LoadCtrl(stream,pNewContainer,&pSubCtrl))
			{
				return TSun::TFALSE;
			}
		}
		// SubContainer
		TSun::TxtFileReader::ReadLineInteger(&subContainerNum,stream,1,' ');
		for (TSun::TS32 i=0;i<subContainerNum;i++)
		{
			GUIContainer* pSubContainer = TSun::TNULL;
			if (!LoadContainer(stream,pNewContainer,&pSubContainer))
			{
				return TSun::TFALSE;
			}
		}
		// 根据动画类型加载动画
		if (!LoadAnime(stream,pNewContainer,(TSun::TUByte)iAnimeType))
		{
			return TSun::TFALSE;
		}
		// Container_End
		TSun::TxtFileReader::ReadLineString(strResult,stream,"Container_End",&bEqual,1024,TSun::TNULL);
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIMgr::LoadAnime(FILE* stream,GUIContainer* pNewContainer,TSun::TUByte yAnimeType)
	{
		if (!pNewContainer || !stream)
		{
			return TSun::TFALSE;
		}
		if (yAnimeType != 0)
		{
			TSun::TF32 fChangedPerSec = 0;
			TSun::TS32 nAnimeTimes = 0;
			if (yAnimeType & CAT_FADE)
			{
				// 淡入淡出
				TSun::TS32 iFadeType = -1;
				TSun::TxtFileReader::ReadLineInteger(&iFadeType,stream,1,' ');
				TSun::TxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TSun::TxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitFadeAnime((CONTAINER_FADE_TYPE)iFadeType,fChangedPerSec,nAnimeTimes))
				{
					return TSun::TFALSE;
				}
			}
			if (yAnimeType & CAT_PIC_CHANGE)
			{
				// 图片帧动画
				TSun::TS32 iStartIndex = -1;
				TSun::TS32 iEndIndex = -1;
				TSun::TxtFileReader::ReadLineInteger(&iStartIndex,stream,1,' ');
				TSun::TxtFileReader::ReadLineInteger(&iEndIndex,stream,1, ' ');
				TSun::TxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TSun::TxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitPicChangeAnime(iStartIndex,iEndIndex,fChangedPerSec,nAnimeTimes))
				{
					return TSun::TFALSE;
				}
			}
			if (yAnimeType & CAT_POS_CHANGE)
			{
				// 位置帧动画
				TSun::TS32 iStartPos[2] = {0};
				TSun::TS32 iEndPos[2] = {0};
				TSun::TxtFileReader::ReadLineInteger(iStartPos,stream,2,',');
				TSun::TxtFileReader::ReadLineInteger(iEndPos,stream,2,',');
				TSun::TxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TSun::TxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitPosChangeAnime(iStartPos[0],iStartPos[1],iEndPos[0],iEndPos[1],fChangedPerSec,nAnimeTimes))
				{
					return TSun::TFALSE;
				}
			}
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIMgr::LoadCtrl(FILE *stream, TLunaEngine::GUIContainer *pParentContainer, TLunaEngine::GUICtrl **ppCtrl)
	{
		if (!stream || !ppCtrl || !pParentContainer)
		{
			return TSun::TFALSE;
		}
		// 准备变量
		TSun::TS32 index = -1;
		TSun::TS32 rc[4] = {0};
		TSun::TS32 type = 0;
		// 临时变量
		TSun::TCHAR strResult[1024] = {0};
		TSun::TS32 nCount = 1024;
		TSun::TBOOL bEqual = TSun::TFALSE;
		// Container_Begin
		TSun::TxtFileReader::ReadLineString(strResult,stream,"Ctrl_Begin",&bEqual,1024,TSun::TNULL);
		if (!bEqual)
		{
			return TSun::TFALSE;
		}
		// Id
		TSun::TxtFileReader::ReadLineInteger(&index,stream,1,' ');
		if (index < 0)
		{
			return TSun::TFALSE;
		}
		// Rect
		TSun::TxtFileReader::ReadLineInteger(rc,stream,4,',');
		// Type
		TSun::TxtFileReader::ReadLineInteger(&type,stream,1,' ');
		// 生成Ctrl
		if (type == GUI_CTRL_PICTURE)
		{
			GUICtrl* pNewCtrl = new GUIPicture();
			(*ppCtrl) = pNewCtrl;
			TSun::TS32 texID = -1;
			TSun::TF32 texRC[4] = {0};
			// 注意这里没有判断TEXID的合法性
			TSun::TxtFileReader::ReadLineInteger(&texID,stream,1,' ');
			TSun::TxtFileReader::ReadLineFloat(texRC,stream,4,',');
			GUIPicture* pPicture = (GUIPicture*)pNewCtrl;
			if (!pPicture->InitGUIPicture(index,pParentContainer,(TSun::TS32)rc[0],
				(TSun::TS32)rc[1],(TSun::TS32)rc[2],(TSun::TS32)rc[3],texID,texRC[0],texRC[1],texRC[2],texRC[3]))
			{
				delete pNewCtrl;
				return TSun::TFALSE;
			}
			pParentContainer->AddCtrl(pNewCtrl);
		}
		else if(type == GUI_CTRL_TEXT)
		{
			GUICtrl* pNewCtrl = new GUIText();
			(*ppCtrl) = pNewCtrl;
			TSun::TS32 fontID = -1;
			TSun::TF32 fontColor[4] = {0};
			TSun::TU64 textID = 0;
			// 注意这里没有判断fontID的合法性
			TSun::TxtFileReader::ReadLineInteger(&fontID,stream,1,' ');
			TSun::TxtFileReader::ReadLineFloat(fontColor,stream,4,',');
			TSun::TxtFileReader::ReadLineULongLong(&textID, stream, 1, ' ');
			GUIText* pText = (GUIText*)pNewCtrl;
			if (!pText->InitGUIText(index,pParentContainer,(TSun::TS32)rc[0],(TSun::TS32)rc[1],(TSun::TS32)rc[2],
				(TSun::TS32)rc[3],fontID,TSun::Vector4<TSun::TF32>(fontColor[0],fontColor[1],fontColor[2],fontColor[3]), textID))
			{
				delete pNewCtrl;
				return TSun::TFALSE;
			}
			pParentContainer->AddCtrl(pNewCtrl);
		}
		else
		{
			return TSun::TFALSE;
		}
		// Ctrl_End
		TSun::TxtFileReader::ReadLineString(strResult,stream,"Ctrl_End",&bEqual,1024,TSun::TNULL);
		return TSun::TTRUE;
	}

}