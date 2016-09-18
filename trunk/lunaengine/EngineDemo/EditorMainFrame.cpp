#include "EditorMainFrame.h"
#include "EditorApp.h"
//#include "gb_seappmgr.h"

BEGIN_EVENT_TABLE(EditorMainFrame, wxFrame)
	/*EVT_MENU(TLunaEngine::GB_MENU_NEW_FILE,TLunaEngine::EditorMainFrame::onNewFile)
	EVT_MENU(TLunaEngine::GB_MENU_OPEN_FILE,TLunaEngine::EditorMainFrame::onOpenFile)
	EVT_MENU(TLunaEngine::GB_MENU_SAVE_FILE,TLunaEngine::EditorMainFrame::onSaveFile)
	EVT_MENU(TLunaEngine::GB_MENU_SAVE_FILE_AS,TLunaEngine::EditorMainFrame::onSaveFileAs)
	EVT_MENU(TLunaEngine::GB_MENU_WIREFRAME,TLunaEngine::EditorMainFrame::onWireFrame)
	EVT_MENU(TLunaEngine::GB_MENU_POINTS,TLunaEngine::EditorMainFrame::onPoints)
	EVT_MENU(TLunaEngine::GB_MENU_SOLID,TLunaEngine::EditorMainFrame::onSolid)
	EVT_MENU(TLunaEngine::GB_MENU_NEW_TERRAIN,TLunaEngine::EditorMainFrame::onNewTerrain)
	EVT_MENU(TLunaEngine::GB_MENU_EDIT_TERRAIN,TLunaEngine::EditorMainFrame::onEditTerrain)
	EVT_MENU(TLunaEngine::GB_MENU_EDIT_SKY,TLunaEngine::EditorMainFrame::onEditSky)
	EVT_MENU(TLunaEngine::GB_MENU_EDIT_FOG,TLunaEngine::EditorMainFrame::onEditFog)
	EVT_MENU(TLunaEngine::GB_MENU_EDIT_AMBIENT,TLunaEngine::EditorMainFrame::onEditAmbient)
	EVT_MENU(TLunaEngine::GB_MENU_EDIT_MESHBRUSH,TLunaEngine::EditorMainFrame::onEditMeshBrush)
	EVT_MENU(TLunaEngine::GB_MENU_SET_TONEMAPPING,TLunaEngine::EditorMainFrame::onSetToneMapping)
	EVT_MENU(TLunaEngine::GB_MENU_SC_HIDE_ME,TLunaEngine::EditorMainFrame::onScHideMe)
	EVT_MENU(TLunaEngine::GB_MENU_SC_HIDE_OTHER,TLunaEngine::EditorMainFrame::onScHideOther)
	EVT_MENU(TLunaEngine::GB_MENU_SC_SHOW_ALL,TLunaEngine::EditorMainFrame::onScShowAll)
	EVT_MENU(TLunaEngine::GB_MENU_SC_HIDE_ALL,TLunaEngine::EditorMainFrame::onScHideAll)
	EVT_MENU(TLunaEngine::GB_MENU_SC_HIDE_TYPE,TLunaEngine::EditorMainFrame::onScHideType)
	EVT_MENU(TLunaEngine::GB_MENU_SC_HIDE_OTHER_TYPE,TLunaEngine::EditorMainFrame::onScHideOtherType)
	EVT_MENU(TLunaEngine::GB_MENU_SC_SHOW_TYPE,TLunaEngine::EditorMainFrame::onScShowType)
	EVT_MENU(TLunaEngine::GB_MENU_SC_EDIT_PROPERTY,TLunaEngine::EditorMainFrame::onScEditProperty)
	EVT_MENU(TLunaEngine::GB_MENU_SC_PLAY_HERE,TLunaEngine::EditorMainFrame::onScPlayHere)
	EVT_MENU(TLunaEngine::GB_MENU_SC_ADD_OBJ,TLunaEngine::EditorMainFrame::onScAddObj)
	EVT_MENU(TLunaEngine::GB_MENU_SC_DEL_ME,TLunaEngine::EditorMainFrame::onScDelMe)
	EVT_MENU(TLunaEngine::GB_MENU_SC_MEM_ME,TLunaEngine::EditorMainFrame::onScMemMe)
	EVT_MENU(TLunaEngine::GB_MENU_OPEN_EMAP,TLunaEngine::EditorMainFrame::onOpenEMap)
	EVT_MENU(TLunaEngine::GB_MENU_SAVE_EMAP,TLunaEngine::EditorMainFrame::onSaveEMap)
	EVT_MENU(TLunaEngine::GB_MENU_COPY_EMAP_OBJ,TLunaEngine::EditorMainFrame::onCopyEMapObjToGMap)
	EVT_MENU(TLunaEngine::GB_MENU_ADJUST_TRANSLATE,TLunaEngine::EditorMainFrame::onScAdjustTranslate)
	EVT_MENU(TLunaEngine::GB_MENU_ADJUST_ROTATE,TLunaEngine::EditorMainFrame::onScAdjustRotate)
	EVT_MENU(TLunaEngine::GB_MENU_NAVGEN, TLunaEngine::EditorMainFrame::onScNavGen)
	EVT_MENU(TLunaEngine::GB_MENU_RELOAD_NPC_TMPL, TLunaEngine::EditorMainFrame::onReloadNpcTemplate)*/

	//EVT_CONTEXT_MENU(TLunaEngine::EditorMainFrame::onContextMenu)
