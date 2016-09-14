#include "StdAfx.h"
#include "TLGUIMgr.h"
#include "TTxtFileReader.h"
#include "TString.h"
#include "TLog.h"
using namespace TLunaEngine;

namespace TLunaEngine{
	T_SINGLETON_IMP(TLGUIMgr);

	TLGUIMgr::TLGUIMgr(void) : m_pRootContainer(NULL),m_pSceneListener(NULL)
	{
	}

	TLGUIMgr::~TLGUIMgr(void)
	{
		SAFE_DELETE(m_pRootContainer);
	}

	bool TLGUIMgr::LoadFromFile(const char *file)
	{
		FILE* stream = NULL;
		TString strFile(file);
		if (!TTxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return FALSE;
		}
		// ƥ���һ���ַ�
		bool bEqual = false;
		char strCmp[16] = {0};
		sprintf_s(strCmp,16,"TUI_CTRL_100");
		char strResult[1024] = {0};
		int nCount = 1024;
		if(!TTxtFileReader::ReadLineString(strResult,stream,strCmp,&bEqual,nCount,NULL))
		{
			TTxtFileReader::CloseTxtFile(stream);
			return false;
		}
		if(!bEqual)
		{
			TTxtFileReader::CloseTxtFile(stream);
			return false;
		}
		// ��ʼ��������
		if (m_pRootContainer)
		{
			TLog::WriteLine(TLog::LOG_LEVEL_ERROR,true,__FILE__,__LINE__,"�Ѿ��и�������");
			return false;
		}
		if (!LoadContainer(stream,NULL,&m_pRootContainer))
		{
			TLog::WriteLine(TLog::LOG_LEVEL_ERROR,true,__FILE__,__LINE__,"����ʧ��");
			TTxtFileReader::CloseTxtFile(stream);
			return false;
		}
		TTxtFileReader::CloseTxtFile(stream);
		return true;
	}

	bool TLGUIMgr::LoadContainer(FILE* stream,TLGUIContainer* pParentContainer,TLGUIContainer** ppContainer)
	{
		if (!stream || !ppContainer)
		{
			return false;
		}
		// ׼������
		int containerID = -1;
		int rc[4] = {0};
		int ctrlNum = 0;
		int subContainerNum = 0;
		int iAnimeType = 0;
		// ��ʱ����
		char strResult[1024] = {0};
		int nCount = 1024;
		bool bEqual = false;
		// Container_Begin
		TTxtFileReader::ReadLineString(strResult,stream,"Container_Begin",&bEqual,1024,NULL);
		if (!bEqual)
		{
			return false;
		}
		// Id
		TTxtFileReader::ReadLineInteger(&containerID,stream,1,' ');
		if (containerID < 0)
		{
			return false;
		}
		// Rect
		TTxtFileReader::ReadLineInteger(rc,stream,4,',');
		// AnimeType
		TTxtFileReader::ReadLineInteger(&iAnimeType,stream,1,' ');
		// ����Container
		TLGUIContainer* pNewContainer = new TLGUIContainer();
		(*ppContainer) = pNewContainer;
		// ����һ����һ��
		// ���ʧ�ܣ�ͳһ���٣���ֹ�ڴ�й©
		if (pParentContainer)
		{
			pParentContainer->AddContainer(pNewContainer);
		}
		if (!pNewContainer->InitContainer(containerID,pParentContainer,(LONG)rc[0],(LONG)rc[1],(LONG)rc[2],(LONG)rc[3],(BYTE)iAnimeType,this))
		{
			return false;
		}
		// SubCtrl
		TTxtFileReader::ReadLineInteger(&ctrlNum,stream,1,' ');
		for (int i=0;i<ctrlNum;i++)
		{
			TLGUICtrl* pSubCtrl = NULL;
			if (!LoadCtrl(stream,pNewContainer,&pSubCtrl))
			{
				return false;
			}
		}
		// SubContainer
		TTxtFileReader::ReadLineInteger(&subContainerNum,stream,1,' ');
		for (int i=0;i<subContainerNum;i++)
		{
			TLGUIContainer* pSubContainer = NULL;
			if (!LoadContainer(stream,pNewContainer,&pSubContainer))
			{
				return false;
			}
		}
		// ���ݶ������ͼ��ض���
		if (!LoadAnime(stream,pNewContainer,(BYTE)iAnimeType))
		{
			return false;
		}
		// Container_End
		TTxtFileReader::ReadLineString(strResult,stream,"Container_End",&bEqual,1024,NULL);
		return true;
	}

