// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here
#include <windows.h>
#include <string>
#include <limits>
#include <vector>
#include <stdio.h>        //-- c 语言的头文件
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <strsafe.h>


// TODO: reference additional headers your program requires here

#define SAFE_DELETE_ARRAY(p)    {if(p) {delete[](p);(p)=NULL;}}
#define SAFE_DELETE(p)       { if(p) {delete (p); (p)=NULL;} }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; /*delete (p);*/ } }