END_EVENT_TABLE()

EditorMainFrame::EditorMainFrame(void) : wxFrame((wxFrame *)NULL, wxID_ANY, _T("Editor"))
{
	//// menu
	//wxMenu *fileMenu = new wxMenu;
	//fileMenu->Append(GB_MENU_NEW_FILE,wxT("New Map"));
	//fileMenu->Append(GB_MENU_OPEN_FILE,wxT("Open Map"));
	//fileMenu->Append(GB_MENU_SAVE_FILE,wxT("Save Map"));
	//fileMenu->Append(GB_MENU_SAVE_FILE_AS,wxT("Save Map As"));
	//fileMenu->Append(GB_MENU_OPEN_EMAP,wxT("Open EMap"));
	//fileMenu->Append(GB_MENU_SAVE_EMAP,wxT("Save EMap"));
	//fileMenu->Append(GB_MENU_COPY_EMAP_OBJ,wxT("Copy EMap Obj"));
	//wxMenu* ctrlMenu = new wxMenu;
	//wxMenu* subDisplay = new wxMenu;
	//subDisplay->AppendRadioItem(GB_MENU_WIREFRAME,wxT("Wireframe"));
	//subDisplay->AppendRadioItem(GB_MENU_POINTS,wxT("Points"));
	//wxMenuItem* pSolidMenu = subDisplay->AppendRadioItem(GB_MENU_SOLID,wxT("Solid"));
	//pSolidMenu->Check(true);
	//ctrlMenu->AppendSubMenu(subDisplay,wxT("Polygon Mode"));
	//wxMenu* subTerrain = new wxMenu;
	//subTerrain->Append(GB_MENU_NEW_TERRAIN,wxT("New Terrain"));
	//subTerrain->Append(GB_MENU_EDIT_TERRAIN,wxT("Edit Terrain"));
	//ctrlMenu->AppendSubMenu(subTerrain,wxT("Terrain"));
	//ctrlMenu->Append(GB_MENU_EDIT_SKY,wxT("Edit Sky"));
	//ctrlMenu->Append(GB_MENU_EDIT_FOG,wxT("Edit Fog"));
	//ctrlMenu->Append(GB_MENU_EDIT_AMBIENT,wxT("Edit Ambient"));
	//ctrlMenu->Append(GB_MENU_EDIT_MESHBRUSH, wxT("Edit MeshBrush"));
	//ctrlMenu->Append(GB_MENU_SET_TONEMAPPING, wxT("Set Tone Mapping"));
	//ctrlMenu->Append(GB_MENU_RELOAD_NPC_TMPL, wxT("Reload NPC Template"));
	//wxMenu* adjustMenu = new wxMenu;
	//adjustMenu->Append(GB_MENU_ADJUST_TRANSLATE, wxT("Translate"));
	//adjustMenu->Append(GB_MENU_ADJUST_ROTATE, wxT("Rotate"));
	//wxMenu* navGenMenu = new wxMenu;
	//navGenMenu->Append(GB_MENU_NAVGEN, wxT("NavGen"));
	//wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );
	//menuBar->Append(fileMenu,wxT("File"));
	//menuBar->Append(ctrlMenu,wxT("Control"));
	//menuBar->Append(adjustMenu, wxT("Adjust"));
	//menuBar->Append(navGenMenu, wxT("NavGen"));
	//SetMenuBar(menuBar);
	//// sc menu
	//mSCMenu = new wxMenu;
	//wxMenu* subSelect = new wxMenu;
	//subSelect->Append(GB_MENU_SC_HIDE_ME,wxT("Hide this"));
	//subSelect->Append(GB_MENU_SC_HIDE_OTHER,wxT("Hide other"));
	//subSelect->Append(GB_MENU_SC_SHOW_ALL,wxT("Show all"));
	//subSelect->Append(GB_MENU_SC_HIDE_ALL,wxT("Hide all"));
	//subSelect->Append(GB_MENU_SC_HIDE_TYPE,wxT("Hide this type"));
	//subSelect->Append(GB_MENU_SC_HIDE_OTHER_TYPE,wxT("Hide other type"));
	//subSelect->Append(GB_MENU_SC_SHOW_TYPE,wxT("Show this type"));
	//mSCMenu->AppendSubMenu(subSelect,wxT("Select"));
	//mSCMenu->Append(GB_MENU_SC_EDIT_PROPERTY,wxT("Edit property"));
	//mSCMenu->Append(GB_MENU_SC_PLAY_HERE,wxT("Play here"));
	//mSCMenu->Append(GB_MENU_SC_ADD_OBJ,wxT("Add object here"));
	//mSCMenu->Append(GB_MENU_SC_DEL_ME,wxT("Delete selected"));
	//mSCMenu->Append(GB_MENU_SC_MEM_ME,wxT("Memory selected"));
		
}

