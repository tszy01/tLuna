// compileFx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

void GanerateFiles(std::string& dir,const char* profile)
{
	// 生成文件名
	char szName[4096]={0};
	strcpy(szName,dir.c_str());
	strcat(szName,"\\*.*");

	/*CString strMsg;
	strMsg.Format("查找目录所有文件:%s",szName);
	m_strMsg=strMsg;
	UpdateData(FALSE);*/
	printf("查找目录所有文件:%s\n",szName);

	// 查找第一个文件
	WIN32_FIND_DATAA wfdNewFile;
	HANDLE hNewFile;
	hNewFile=::FindFirstFileA(szName,&wfdNewFile);
	if(hNewFile==INVALID_HANDLE_VALUE)
	{
		printf("查找文件失败\n");
		return ;
	}
	// 循环查找后面的文件
	do
	{
		// 跳过..这几个文件夹
		if (wfdNewFile.cFileName[0] != '.' && wfdNewFile.cFileName[0] != '$')
		{
			// 检测是否为文件夹
			if(wfdNewFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 文件夹
				std::string nextDir;
				char dirName[4096]={0};
				sprintf_s(dirName,4096,"%s\\%s",dir.c_str(),wfdNewFile.cFileName);
				nextDir = std::string(dirName);
				//nextDir.Format("%s\\%s",dir.GetString(),wfdNewFile.cFileName);

				//strMsg.Format("遍历下一级目录:%s",nextDir);
				//m_strMsg=strMsg;
				//UpdateData(FALSE);
				printf("遍历下一级目录:%s\n",nextDir.c_str());

				// 第归遍历子文件夹
				GanerateFiles(nextDir,profile);
			}
			else
			{
				// 文件
				//int newDirLen=m_strNewDir.GetLength();
				//CString newFile;
				//newFile.Format("%s\\%s",dir.GetString(),wfdNewFile.cFileName);
				std::string newFile;
				char theName[1024] = {0};
				sprintf_s(theName,1024,"%s\\%s",dir.c_str(),wfdNewFile.cFileName);

				//strMsg.Format("找到一个文件:%s",newFile);
				//m_strMsg=strMsg;
				//UpdateData(FALSE);
				printf("找到一个文件:%s\n",theName);

				// 替换后缀
				std::string theFile(theName);
				std::basic_string <char>::size_type pos = theFile.find(".fx");
				std::basic_string <char>::size_type pos2 = theFile.find(".fxo");
				if(pos!=std::string::npos && pos2==std::string::npos)
				{
					newFile = theFile.substr(0,pos);
					newFile = newFile + ".fxo";
					char szCmd[4096] = {0};
					sprintf_s(szCmd,4096,"fxc /T %s /Fo %s %s",profile,newFile.c_str(),theFile.c_str());
					printf("执行:%s",szCmd);
					::system(szCmd);
				}
			}
		}
		
		// 查找下一个文件
		if(::FindNextFileA(hNewFile,&wfdNewFile)==0)
		{
			if(GetLastError()==ERROR_NO_MORE_FILES)
			{
				//strMsg.Format("此目录结束");
				//m_strMsg=strMsg;
				//UpdateData(FALSE);
				printf("此目录结束\n");
			}
			else
			{
				//MessageBox("查找文件失败");
			}
			break;
		}
	}while(true);
	// 关闭查找
	FindClose(hNewFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	GanerateFiles(std::string(argv[1]),argv[2]);
	while(getchar()!='q'){};
	return 0;
}

