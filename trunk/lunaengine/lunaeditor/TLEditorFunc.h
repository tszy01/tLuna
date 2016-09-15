#ifndef _TLEDITORFUNC_H_
#define _TLEDITORFUNC_H_

#include <Windows.h>

namespace TLunaEngine{

	// 启动编辑器
	// hInst:应用程序实例句柄
	// lWidth:后缓冲区宽
	// lHeight:后缓冲区高
	// szResDir:资源根目录
	int LaunchEditor(HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir);

}

#endif