	bool TLGUIMgr::LoadAnime(FILE* stream,TLGUIContainer* pNewContainer,BYTE yAnimeType)
	{
		if (!pNewContainer || !stream)
		{
			return false;
		}
		if (yAnimeType != 0)
		{
			float fChangedPerSec = 0;
			int nAnimeTimes = 0;
			if (yAnimeType & CAT_FADE)
			{
				// ���뵭��
				int iFadeType = -1;
				TTxtFileReader::ReadLineInteger(&iFadeType,stream,1,' ');
				TTxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TTxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitFadeAnime((CONTAINER_FADE_TYPE)iFadeType,fChangedPerSec,nAnimeTimes))
				{
					return false;
				}
			}
			if (yAnimeType & CAT_PIC_CHANGE)
			{
				// ͼƬ֡����
				int iStartIndex = -1;
				int iEndIndex = -1;
				TTxtFileReader::ReadLineInteger(&iStartIndex,stream,1,' ');
				TTxtFileReader::ReadLineInteger(&iEndIndex,stream,1, ' ');
				TTxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TTxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitPicChangeAnime(iStartIndex,iEndIndex,fChangedPerSec,nAnimeTimes))
				{
					return false;
				}
			}
			if (yAnimeType & CAT_POS_CHANGE)
			{
				// λ��֡����
				LONG iStartPos[2] = {0};
				LONG iEndPos[2] = {0};
				TTxtFileReader::ReadLineLong(iStartPos,stream,2,',');
				TTxtFileReader::ReadLineLong(iEndPos,stream,2,',');
				TTxtFileReader::ReadLineFloat(&fChangedPerSec,stream,1,' ');
				TTxtFileReader::ReadLineInteger(&nAnimeTimes,stream,1,' ');
				if (!pNewContainer->InitPosChangeAnime(iStartPos[0],iStartPos[1],iEndPos[0],iEndPos[1],fChangedPerSec,nAnimeTimes))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool TLGUIMgr::LoadCtrl(FILE *stream, TLunaEngine::TLGUIContainer *pParentContainer, TLunaEngine::TLGUICtrl **ppCtrl)
	{
		if (!stream || !ppCtrl || !pParentContainer)
		{
			return false;
		}
		// ׼������
		int index = -1;
		int rc[4] = {0};
		int type = 0;
		// ��ʱ����
		char strResult[1024] = {0};
		int nCount = 1024;
		bool bEqual = false;
		// Container_Begin
		TTxtFileReader::ReadLineString(strResult,stream,"Ctrl_Begin",&bEqual,1024,NULL);
		if (!bEqual)
		{
			return false;
		}
		// Id
		TTxtFileReader::ReadLineInteger(&index,stream,1,' ');
		if (index < 0)
		{
			return false;
		}
		// Rect
		TTxtFileReader::ReadLineInteger(rc,stream,4,',');
		// Type
		TTxtFileReader::ReadLineInteger(&type,stream,1,' ');
		// ����Ctrl
		if (type == GUI_CTRL_PICTURE)
		{
			TLGUICtrl* pNewCtrl = new TLGUIPicture();
			(*ppCtrl) = pNewCtrl;
			pParentContainer->AddCtrl(pNewCtrl);
			int texID = -1;
			float texRC[4] = {0};
			// ע������û���ж�TEXID�ĺϷ���
			TTxtFileReader::ReadLineInteger(&texID,stream,1,' ');
			TTxtFileReader::ReadLineFloat(texRC,stream,4,',');
			TLGUIPicture* pPicture = (TLGUIPicture*)pNewCtrl;
			if (!pPicture->InitGUIPicture(index,pParentContainer,(LONG)rc[0],
				(LONG)rc[1],(LONG)rc[2],(LONG)rc[3],texID,texRC[0],texRC[1],texRC[2],texRC[3]))
			{
				return false;
			}
		}
		else if(type == GUI_CTRL_TEXT)
		{
			TLGUICtrl* pNewCtrl = new TLGUIText();
			(*ppCtrl) = pNewCtrl;
			pParentContainer->AddCtrl(pNewCtrl);
			int fontID = -1;
			float fontColor[4] = {0};
			// ע������û���ж�fontID�ĺϷ���
			TTxtFileReader::ReadLineInteger(&fontID,stream,1,' ');
			TTxtFileReader::ReadLineFloat(fontColor,stream,4,',');
			TLGUIText* pText = (TLGUIText*)pNewCtrl;
			if (!pText->InitGUIText(index,pParentContainer,(LONG)rc[0],(LONG)rc[1],(LONG)rc[2],
				(LONG)rc[3],fontID,TLunaEngine::TVector4<float>(fontColor[0],fontColor[1],fontColor[2],fontColor[3])))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		// Ctrl_End
		TTxtFileReader::ReadLineString(strResult,stream,"Ctrl_End",&bEqual,1024,NULL);
		return true;
	}

}