EditorMainFrame::~EditorMainFrame(void)
{
	if (wxTheApp)
	{
		((EditorApp*)wxTheApp)->ProcessEditorDestroy();
	}
	//delete mSCMenu;
}

void EditorMainFrame::showShortcutMenu()
{
	////wxPoint point = event.GetPosition();
	//POINT ptCursor;
	//::GetCursorPos(&ptCursor);
	//::ScreenToClient((HWND)GetHWND(),&ptCursor);
	//wxPoint point;
	//point.x = ptCursor.x;
	//point.y = ptCursor.y;
	//// If from keyboard
	///*if (point.x == -1 && point.y == -1)
	//{
	//	wxSize size = GetSize();
	//	point.x = size.x / 2;
	//	point.y = size.y / 2;
	//}
	//else
	//{
	//	point = ScreenToClient(point);
	//}*/
	//PopupMenu(mSCMenu, point);
}

void EditorMainFrame::onContextMenu(wxContextMenuEvent& event)
{
	//wxPoint point = event.GetPosition();
	//// If from keyboard
	//if (point.x == -1 && point.y == -1)
	//{
	//	wxSize size = GetSize();
	//	point.x = size.x / 2;
	//	point.y = size.y / 2;
	//}
	//else
	//{
	//	point = ScreenToClient(point);
	//}
	//PopupMenu(mSCMenu, point);
}

void EditorMainFrame::onNewFile(wxCommandEvent& event)
{
	//std::vector<Ogre::String> diffs;
	//diffs.push_back("bj1.tga");
	//std::map<GB_Int32,GB_Real32> uvMuls;
	//uvMuls.insert(std::pair<GB_Int32,GB_Real32>(0,1));
	//// 这里构造一个空场景，这个场景将不会后续处理
	//GB_SeAppMgr::getPtr()->newScene();
	//((GB_WxApp*)wxTheApp)->showCameralCtrlWnd();
}

void EditorMainFrame::onOpenFile(wxCommandEvent& event)
{
	/*wxFileDialog dialog(
		this,
		_T("Open Scene"),
		wxEmptyString,
		wxEmptyString,
		_T("Scene files (*.gmap;*.spk)|*.gmap;*.spk"),
		wxFD_OPEN
		);
	if(dialog.ShowModal() == wxID_OK)
	{
		GB_SeAppMgr::getPtr()->loadScene(std::string(dialog.GetFilename().c_str().AsChar()));
		((GB_WxApp*)wxTheApp)->showCameralCtrlWnd();
	}*/
}

void EditorMainFrame::onSaveFile(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->saveScene("");*/
}

void EditorMainFrame::onSaveFileAs(wxCommandEvent& event)
{
	//// 此功能去掉
	//return;
	//if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
	//	return;
	//wxFileDialog dialog(
	//	this,
	//	_T("Open Scene"),
	//	wxEmptyString,
	//	wxEmptyString,
	//	_T("Scene files (*.gmap)|*.gmap"),
	//	wxFD_SAVE|wxFD_OVERWRITE_PROMPT
	//	);
	//if(dialog.ShowModal() == wxID_OK)
	//{
	//	GB_SeAppMgr::getPtr()->saveScene(std::string(dialog.GetFilename().c_str().AsChar()));
	//}
}

