#ifndef _TLEDITORFUNC_H_
#define _TLEDITORFUNC_H_

#include <Windows.h>

namespace TLunaEngine{

	// �����༭��
	// hInst:Ӧ�ó���ʵ�����
	// lWidth:�󻺳�����
	// lHeight:�󻺳�����
	// szResDir:��Դ��Ŀ¼
	int LaunchEditor(HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir);

}

#endif