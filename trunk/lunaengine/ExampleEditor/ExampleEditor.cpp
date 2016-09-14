// ExampleEditor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ExampleEditor.h"
#include "EditorFunc.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	int re = TLunaEngine::LaunchEditor(hInstance,1280,800,"../../demores/");
	return re;
}