void EditorMainFrame::onOpenEMap(wxCommandEvent& event)
{
	/*wxFileDialog dialog(
		this,
		_T("Open Scene"),
		wxEmptyString,
		wxEmptyString,
		_T("EMap files (*.emap)|*.emap"),
		wxFD_OPEN
		);
	if(dialog.ShowModal() == wxID_OK)
	{
		GB_SeAppMgr::getPtr()->loadEMap(std::string(dialog.GetFilename().c_str().AsChar()));
		((GB_WxApp*)wxTheApp)->showCameralCtrlWnd();
	}*/
}

void EditorMainFrame::onSaveEMap(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->saveEMap("");*/
}

void EditorMainFrame::onCopyEMapObjToGMap(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	if(GB_SeAppMgr::getPtr()->copyObjToGMap())
	{
		wxMessageBox("拷贝成功");
	}
	else
	{
		wxMessageBox("拷贝失败");
	}*/
}

void EditorMainFrame::onWireFrame(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->setPolygonMode(Ogre::PM_WIREFRAME);*/
}

void EditorMainFrame::onPoints(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->setPolygonMode(Ogre::PM_POINTS);*/
}

void EditorMainFrame::onSolid(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->setPolygonMode(Ogre::PM_SOLID);*/
}

void EditorMainFrame::onNewTerrain(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->openNewTerrainWnd();*/
}

void EditorMainFrame::onEditTerrain(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->openEditTerrainWnd();*/
}

void EditorMainFrame::onEditSky(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->openEditSkyWnd();*/
}

void EditorMainFrame::onEditFog(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->openEditFogWnd();*/
}

void EditorMainFrame::onEditAmbient(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->openEditAmbientWnd();*/
}

void EditorMainFrame::onEditMeshBrush(wxCommandEvent& event)
{
	/*GB_SeAppMgr* ptr = GB_SeAppMgr::getPtr();
	if(ptr == NULL)
		return ;
	if(ptr->isDataAvailable() == false)
		return ;
	((GB_WxApp*)wxTheApp)->openEditMeshBrushWnd();*/
}
void EditorMainFrame::onSetToneMapping(wxCommandEvent& event)
{
	/*GB_SeAppMgr* ptr = GB_SeAppMgr::getPtr();
	if(ptr == NULL)
		return ;
	if(ptr->isDataAvailable() == false)
		return ;
	((GB_WxApp*)wxTheApp)->openSetToneMappingWnd();*/
}

void EditorMainFrame::onScHideMe(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->hideSelectedSceneObj();*/
}

void EditorMainFrame::onScHideOther(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->hideUnselectedSceneObjects();*/
}

void EditorMainFrame::onScShowAll(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->showAllObjects(true);*/
}

void EditorMainFrame::onScHideAll(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->showAllObjects(false);*/
}

void EditorMainFrame::onScHideType(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->showSelectedObjType(false);*/
}

void EditorMainFrame::onScHideOtherType(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->hideUnselectedObjType();*/
}

void EditorMainFrame::onScShowType(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->showSelectedObjType(true);*/
}

void EditorMainFrame::onScEditProperty(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->openObjectProperty();*/
}

void EditorMainFrame::onScAddObj(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->openAddObjWnd();*/
}

void EditorMainFrame::onScDelMe(wxCommandEvent &event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	((GB_WxApp*)wxTheApp)->closeObjectProperty();
	GB_SeAppMgr::getPtr()->deleteSelectedSceneObj();*/
}

void EditorMainFrame::onScMemMe(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->memSelObject();*/
}

void EditorMainFrame::onScPlayHere(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;*/
}

void EditorMainFrame::onScAdjustTranslate(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->setAjustMode(GB_SeAppMgr::AT_TRANSLATE);*/
}

void EditorMainFrame::onScAdjustRotate(wxCommandEvent& event)
{
	/*if(GB_SeAppMgr::getPtr()->isDataAvailable()==false)
		return;
	GB_SeAppMgr::getPtr()->setAjustMode(GB_SeAppMgr::AT_ROTATE);*/
}

void EditorMainFrame::onScNavGen(wxCommandEvent& event)
{
	/*GB_SeAppMgr* ptr = GB_SeAppMgr::getPtr();
	if (ptr == NULL)
		return ;
	if (ptr->isDataAvailable() == false)
		return ;
	((GB_WxApp*)wxTheApp)->openNavGenWnd();*/
}

void EditorMainFrame::onReloadNpcTemplate(wxCommandEvent& event)
{
	/*GB_SeAppMgr* ptr = GB_SeAppMgr::getPtr();
	if (ptr == NULL)
		return ;
	if (ptr->isDataAvailable() == false)
		return ;
	ptr->reloadNpcTemplate();*/
}