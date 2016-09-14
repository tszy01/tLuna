// compileFx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

void GanerateFiles(std::string& dir,const char* profile)
{
	// �����ļ���
	char szName[4096]={0};
	strcpy(szName,dir.c_str());
	strcat(szName,"\\*.*");

	/*CString strMsg;
	strMsg.Format("����Ŀ¼�����ļ�:%s",szName);
	m_strMsg=strMsg;
	UpdateData(FALSE);*/
	printf("����Ŀ¼�����ļ�:%s\n",szName);

	// ���ҵ�һ���ļ�
	WIN32_FIND_DATAA wfdNewFile;
	HANDLE hNewFile;
	hNewFile=::FindFirstFileA(szName,&wfdNewFile);
	if(hNewFile==INVALID_HANDLE_VALUE)
	{
		printf("�����ļ�ʧ��\n");
		return ;
	}
	// ѭ�����Һ�����ļ�
	do
	{
		// ����..�⼸���ļ���
		if (wfdNewFile.cFileName[0] != '.' && wfdNewFile.cFileName[0] != '$')
		{
			// ����Ƿ�Ϊ�ļ���
			if(wfdNewFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// �ļ���
				std::string nextDir;
				char dirName[4096]={0};
				sprintf_s(dirName,4096,"%s\\%s",dir.c_str(),wfdNewFile.cFileName);
				nextDir = std::string(dirName);
				//nextDir.Format("%s\\%s",dir.GetString(),wfdNewFile.cFileName);

				//strMsg.Format("������һ��Ŀ¼:%s",nextDir);
				//m_strMsg=strMsg;
				//UpdateData(FALSE);
				printf("������һ��Ŀ¼:%s\n",nextDir.c_str());

				// �ڹ�������ļ���
				GanerateFiles(nextDir,profile);
			}
			else
			{
				// �ļ�
				//int newDirLen=m_strNewDir.GetLength();
				//CString newFile;
				//newFile.Format("%s\\%s",dir.GetString(),wfdNewFile.cFileName);
				std::string newFile;
				char theName[1024] = {0};
				sprintf_s(theName,1024,"%s\\%s",dir.c_str(),wfdNewFile.cFileName);

				//strMsg.Format("�ҵ�һ���ļ�:%s",newFile);
				//m_strMsg=strMsg;
				//UpdateData(FALSE);
				printf("�ҵ�һ���ļ�:%s\n",theName);

				// �滻��׺
				std::string theFile(theName);
				std::basic_string <char>::size_type pos = theFile.find(".fx");
				std::basic_string <char>::size_type pos2 = theFile.find(".fxo");
				if(pos!=std::string::npos && pos2==std::string::npos)
				{
					newFile = theFile.substr(0,pos);
					newFile = newFile + ".fxo";
					char szCmd[4096] = {0};
					sprintf_s(szCmd,4096,"fxc /T %s /Fo %s %s",profile,newFile.c_str(),theFile.c_str());
					printf("ִ��:%s",szCmd);
					::system(szCmd);
				}
			}
		}
		
		// ������һ���ļ�
		if(::FindNextFileA(hNewFile,&wfdNewFile)==0)
		{
			if(GetLastError()==ERROR_NO_MORE_FILES)
			{
				//strMsg.Format("��Ŀ¼����");
				//m_strMsg=strMsg;
				//UpdateData(FALSE);
				printf("��Ŀ¼����\n");
			}
			else
			{
				//MessageBox("�����ļ�ʧ��");
			}
			break;
		}
	}while(true);
	// �رղ���
	FindClose(hNewFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	GanerateFiles(std::string(argv[1]),argv[2]);
	while(getchar()!='q'){};
	return 0;
}

