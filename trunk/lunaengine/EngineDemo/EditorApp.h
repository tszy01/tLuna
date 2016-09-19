#ifndef _EDITORAPP_H_
#define _EDITORAPP_H_

#include "ConfigDef.h"

#ifdef BUILD_EDITOR

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/app.h"
#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/msgdlg.h"
#include "wx/log.h"
#include "wx/textctrl.h"
#include "wx/textdlg.h"
#endif

/*class GB_WxCmdWnd;
class GB_WxAddObjWnd;
class GB_WxPropertyWnd;
class GB_WxNewTerrainWnd;
class GB_WxEditTerrainWnd;
class GB_WxEditFogWnd;
class GB_WxEditAmbientWnd;
class GB_WxEditSkyWnd;
class GB_WxEditMeshBrushWnd;
class GB_WxSetToneMappingWnd;
class GB_WxEditNpcTemplateWnd;
class GB_WxEditResTemplateWnd;
class GB_WxNavGenWnd;
class GB_WxCameraCtrlWnd;*/
//! wxApp
class EditorApp: public wxApp
{
public:
	//! constructor
	EditorApp();
	//! destructor
	virtual ~EditorApp();
public:
	//! init app
	//! \return true for success
	bool OnInit();
	//! app loop
	//! \return true for continue
	virtual bool ProcessAppLoop();
	// app退出
	// 注意这时主窗口已经被销毁了
	// 编辑器引擎部分应该放在这之前销毁
	//! exit
	virtual void ProcessAppExit();
	//! game loop
	//! \return true for success
	virtual bool ProcessGameLoop();
	//! call to destroy editor
	void ProcessEditorDestroy();
private:
	//! main frame
	wxFrame* mMainFrame;
	////! cmd wnd
	//GB_WxCmdWnd* mCmdWnd;
	////! add obj wnd
	//GB_WxAddObjWnd* mAddObjWnd;
	////! property wnd
	//GB_WxPropertyWnd* mPropWnd;
	////! new terrain wnd
	//GB_WxNewTerrainWnd* mNewTerrainWnd;
	////! edit terrain wnd
	//GB_WxEditTerrainWnd* mEditTerrainWnd;
	////! edit fog wnd
	//GB_WxEditFogWnd* mEditFogWnd;
	////! edit sky wnd
	//GB_WxEditSkyWnd* mEditSkyWnd;
	////! edit ambient wnd
	//GB_WxEditAmbientWnd* mEditAmbientWnd;
	////! edit meshBrush wnd
	//GB_WxEditMeshBrushWnd* mEditMeshBrushWnd;
	////! set toneMapping wnd
	//GB_WxSetToneMappingWnd* mSetToneMappingWnd;
	////! edit npctemplate 
	//GB_WxEditNpcTemplateWnd* mEditNpcTemplateWnd;
	////! edit restemplate 
	//GB_WxEditResTemplateWnd* mEditResTemplateWnd;
	////! nav mesh gen 
	//GB_WxNavGenWnd* mNavGenWnd;
	////! camera control 
	//GB_WxCameraCtrlWnd* mCameralCtrlWnd;
public:
	////! get cmd wnd ptr
	//inline GB_WxCmdWnd* getCmdWnd()
	//{
	//	return mCmdWnd;
	//}
	////! get property wnd ptr
	//inline GB_WxPropertyWnd* getPropWnd()
	//{
	//	return mPropWnd;
	//}
	//! show object property
	void openObjectProperty();
	//! close object property
	void closeObjectProperty();
	//! open add obj wnd
	void openAddObjWnd();
	//! close add obj wnd
	void closeAddObjWnd();
	//! open new terrain wnd
	void openNewTerrainWnd();
	//! close new terrain wnd
	void closeNewTerrainWnd();
	//! open edit terrain wnd
	void openEditTerrainWnd();
	//! close edit terrain wnd
	void closeEditTerrainWnd();
	//! call edit terrain wnd refresh
	void refreshEditTerrainWnd();
	//! open edit fog wnd
	void openEditFogWnd();
	//! close edit fog wnd
	void closeEditFogWnd();
	//! open edit sky wnd
	void openEditSkyWnd();
	//! close edit sky wnd
	void closeEditSkyWnd();
	//! open edit ambient wnd
	void openEditAmbientWnd();
	//! close edit ambient wnd
	void closeEditAmbientWnd();
	//! open edit meshBrush wnd
	void openEditMeshBrushWnd();
	//! close edit meshBrush wnd
	void closeEditMeshBrushWnd();
	//! open SetToneMapping wnd
	void openSetToneMappingWnd();
	//! close SetToneMapping wnd
	void closeSetToneMappingWnd();
	//! open NavGen wnd
	void openNavGenWnd();
	//! close NavGen wnd
	void closeNavGenWnd();
	//! open CameralCtrl wnd
	void openCameralCtrlWnd();
	//! hide  CameralCtrl wnd
	void hideCameralCtrlWnd();
	//! show CameralCtrl wnd
	void showCameralCtrlWnd();
	//! close Cameral wnd
	void closeCameralCtrlWnd();
	//! show main frame shortcut menu
	void showMainFrameShortcutMenu();
};

#endif // BUILD_EDITOR

#endif