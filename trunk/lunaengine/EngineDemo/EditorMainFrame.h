#ifndef _EDITORMAINFRAME_H_
#define _EDITORMAINFRAME_H_

#include "ConfigDef.h"

#ifdef BUILD_EDITOR
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/app.h"
#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/msgdlg.h"
#include "wx/log.h"
#include "wx/textctrl.h"
#include "wx/textdlg.h"
#endif

//! menu id enum
/*enum GB_MENU_ID
{
	GB_MENU_NEW_FILE			= 100,
	GB_MENU_OPEN_FILE			= 101,
	GB_MENU_SAVE_FILE			= 102,
	GB_MENU_SAVE_FILE_AS		= 103,
	GB_MENU_SUB_DISPLAY			= 104,
	GB_MENU_WIREFRAME			= 105,
	GB_MENU_POINTS				= 106,
	GB_MENU_SOLID				= 107,
	GB_MENU_SUB_TERRAIN			= 108,
	GB_MENU_NEW_TERRAIN			= 109,
	GB_MENU_EDIT_TERRAIN		= 110,
	GB_MENU_EDIT_SKY			= 111,
	GB_MENU_EDIT_FOG			= 112,
	GB_MENU_EDIT_AMBIENT		= 113,
	GB_MENU_ADJUST_TRANSLATE	= 114,
	GB_MENU_ADJUST_ROTATE		= 115,
	GB_MENU_EDIT_MESHBRUSH      = 116,
	GB_MENU_NAVGEN              = 119,
	GB_MENU_SET_TONEMAPPING     = 120,
	GB_MENU_OPEN_EMAP			= 121,
	GB_MENU_SAVE_EMAP			= 122,
	GB_MENU_COPY_EMAP_OBJ		= 123,
	GB_MENU_RELOAD_NPC_TMPL		= 124,

	GB_MENU_SC_SUB_SHOW			= 200,
	GB_MENU_SC_HIDE_ME			= 201,
	GB_MENU_SC_HIDE_OTHER		= 202,
	GB_MENU_SC_SHOW_ALL			= 203,
	GB_MENU_SC_HIDE_ALL			= 204,
	GB_MENU_SC_HIDE_TYPE		= 205,
	GB_MENU_SC_HIDE_OTHER_TYPE	= 206,
	GB_MENU_SC_SHOW_TYPE		= 207,
	GB_MENU_SC_EDIT_PROPERTY	= 208,
	GB_MENU_SC_PLAY_HERE		= 209,
	GB_MENU_SC_ADD_OBJ			= 210,
	GB_MENU_SC_DEL_ME			= 211,
	GB_MENU_SC_MEM_ME			= 212,
};*/

//! main frame for editor
class EditorMainFrame : public wxFrame
{
public:
	//! constructor
	EditorMainFrame(void);
	//! destructor
	virtual ~EditorMainFrame(void);
private:
	// ---------------- event functions -----------------
	//! on new file
	void onNewFile(wxCommandEvent& event);
	//! on open file
	void onOpenFile(wxCommandEvent& event);
	//! on save file
	void onSaveFile(wxCommandEvent& event);
	//! on save file as
	void onSaveFileAs(wxCommandEvent& event);
	//! on open emap
	void onOpenEMap(wxCommandEvent& event);
	//! on save emap
	void onSaveEMap(wxCommandEvent& event);
	//! copy emap obj to gmap
	void onCopyEMapObjToGMap(wxCommandEvent& event);
	//! on wireframe
	void onWireFrame(wxCommandEvent& event);
	//! on points
	void onPoints(wxCommandEvent& event);
	//! on solid
	void onSolid(wxCommandEvent& event);
	//! on new terrain
	void onNewTerrain(wxCommandEvent& event);
	//! on edit terrain
	void onEditTerrain(wxCommandEvent& event);
	//! on edit sky
	void onEditSky(wxCommandEvent& event);
	//! on edit fog
	void onEditFog(wxCommandEvent& event);
	//! on edit ambient
	void onEditAmbient(wxCommandEvent& event);
	//! on edit meshbrush
	void onEditMeshBrush(wxCommandEvent& event);
	//! on set Tone Mapping
	void onSetToneMapping(wxCommandEvent& event);
	//! on hide me
	void onScHideMe(wxCommandEvent& event);
	//! on hide other
	void onScHideOther(wxCommandEvent& event);
	//! on show all
	void onScShowAll(wxCommandEvent& event);
	//! on hide all
	void onScHideAll(wxCommandEvent& event);
	//! on hide this type
	void onScHideType(wxCommandEvent& event);
	//! on hide other type
	void onScHideOtherType(wxCommandEvent& event);
	//! on show this type
	void onScShowType(wxCommandEvent& event);
	//! on Edit Property
	void onScEditProperty(wxCommandEvent& event);
	//! on Add Obj
	void onScAddObj(wxCommandEvent& event);
	//! on play here
	void onScPlayHere(wxCommandEvent& event);
	//! on del me
	void onScDelMe(wxCommandEvent& event);
	//! on mem me
	void onScMemMe(wxCommandEvent& event);
	//! on adjust translate
	void onScAdjustTranslate(wxCommandEvent& event);
	//! on adjust rotate
	void onScAdjustRotate(wxCommandEvent& event);
	//! on NavMesh Gen 
	void onScNavGen(wxCommandEvent& event);
	//! on Reload Npc Template
	void onReloadNpcTemplate(wxCommandEvent& event);
	//! on context menu
	void onContextMenu(wxContextMenuEvent& event);

private:
	//! shortcut menu
	wxMenu* mSCMenu;
public:
	//! show shortcut menu
	void showShortcutMenu();
private:
	DECLARE_EVENT_TABLE()
};

#endif // BUILD_EDITOR

#endif