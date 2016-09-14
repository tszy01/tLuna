//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "maxProject1.h"

//#include "msxml2.h"
//#include "XMLUtility.h"
#include "decomp.h"

#include "IGame.h"
#include "IGameObject.h"
#include "IGameProperty.h"
#include "IGameControl.h"
#include "IGameModifier.h"
#include "IConversionManager.h"
#include "IGameError.h"
#include "IGameFX.h"

#include "3dsmaxport.h"
#include "CS/phyexp.h"
#include "ISkin.h"

#include <string>
#include <locale.h>

#define maxProject1_CLASS_ID	Class_ID(0x7875b8a4, 0xa7a7461c)
Matrix3 TransposeMatrix(Matrix3 matrix);

std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs"); 
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

// pulled directly from the Sparks site: 
// http://sparks.discreet.com/Knowledgebase/sdkdocs_v8/prog/cs/cs_physique_export.html
// Also available in the SDK docs. Used to find out if this node has a physique modifier or not.
// If it does, it returns a pointer to the modifier, and if not, returns NULL. This can be used to 
// determine whether a node is bone or mesh -- mesh nodes will have Physique modifiers, bone nodes
// will not.
Modifier* FindPhysiqueModifier (INode* nodePtr)
{
	// Get object from node. Abort if no object.
	Object* ObjectPtr = nodePtr->GetObjectRef();

	if (!ObjectPtr) return NULL;

	// Is derived object ?
	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
	{
		// Yes -> Cast.
		IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);
						
		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			// Get current modifier.
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			// Is this Physique ?
			if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				// Yes -> Exit.
				return ModifierPtr;
			}

			// Next modifier stack entry.
			ModStackIndex++;
		}
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}

	// Not found.
	return NULL;
}

Modifier* FindSkinModifier(INode* pNode)
{
	// get the object reference of the node
	Object *pObject;
	pObject = pNode->GetObjectRef();
	if(pObject == 0) return 0;

	// loop through all derived objects
	while(pObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject *pDerivedObject;
		pDerivedObject = static_cast<IDerivedObject *>(pObject);

		// loop through all modifiers
		int stackId;
		for(stackId = 0; stackId < pDerivedObject->NumModifiers(); stackId++)
		{
		  // get the modifier
		  Modifier *pModifier;
		  pModifier = pDerivedObject->GetModifier(stackId);

		  // check if we found the skin modifier
		  if(pModifier->ClassID() == SKIN_CLASSID) return pModifier;
		}

		// continue with next derived object
		pObject = pDerivedObject->GetObjRef();
	}

	return NULL;
}


// �õ�Mesh,Victory�÷���
class NullView : public View
{
public:
	NullView()
	{
		worldToView.IdentityMatrix();
		screenW = 640.0f;
		screenH = 480.0f;
	}
	Point2 ViewToScreen(Point3 p)
	{
		return Point2(p.x,p.y);
	}
};

Mesh* FindMesh(INode* pNode,TimeValue time)
{
	Object* pObj = pNode->EvalWorldState(time).obj;
	if(pObj)
	{
		TriObject* tri = (TriObject*)pObj->ConvertToType(time,triObjectClassID);
		if(tri==NULL)
		{
			if(GEOMOBJECT_CLASS_ID == pObj->SuperClassID())
			{
				GeomObject* pGeo = (GeomObject*)pObj;
				NullView view;
				BOOL needDel = FALSE;
				if(pGeo)
				{
					Mesh* mesh = pGeo->GetRenderMesh(time,pNode,view,needDel);
					return mesh;
				}
			}
			return NULL;
		}
		Mesh* mesh = &tri->GetMesh();
		return mesh;
	}
	return NULL;
}

// Add a normal to the list if the smoothing group bits overlap, 
// otherwise create a new vertex normal in the list 
void VNormal::AddNormal(Point3 &n,DWORD s) { 
   if (!(s&smooth) && init) { 
     if (next) next->AddNormal(n,s); 
     else { 
       next = new VNormal(n,s); 
     } 
   } 
   else { 
     norm += n; 
     smooth |= s; 
     init = TRUE; 
   } 
}

// Retrieves a normal if the smoothing groups overlap or there is 
// only one in the list 
Point3& VNormal::GetNormal(DWORD s) 
{ 
   if (smooth&s || !next) return norm; 
   else return next->GetNormal(s);  
}

// Normalize each normal in the list 
void VNormal::Normalize() { 
   VNormal *ptr = next, *prev = this; 
   while (ptr) 
   { 
     if (ptr->smooth&smooth) { 
       norm += ptr->norm; 
       prev->next = ptr->next; 
        delete ptr; 
       ptr = prev->next; 
     } 
     else { 
       prev = ptr; 
       ptr = ptr->next; 
     } 
   } 
   norm = ::Normalize(norm); 
   if (next) next->Normalize(); 
}

// SDK�м��㷨�ߵķ���
void ComputeVertexNormals(Mesh *mesh,VECTOR3* pNors,Matrix3 tm)
{
	Face *face;  
   Point3 *verts; 
   Point3 v0, v1, v2; 
   Tab<VNormal> vnorms; 
   Tab<Point3> fnorms; 
   face = mesh->faces;  
   verts = mesh->verts; 
   vnorms.SetCount(mesh->getNumVerts()); 
   fnorms.SetCount(mesh->getNumFaces()); 
   // Compute face and vertex surface normals 
   for (int i = 0; i < mesh->getNumVerts(); i++) { 
     vnorms[i] = VNormal(); 
   } 
   for (int i = 0; i < mesh->getNumFaces(); i++, face++) { 
     // Calculate the surface normal 
     v0 = verts[face->v[0]]; 
     v1 = verts[face->v[1]]; 
     v2 = verts[face->v[2]]; 
     fnorms[i] = (v1-v0)^(v2-v1); 
     for (int j=0; j<3; j++) {  
       vnorms[face->v[j]].AddNormal(fnorms[i],face->smGroup); 
     } 
     fnorms[i] = Normalize(fnorms[i]); 
   } 

   for (int i=0; i < mesh->getNumVerts(); i++) { 
     vnorms[i].Normalize(); 
   } 
   // Display the normals in the debug window of the VC++ IDE 
   for (int i = 0; i < vnorms.Count(); i++) { 
     //DisplayVertexNormal(vnorms.Addr(i), i, 0); 
	   Point3 normal = Point3(vnorms.Addr(i)->norm.x,vnorms.Addr(i)->norm.y,vnorms.Addr(i)->norm.z);
	   normal = normal * Inverse(TransposeMatrix(tm));
	   normal = normal.Normalize();
	   pNors[i] = VECTOR3(normal.x,normal.y,normal.z);
	   pNors[i].ChangeYZ();
   } 
}

// ���������ķ���,ģ�Ϳռ�,Cal3D�÷���
Point3 GetVertexNormal(Mesh* maxMesh,int faceId, int vertexId)
{
  // get the "rendered" vertex
  RVertex *pRVertex;
  pRVertex = maxMesh->getRVertPtr(vertexId);

  // get the face
  Face *pFace;
  pFace = &maxMesh->faces[faceId];

  // get the smoothing group of the face
  DWORD smGroup;
  smGroup = pFace->smGroup;

  // get the number of normals
  int normalCount;
  normalCount = pRVertex->rFlags & NORCT_MASK;

  // check if the normal is specified ...
  if(pRVertex->rFlags & SPECIFIED_NORMAL)
  {
    return pRVertex->rn.getNormal();
  }
  // ... otherwise, check for a smoothing group
  else if((normalCount > 0) && (smGroup != 0))
  {
    // If there is only one vertex is found in the rn member.
    if(normalCount == 1)
    {
      return pRVertex->rn.getNormal();
    }
    else
    {
      int normalId;
      for(normalId = 0; normalId < normalCount; normalId++)
      {
        if(pRVertex->ern[normalId].getSmGroup() & smGroup)
        {
          return pRVertex->ern[normalId].getNormal();
        }
      }
    }
  }

  // if all fails, return the face normal
  return maxMesh->getFaceNormal(faceId);
}

// ��ת����
Matrix3 TransposeMatrix(Matrix3 matrix)
{
	float transpose[4][3];

	int row;
	for(row = 0; row < 3; row++)
	{
		int column;
		for(column = 0; column < 3; column++)
		{
			transpose[column][row] = matrix.GetAddr()[row][column];
		}
	}

	int column;
	for(column = 0; column < 3; column++)
	{
		transpose[3][column] = 0;
	}

	Matrix3 transposeMatrix(transpose);
	transposeMatrix.NoTrans();

	return transposeMatrix;
}

// Cal3D,�õ��ڵ�TM
Matrix3 GetNodeTMByCal3D(INode *pNode, float time)
{
	// initialize matrix with the identity
	Matrix3 tm, tmParent;
	tm.IdentityMatrix();

	// only do this for valid nodes
	if(pNode != 0)
	{
		bool isMirrored = false;
		// get the node transformation
		tm = pNode->GetNodeTM(SecToTicks(time));

		// This code seems to corrupt some models

		/*if(!pNode->GetINode()->IsRootNode())
		{
		  tmParent = pNode->GetINode()->GetParentTM(SecToTicks(time));
		  
		  
		  // check if the matrix is right handed
		  
		  if(DotProd( CrossProd( tmParent.GetRow(0).Normalize(), 
			tmParent.GetRow(1).Normalize() ).Normalize(), 
			tmParent.GetRow(2).Normalize() ) < -0.1)
		  {
			isMirrored = true;
		  }
		}*/

		// make the transformation uniform
		tm.NoScale();

		AffineParts parts;
		decomp_affine(tm, &parts);
		parts.q.MakeMatrix(tm);

		if(!isMirrored)
		  tm.SetRow(3, parts.t);
		else
		  tm.SetRow(3, -parts.t);
	}

	return tm;
}

// Cal3D,�õ������ռ�������,ȥ������
Matrix3 GetTranslationAndRotationBoneSpace(INode *pNode, float time, Point3& translation, Quat& rotation)
{
	Matrix3 tm;
	// check for invalid nodes
	if(pNode == 0) return tm;

	// the initial node pose (time == -1.0) is at (time == 0.0) for 3ds max
	if(time < 0.0f) time = 0.0f;

	// downcast the node to a max node
	//CMaxNode *pMaxNode;
	//pMaxNode = dynamic_cast<CMaxNode *>(pNode);

	// calculate the inverse transformation
	tm = Inverse(GetNodeTMByCal3D(pNode, time));

	// cpinson here i should make the transform max2gl
	/*if (theExporter.GetAxisGL())
	tm=ConvertMax2Ogl(tm);*/


	// calculate the translation component
	Point3 p;
	p = tm.GetTrans();
	translation = p;

	// calculate the rotation component
	Quat q(tm);
	rotation = q;

	return tm;
}
// Cal3D�õ�face��matid
int GetFaceMaterialId(int faceId,Mesh* maxMesh,std::vector<StdMat*> matlist)
{
	// get the material id of the face
	int materialId;
	materialId = maxMesh->getFaceMtlIndex(faceId);

	// get the material count of this mesh
	int materialCount;
	materialCount = matlist.size();

	// make sure we return a valid material id in all cases:
	// 3ds max assigns predefined material ids to primitives. these material
	// ids can be higher than the actual material count of the mesh itself.
	// the official workaround is to calculate the modulo of the value to the
	// number of existing materials.
	if(materialCount > 0)
	{
		materialId %= materialCount;
	}
	else
	{
		materialId = 0;
	}

	return materialId;
}

// ���������ҵ�matlist�е�mat


// һЩȫ�ֺ���
TCHAR *FixupName (const TCHAR *buf)
{
	static TCHAR buffer[256];
	TCHAR* cPtr;

    _tcscpy(buffer, buf);
    cPtr = buffer;

    while(*cPtr) {
		if (*cPtr == '"') *cPtr = 39;	// Replace double-quote with single quote.
        else if (*cPtr <= 31) *cPtr = _T('_');	// Replace control characters with underscore
        cPtr++;
    }

	return buffer;
}

class maxProject1 : public SceneExport {
	public:
		
		static HWND hParams;
		int				staticFrame;				// �̶�Ϊ0
		int				framePerSample;				// �̶�Ϊ4
		
		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		//Constructor/Destructor
		maxProject1();
		~maxProject1();	

		// ����
		IGameScene * pIgame;		// API�ӿ�
		FILE * m_saveFile;			// �ļ�ָ��
		MatList m_listMat;			// ��������
		MeshList m_listMesh;		// Mesh����
		_SKELETON* m_pSkeletonRoot;	// ��������
		BOOL m_bExportSkeletonMesh;	// �Ƿ�Ϊ��������
		//BOOL m_bExportMesh;			// �Ƿ񵼳�����ģ��
		BOOL m_bExportAnime;		// �Ƿ񵼳�����
		BOOL m_bExportVertexAnime;	// �Ƿ񵼳����㶯��
		BOOL m_bExportNodeAnime;	// �Ƿ񵼳��ڵ㶯��
		std::vector<StdMat*> m_stdMatList;	// �������ı�׼����
		char m_szAnimeName[256];		// AnimeSet������
		// ����
		void ExportSceneInfo();		// ����������Ϣ
		void ExportMaterials();		// ����������Ϣ
		void ExportChildNodeInfo(IGameNode * child,_SKELETON* pRoot);	// �����ڵ���Ϣ
		void CopyMeshInfoToFile();	// �ѵ������ڴ��е�Mesh��Ϣ�浽�ļ���
		void CopyMeshInfoToFileForVA();
		void CopySkeletonInfoToFile(_SKELETON* pRoot);	// �洢������Ϣ
		void CopyAnimeInfoToFile(_SKELETON* pRoot);	// �洢������Ϣ

		// ������������
		void UpdateMapForTex(std::map<int,VERTEXDESC*>* pMap,FACE* pFace,MESH* pMesh);
		void UpdateMapForTexForVA(std::map<int,VERTEXDESC*>* pMap,FACE* pFace,MESH* pMesh,int ikey);
		void UpdateTangentVector(VERTEXDESC* pV0,VERTEXDESC* pV1,VERTEXDESC* pV2);
		void UpdateMapForNormal(std::map<int,VERTEXDESC*>* pMap,FACE* pFace,MESH* pMesh);

		// �ӷ���
		void DumpMaterial(IGameMaterial * mat, int index, int matID = -1);		// ����ÿ������
		void DumpTexture(IGameMaterial * mat, TEXTUREMAP * texMap);				// ��������
		void DumpMatrix(Matrix3 tm,MATRIX* pMatrix);							// ��������
		void DumpMesh(IGameMesh *gm,INode* pNode,MESH* pMesh);					// ����Mesh
		void DumpVertexAnime(IGameMesh* gm,INode* pNode,MESH* pMesh);			// �������㶯��
		void DumpSampleKeys(IGameControl *sc,IGameControlType Type,_SKELETON* pNode);	// �����ؼ�֡����
		void DumpPhysique(Modifier* modifier,MESH* pMesh,INode* pNode);					// ����Physique
		void DumpSkin(Modifier* modifier,MESH* pMesh,INode*pNode);						// ����Skin
		void DumpControl(Control* control,INode* maxNode,_SKELETON* pNode);				// ��������
};



class maxProject1ClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new maxProject1(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return maxProject1_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("maxProject1"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};

static maxProject1ClassDesc maxProject1Desc;
ClassDesc2* GetmaxProject1Desc() { return &maxProject1Desc; }

INT_PTR CALLBACK maxProject1OptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static maxProject1 *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (maxProject1 *)lParam;
			::SetDlgItemTextA(hWnd,IDC_EDIT1,"Anim1");
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;

		case WM_COMMAND:
			{
				if(LOWORD(wParam)==IDC_OK && HIWORD(wParam)==BN_CLICKED)
				{
					imp->m_bExportSkeletonMesh = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK1);
					//imp->m_bExportMesh = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK3);
					imp->m_bExportAnime = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK2);
					imp->m_bExportVertexAnime = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK4);
					imp->m_bExportNodeAnime = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK5);
					::GetDlgItemTextA(hWnd,IDC_EDIT1,imp->m_szAnimeName,255);
					if((strlen(imp->m_szAnimeName)==0) && (imp->m_bExportSkeletonMesh || imp->m_bExportVertexAnime || imp->m_bExportNodeAnime) && imp->m_bExportAnime)
					{
						MessageBoxA(hWnd,"��������Ч����","����",MB_OK);
					}
					else
					{
						EndDialog(hWnd,1);
					}
				}
				else if(LOWORD(wParam)==IDC_CHECK1)
				{
					BOOL bCheck = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK1);
					if(bCheck)
					{
						CheckDlgButton(hWnd,IDC_CHECK4,!(BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK1));
						CheckDlgButton(hWnd,IDC_CHECK5,!(BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK1));
					}
					else
					{
						CheckDlgButton(hWnd,IDC_CHECK2,FALSE);
					}
					EnableWindow(GetDlgItem(hWnd, IDC_CHECK2), (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK1));
				}
				else if(LOWORD(wParam)==IDC_CHECK4)
				{
					BOOL bCheck = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK4);
					if(bCheck)
					{
						CheckDlgButton(hWnd,IDC_CHECK1,!(BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK4));
						CheckDlgButton(hWnd,IDC_CHECK5,!(BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK4));
					}
					else
					{
						CheckDlgButton(hWnd,IDC_CHECK2,FALSE);
					}
					EnableWindow(GetDlgItem(hWnd, IDC_CHECK2), (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK4));
				}
				else if(LOWORD(wParam)==IDC_CHECK5)
				{
					BOOL bCheck = (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK5);
					if(bCheck)
					{
						CheckDlgButton(hWnd,IDC_CHECK4,!(BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK5));
						CheckDlgButton(hWnd,IDC_CHECK1,!(BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK5));
					}
					else
					{
						CheckDlgButton(hWnd,IDC_CHECK2,FALSE);
					}
					EnableWindow(GetDlgItem(hWnd, IDC_CHECK2), (BOOL)IsDlgButtonChecked(hWnd, IDC_CHECK5));
				}
			}
			return 1;
	}
	return 0;
}


//--- maxProject1 -------------------------------------------------------
maxProject1::maxProject1()
{
	staticFrame = 0;
	framePerSample = 1;
	m_bExportSkeletonMesh = FALSE;
	m_bExportAnime = FALSE;
	m_bExportNodeAnime = FALSE;
	m_bExportVertexAnime = FALSE;
	memset(m_szAnimeName,0,256);
}

maxProject1::~maxProject1() 
{
	
}

int maxProject1::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *maxProject1::Ext(int n)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("tmesh");
}

const TCHAR *maxProject1::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("Tmesh IGame Export");
}
	
const TCHAR *maxProject1::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("Tmesh Exporter");
}

const TCHAR *maxProject1::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("Tang Ziyin");
}

const TCHAR *maxProject1::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("Copyright (c) 1999-2008 Tangsoft All Rights Reserved");
}

const TCHAR *maxProject1::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *maxProject1::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int maxProject1::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void maxProject1::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL maxProject1::SupportsOptions(int ext, DWORD options)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}


void maxProject1::ExportSceneInfo()
{
	TSTR buf;
	struct tm *newtime;
	time_t aclock;

	// д�ļ�
	char sz[1024]={0};

	time( &aclock );
	newtime = localtime(&aclock);

	TSTR today = _tasctime(newtime);	// The date string has a \n appended.
	today.remove(today.length()-1);		// Remove the \n

	//// д�ļ�
	//// StartFrame
	//sprintf(sz,"%d\n",pIgame->GetSceneStartTime() / pIgame->GetSceneTicks());
	//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

	//// д�ļ�
	//// EndFrame
	//sprintf(sz,"%d\n",pIgame->GetSceneEndTime() / pIgame->GetSceneTicks());
	//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

	//// д�ļ�
	//// FrameRate
	//sprintf(sz,"%d\n",GetFrameRate());
	//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

	//// д�ļ�
	//// TicksPerFrame
	//sprintf(sz,"%d\n",pIgame->GetSceneTicks());
	//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

	int count = 0;
	count = (pIgame->GetSceneEndTime() / pIgame->GetSceneTicks()) -
		(pIgame->GetSceneStartTime() / pIgame->GetSceneTicks()) + 1;

	// д����֡��
	sprintf(sz,"%d\n",count);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

	for(int i=0;i<count;i++)
	{
		float time = (float)i/(float)GetFrameRate();
		// ÿ֡ʱ��
		sprintf(sz,"%f\n",time);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	}

	return ;
}

void maxProject1::ExportMaterials()
{
	TSTR buf;
	int matCount = pIgame->GetRootMaterialCount();

	m_listMat.clear();

	// д�ļ�
	char sz[1024]={0};

	for(int j =0;j<matCount;j++)
	{
		DumpMaterial(pIgame->GetRootMaterial(j),j);
	}

	// ȫ�����������ڴ�֮��,ͳһд���ļ�
	// ����������
	sprintf(sz,"%d\n",m_listMat.size());
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	MatList::iterator itr = m_listMat.begin();
	for(;itr!=m_listMat.end();++itr)
	{
		MATERIAL *p = *itr;
		// Name
		sprintf(sz,"%s\n",p->_szName);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// Diffuse
		sprintf(sz,"%f %f %f\n",p->_Diffuse[0],p->_Diffuse[1],p->_Diffuse[2]);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// Ambient
		sprintf(sz,"%f %f %f\n",p->_Ambient[0],p->_Ambient[1],p->_Ambient[2]);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// Specular
		sprintf(sz,"%f %f %f\n",p->_Specular[0],p->_Specular[1],p->_Specular[2]);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// ��������
		sprintf(sz,"%d\n",p->_TexMap._Count);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// ѭ������
		for(int i=0;i<p->_TexMap._Count;i++)
		{
			// Bitmap�ļ���
			sprintf(sz,"%s\n",p->_TexMap._pTexs[i]._szFile);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		}
		// �ͷŵ��ڴ�
		delete p;
		p = NULL;
	}
	m_listMat.clear();
}

void maxProject1::DumpMaterial(IGameMaterial *mat,int index,int matID)
{
	TSTR buf;
	IGameProperty *prop;

	// д�ļ�
	char sz[1024]={0};

	// ********************
	// ֻ����Standard���͵�
	if(wcscmp(mat->GetMaterialClass(),L"Standard")==0)
	//if(!mat->IsMultiType())
	{
		StdMat* stdMat = (StdMat*)mat->GetMaxMaterial();
		m_stdMatList.push_back(stdMat);

		MATERIAL* matStruct = new MATERIAL();
		sprintf(matStruct->_szName,"%s",mat->GetMaterialName());
		
		Point3 pt;
		prop = mat->GetDiffuseData();
		prop->GetPropertyValue(pt);
		matStruct->_Diffuse[0] = pt.x;
		matStruct->_Diffuse[1] = pt.y;
		matStruct->_Diffuse[2] = pt.z;

		prop = mat->GetAmbientData();
		prop->GetPropertyValue(pt);
		matStruct->_Ambient[0] = pt.x;
		matStruct->_Ambient[1] = pt.y;
		matStruct->_Ambient[2] = pt.z;

		prop = mat->GetSpecularData();
		prop->GetPropertyValue(pt);
		matStruct->_Specular[0] = pt.x;
		matStruct->_Specular[1] = pt.y;
		matStruct->_Specular[2] = pt.z;

		DumpTexture(mat,&matStruct->_TexMap);

		m_listMat.push_back(matStruct);
	}
	// **********************

	if(mat->IsMultiType())
	{
		for(int k=0;k<mat->GetSubMaterialCount();k++)
		{
			int mID = mat->GetMaterialID(k);
			IGameMaterial * subMat = mat->GetSubMaterial(k);
			DumpMaterial(subMat,k, mID);
		}
	}
}

void maxProject1::DumpTexture(IGameMaterial * mat, TEXTUREMAP * texMap)
{
	//int texCount = mat->GetNumberOfTextureMaps();

	//// 0Ҳ����
	////if(texCount==0) return;

	//TSTR buf;
	//texMap->_Count = texCount;
	//if(texCount>0)
	//{
	//	texMap->_pTexs = new TEXTURE[texCount];
	//}
	//else
	//{
	//	return;
	//}
	//
	//for(int i=0;i<texCount;i++)
	//{
	//	IGameTextureMap * tex = mat->GetIGameTextureMap(i);
	//	int mapSlot = tex->GetStdMapSlot();
	//	//TODO: Add attributes 'Source', 'UAddress', 'VAddress', 'WAddress', 'MapChannel'
	//	// ����Ҫ������������BitmapTexture
	//	if(tex->IsEntitySupported())	//its a bitmap texture
	//	{
	//		// TODO: Add elements MapStrength, MapOffset, ...
	//		TSTR str = tex->GetBitmapFileName();
	//		sprintf(texMap->_pTexs[i]._szFile,"%s",str.data());
	//	}
	//}

	//texMap->_Count = 1;
	//texMap->_pTexs = new TEXTURE[1];

	// ʹ��cal3d�ķ���ֻ����1��ͨ��������
	StdMat* maxMat = (StdMat*)mat->GetMaxMaterial();

	// loop through all maps of the material
	int materialMapId;
	for(materialMapId = 0; materialMapId < maxMat->NumSubTexmaps(); materialMapId++)
	{
		// get the texture map
		Texmap *pTexMap;
		pTexMap = maxMat->GetSubTexmap(materialMapId);

		// check if the map is valid
		if((pTexMap != 0) && (maxMat->MapEnabled(materialMapId)) && (materialMapId==1))
		{
			if(pTexMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
			{
				texMap->_Count = 1;
				texMap->_pTexs = new TEXTURE[1];
				// get the full filepath
				std::string strFilename;
				std::wstring wstrFilename;
				wstrFilename = ((BitmapTex *)pTexMap)->GetMapName();
				strFilename = ws2s(wstrFilename);

				// extract the filename
				std::string::size_type pos;
				pos = strFilename.find_last_of("\\/:");
				//if(pos == std::string::npos) return strFilename;

				//return strFilename.substr(pos + 1);

				if(pos==std::string::npos)
				{
					sprintf(texMap->_pTexs[0]._szFile,"%s",strFilename.data());
				}
				else
				{
					sprintf(texMap->_pTexs[0]._szFile,"%s",strFilename.substr(pos + 1).data());
				}

				// ���ﱣ��uv������Ϣ
				int keyCount = 0;
				keyCount = (pIgame->GetSceneEndTime() / pIgame->GetSceneTicks()) -
					(pIgame->GetSceneStartTime() / pIgame->GetSceneTicks()) + 1;
				texMap->_pTexs[0]._keyCount = keyCount;
				StdUVGen* pStdUVGen = (StdUVGen*)pTexMap->GetTheUVGen();
				if(keyCount>0 && pStdUVGen)
				{
					texMap->_pTexs[0]._UVOff = new VECTOR3[keyCount];
					for(int iKey=0;iKey<keyCount;iKey++)
					{
						TimeValue time = iKey * pIgame->GetSceneTicks();
						// ����ʼ�ձ����0���㣬��Ϊÿ���㶼��һ����
						texMap->_pTexs[0]._UVOff[iKey]._X = pStdUVGen->GetUOffs(time);
						texMap->_pTexs[0]._UVOff[iKey]._Y = pStdUVGen->GetVOffs(time);
					}
				}
			}
			break;
		}
	}
}

void maxProject1::ExportChildNodeInfo(IGameNode *child,_SKELETON* pRoot)
{
	IGameKeyTab poskeys;
	IGameKeyTab rotkeys;
	IGameKeyTab scalekeys;
	TSTR buf,data;

	// д�ļ�
	char sz[1024]={0};
	INode* maxNode = child->GetMaxNode();

	// ��Զ����GroupOwner
	if(!child->IsGroupOwner())
	{
		ULONG  handle = child->GetMaxNode()->GetHandle();
		IGameObject * obj = child->GetIGameObject();
		IGameObject::MaxType T = obj->GetMaxType();

		// д�ļ�
		// ObjectType
		// 6:����
		// 2:Mesh
		// �������಻����
		int objType = obj->GetIGameType();

		switch(objType)
		{
			// ����
			case IGameObject::IGAME_BONE:
			{
				// �Ȱѽڵ������
				_SKELETON* pNew = new _SKELETON();
				pNew->_pChildList = new std::list<_SKELETON*>();
				pRoot->_pChildList->push_back(pNew);
				pNew->_pParent = pRoot;
				// д�ļ�
				// NodeName
				sprintf(pNew->_szName,"%s",FixupName (child->GetName()));
				// д�ļ�
				// NodeID
				pNew->_ID = child->GetNodeID();
				// ��������
				Matrix3 parentTM = maxNode->GetParentTM(0);
				parentTM.Invert();
				Matrix3 thisTM = maxNode->GetNodeTM(0) * parentTM;
				DumpMatrix(thisTM,&pNew->_Mat);
				pNew->_Mat.ChangeYZ();
				// �����ؼ�֡
				DumpControl(maxNode->GetTMController(),maxNode,pNew);
				// ����涨Mesh�ǲ������ӽڵ��
				for(int i=0;i<child->GetChildCount();i++)
				{
					IGameNode * newchild = child->GetNodeChild(i);
					// �ڹ��ӽڵ�
					ExportChildNodeInfo(newchild,pNew);
				}
			}
			break;
			// Mesh
			case IGameObject::IGAME_MESH:
			{
				IGameMesh * gM = (IGameMesh*)obj;
				gM->InitializeData();
				gM->SetCreateOptimizedNormalList();

				Modifier* modi = FindPhysiqueModifier(maxNode);
				Modifier* skinmod = FindSkinModifier(maxNode);

				// ����Ȩ����Ϣ
				// Ȩ�ص���Ϣ Modifiers
				//int numMod = gM->GetNumModifiers();
				//bool bDump = false;	// �Ƿ񵼳�,����涨���������Ƥ�Ͳ�����
				//IGameSkin* skin = NULL;
				//// �������ֻ����һ����Ƥ
				//if(numMod==1)
				//{
				//	// ����Ȩ��
				//	IGameModifier * m = gM->GetIGameModifier(0);
				//	if(m->IsSkin())
				//	{
				//		// ���������Skin
				//		skin = (IGameSkin*)m;
				//		bDump = true;
				//	}
				//}

				// ������ж�,����߲�����,���ڷǷ�,��������
				if(true)
				{
					// ��ת����
					MATRIX matTrans = MATRIX();
					DumpMatrix(maxNode->GetNodeTM(0),&matTrans);
					matTrans.ChangeYZ();
					// ����һ��Mesh
					MESH* newMesh = new MESH();
					// ����
					newMesh->_Mat = matTrans;
					// ����Mesh����
					DumpMesh(gM,maxNode,newMesh);
					// ����Mesh����֮����ܵ������㶯��
					if(this->m_bExportVertexAnime)
					{
						DumpVertexAnime(gM,maxNode,newMesh);
					}
					// ������Ƥ��Ϣ
					if(modi!=NULL)
					{
						DumpPhysique(modi,newMesh,maxNode);
					}
					else if(skinmod!=NULL)
					{
						DumpSkin(skinmod,newMesh,maxNode);
					}
					// ��������
					m_listMesh.push_back(newMesh);
				}
			}
			break;
		}
	}	
	
	child->ReleaseIGameObject();
}

void maxProject1::DumpPhysique(Modifier* modifier,MESH* pMesh,INode* pNode)
{
	std::map<int,INode*> tmpMap;
	IPhysiqueExport *physiqueInterface=(IPhysiqueExport *)modifier->GetInterface (I_PHYINTERFACE);
	if (physiqueInterface)
	{
		// Get local data
		IPhyContextExport *localData=physiqueInterface->GetContextInterface(pNode);

		if (localData)
		{
			// Use rigid export
			localData->ConvertToRigid (TRUE);

			// Allow blending
			localData->AllowBlending (TRUE);

			// Check same vertices count
			int vertCount=localData->GetNumberVertices();

			if(vertCount==pMesh->_PositionCount)
			{
				pMesh->_pSkinVertices = new SKINVERTEX[vertCount];
				pMesh->_bSkin = true;
				for (int vert=0; vert<vertCount; vert++)
				{
					// Get a vertex interface
					IPhyVertexExport *vertexInterface=localData->GetVertexInterface (vert);
					// Check if it is a rigid vertex or a blended vertex
					IPhyRigidVertex         *rigidInterface=NULL;
					IPhyBlendedRigidVertex  *blendedInterface=NULL;
					int type=vertexInterface->GetVertexType ();
					if (type==RIGID_TYPE)
					{
						// this is a rigid vertex
						rigidInterface=(IPhyRigidVertex*)vertexInterface;
					}
					else
					{
						// It must be a blendable vertex
						blendedInterface=(IPhyBlendedRigidVertex*)vertexInterface;
					}
					// Get bones count for this vertex
					int boneCount;
					if (blendedInterface)
					{
						// If blenvertex, only one bone
						boneCount=blendedInterface->GetNumberNodes();
					}
					else
					{
						// If rigid vertex, only one bone
						boneCount=1;
					}
					// For each bones
					pMesh->_pSkinVertices[vert]._BoneCount = boneCount;
					pMesh->_pSkinVertices[vert]._pBoneIDs = new int[boneCount];
					pMesh->_pSkinVertices[vert]._pWeights = new float[boneCount];
					Point3 offsetPos = Point3(0,0,0);
					Point3 origNor = Point3(0,0,0);	// �����˹�ChangeYZ,Ϊ�˱�֤���������
					Point3 tmpNor = Point3(pMesh->_pNormals[vert]._X,pMesh->_pNormals[vert]._Z,pMesh->_pNormals[vert]._Y);
					Matrix3 tmNoTrans = pNode->GetObjTMAfterWSM(0);
					tmNoTrans.NoTrans();
					tmpNor = tmpNor * Inverse(TransposeMatrix(tmNoTrans));
					for (int bone=0; bone<boneCount; bone++)
					{
						if (blendedInterface)
						{
								// Get the bone weight
								float weight=blendedInterface->GetWeight(bone);
								// Get node
								INode *node=blendedInterface->GetNode(bone);
								// Insert in the map
								pMesh->_pSkinVertices[vert]._pBoneIDs[bone] = (int)node->GetHandle();
								pMesh->_pSkinVertices[vert]._pWeights[bone] = weight;

								Matrix3 tm;
								physiqueInterface->GetInitNodeTM(node,tm);
								offsetPos = offsetPos + blendedInterface->GetOffsetVector(bone) * tm * weight;

								Matrix3 norTm;
								Matrix3 nodeTM = node->GetNodeTM(0);
								nodeTM.Invert();
								norTm = nodeTM * tm;
								//norTm = norTm * weight;
								norTm.NoTrans();
								origNor = origNor + tmpNor * norTm * weight;

								std::map<int,INode*>::iterator itr = tmpMap.find((int)node->GetHandle());
								if(itr!=tmpMap.end())
								{
								}
								else
								{
									tmpMap.insert(std::pair<int,INode*>((int)node->GetHandle(),node));
								}
						}
						else
						{
								// Get node
								INode *node=rigidInterface->GetNode();
								pMesh->_pSkinVertices[vert]._pBoneIDs[bone] = (int)node->GetHandle();
								pMesh->_pSkinVertices[vert]._pWeights[bone] = 1.0f;

								Matrix3 tm;
								physiqueInterface->GetInitNodeTM(node,tm);
								offsetPos = rigidInterface->GetOffsetVector();
								offsetPos = offsetPos * tm;

								Matrix3 norTm;
								Matrix3 nodeTM = node->GetNodeTM(0);
								nodeTM.Invert();
								norTm = nodeTM * tm;
								norTm.NoTrans();
								origNor = tmpNor * norTm;

								std::map<int,INode*>::iterator itr = tmpMap.find((int)node->GetHandle());
								if(itr!=tmpMap.end())
								{
								}
								else
								{
									tmpMap.insert(std::pair<int,INode*>((int)node->GetHandle(),node));
								}
						}
					}
					pMesh->_pPositions[vert] = VECTOR3(offsetPos.x,offsetPos.y,offsetPos.z);
					pMesh->_pPositions[vert].ChangeYZ();
					origNor.Normalize();
					pMesh->_pNormals[vert] = VECTOR3(origNor.x,origNor.y,origNor.z);
					pMesh->_pNormals[vert].ChangeYZ();
					localData->ReleaseVertexInterface (vertexInterface);
				}
				// ��������
				pMesh->_BoneCount = (int)tmpMap.size();
				if(pMesh->_BoneCount>0)
				{
					pMesh->_pBoneMatrices = new MATRIX[pMesh->_BoneCount];
					pMesh->_pBoneIDs = new int[pMesh->_BoneCount];
				}
				// ������ʼ����
				std::map<int,INode*>::iterator itr = tmpMap.begin();
				for(int i=0;itr!=tmpMap.end();++itr,++i)
				{
					INode* bone = itr->second;
					Matrix3 tm;
					physiqueInterface->GetInitNodeTM(bone,tm);
					tm.Invert();
					DumpMatrix(tm,&pMesh->_pBoneMatrices[i]);
					pMesh->_pBoneMatrices[i].ChangeYZ();
					pMesh->_pBoneIDs[i] = itr->first;
				}
			}
		}
		physiqueInterface->ReleaseContextInterface (localData);
	}
	modifier->ReleaseInterface (I_PHYINTERFACE, physiqueInterface);
}

void maxProject1::DumpSkin(Modifier* modifier,MESH* pMesh,INode*pNode)
{
	std::map<int,INode*> tmpMap;
	ISkin* skin = (ISkin*)modifier->GetInterface(I_SKIN);
	if(skin)
	{
		ISkinContextData* skincontext = skin->GetContextInterface(pNode);
		if(skincontext)
		{
			int numVert=skincontext->GetNumPoints();
			if(numVert==pMesh->_PositionCount)
			{
				pMesh->_pSkinVertices = new SKINVERTEX[numVert];
				pMesh->_bSkin = true;
				for(int i=0;i<numVert;i++)
				{
					int numBones=skincontext->GetNumAssignedBones(i);
					pMesh->_pSkinVertices[i]._BoneCount = numBones;
					pMesh->_pSkinVertices[i]._pBoneIDs = new int[numBones];
					pMesh->_pSkinVertices[i]._pWeights = new float[numBones];
					for(int j=0;j<numBones;j++)
					{
						INode* bone=skin->GetBone(skincontext->GetAssignedBone(i,j));//do not use j,but use GetAssignedBone(i,j)
						float weight = skincontext->GetBoneWeight(i,j);
						pMesh->_pSkinVertices[i]._pBoneIDs[j] = (int)bone->GetHandle();
						pMesh->_pSkinVertices[i]._pWeights[j] = weight;

						std::map<int,INode*>::iterator itr = tmpMap.find((int)bone->GetHandle());
						if(itr!=tmpMap.end())
						{
						}
						else
						{
							tmpMap.insert(std::pair<int,INode*>((int)bone->GetHandle(),bone));
						}
					}
				}
				// ��������
				pMesh->_BoneCount = (int)tmpMap.size();
				if(pMesh->_BoneCount>0)
				{
					pMesh->_pBoneMatrices = new MATRIX[pMesh->_BoneCount];
					pMesh->_pBoneIDs = new int[pMesh->_BoneCount];
				}
				// ������ʼ����
				std::map<int,INode*>::iterator itr = tmpMap.begin();
				for(int i=0;itr!=tmpMap.end();++itr,++i)
				{
					INode* bone = itr->second;
					Matrix3 tmBone;
					Point3 trans;
					Quat rotate;
					tmBone = GetTranslationAndRotationBoneSpace(bone,0,trans,rotate);
					DumpMatrix(tmBone,&pMesh->_pBoneMatrices[i]);
					pMesh->_pBoneMatrices[i].ChangeYZ();
					pMesh->_pBoneIDs[i] = itr->first;
				}
			}
		}
		modifier->ReleaseInterface(I_SKIN, skin);
	}
}

void maxProject1::DumpSampleKeys(IGameControl *sc,IGameControlType Type,_SKELETON* pNode)
{
	IGameKeyTab Key;
	IGameControl * c = sc;
	char sz[1024] = {0};

	// framePerSample:FrameRate�̶�Ϊ1
	// Relative:Ĭ��Ϊ��,������Ը��������ƶ�
	c->GetFullSampledKeys(Key,framePerSample,IGameControlType(Type),true);

	// Key Count
	int keyCount = Key.Count();
	pNode->_KeyCount = keyCount;
	if(keyCount>0) pNode->_pKeys = new SAMPLEKEY[keyCount];

	for(int i=0;i<keyCount;i++)
	{
		int fc = Key[i].t;
		// time
		// ����
		pNode->_pKeys[i]._Time = fc;

		// ����ֻ��������
		// ���ҿ϶�Ҫ�������֧
		if(Type == IGAME_TM)
		{
			//Even though its a 4x4 we dump it as a 4x3 ;-)
			DumpMatrix(Key[i].sampleKey.gval.ExtractMatrix3(),&pNode->_pKeys[i]._Mat);
			pNode->_pKeys[i]._Mat.ChangeYZ();
		}
	}
}

void maxProject1::DumpControl(Control* control,INode* maxNode,_SKELETON* pNode)
{
	if(!control || !pNode || !maxNode)
		return;

	int count = 0;
	count = (pIgame->GetSceneEndTime() / pIgame->GetSceneTicks()) -
		(pIgame->GetSceneStartTime() / pIgame->GetSceneTicks()) + 1;
	pNode->_KeyCount = count;
	if(count>0)
	{
		pNode->_pKeys = new SAMPLEKEY[count];
	}
	for(int i=0;i<count;i++)
	{
		TimeValue time = i * pIgame->GetSceneTicks();
		pNode->_pKeys[i]._Time = time;
		Matrix3 tm = maxNode->GetNodeTM(time);
		DumpMatrix(tm,&pNode->_pKeys[i]._Mat);
		pNode->_pKeys[i]._Mat.ChangeYZ();
	}
}

void maxProject1::DumpVertexAnime(IGameMesh* gm,INode* pNode,MESH* pMesh)
{
	if(!gm || !pNode || !pMesh)
		return;
	if(pMesh->_PositionCount <=0 || pMesh->_NormalCount<=0)
		return;
	int count = 0;
	count = (pIgame->GetSceneEndTime() / pIgame->GetSceneTicks()) -
		(pIgame->GetSceneStartTime() / pIgame->GetSceneTicks()) + 1;
	pMesh->_KeyCount = count;
	if(count<=0)
		return;
	pMesh->_pVAPositions = new VECTOR3*[count];
	pMesh->_pVANormals = new VECTOR3*[count];
	pMesh->_pVATUVOff = new VECTOR3*[count];
	for(int ikey=0;ikey<count;ikey++)
	{
		TimeValue time = ikey * pIgame->GetSceneTicks();
		Mesh* maxMesh = FindMesh(pNode,time);
		Matrix3 tm = pNode->GetObjectTM(time);
		if(maxMesh)
		{
			maxMesh->buildNormals();
			maxMesh->checkNormals(TRUE);
			int num = maxMesh->getNumVerts();
			pMesh->_pVAPositions[ikey] = new VECTOR3[num];
			pMesh->_pVANormals[ikey] = new VECTOR3[num];
			for(int i = 0;i<num;i++)
			{
				Point3 v; 
				v = maxMesh->getVert(i);
				v = v*tm;
				pMesh->_pVAPositions[ikey][i] = VECTOR3(v.x,v.y,v.z);
				pMesh->_pVAPositions[ikey][i].ChangeYZ();
			}
			ComputeVertexNormals(maxMesh,pMesh->_pVANormals[ikey],tm);
			// texture vertex
			int texVerts = maxMesh->getNumTVerts();
			pMesh->_pVATUVOff[ikey] = new VECTOR3[texVerts];
			for(int i=0;i<texVerts;i++)
			{
				UVVert texVert = maxMesh->getTVert(i);
				pMesh->_pVATUVOff[ikey][i] = VECTOR3(0,0,0);
			}
		}

		// dump Texture Verts
		//int texVerts = gm->GetNumberOfTexVerts();
		//pMesh->_TexVertexCount = texVerts;
		//pMesh->_pTexVertices = new VECTOR3[texVerts];
		//for(int i=0;i<texVerts;i++)
		//{
		//	Point2 n;
		//	if(gm->GetTexVertex(i,n))
		//	{
		//		// ����ӳ��
		//		pMesh->_pTexVertices[i] = VECTOR3(n.x,n.y + 1.0f,0);
		//	}
		//}

		// dump Face data
		int numFaces = gm->GetNumberOfFaces();
		//int numFaces = maxMesh->getNumFaces();
		//pMesh->_FaceCount = numFaces;
		//pMesh->_pFaces = new FACE[numFaces];

		for(int n=0;n<numFaces;n++)
		{
			for(int i=0;i<3;i++)
			{
				int vertexIndex = gm->GetFaceVertex(n,i);
				int normalIndex = gm->GetFaceVertexNormal(n,i);
				int texIndex = gm->GetFaceTextureVertex(n,i);

				//int vertexIndex = maxMesh->faces[n].v[i];
				//int normalIndex = vertexIndex;
				//int texIndex = maxMesh->tvFace[n].t[i];

				IGameMaterial* pFaceMat = gm->GetMaterialFromFace(n);

				if(pFaceMat)
				{
					StdMat* pStdMat = (StdMat*)pFaceMat->GetMaxMaterial();
					if(pStdMat)
					{
						int texCount = pStdMat->NumSubTexmaps();
						if(texCount>0)
						{
							Texmap* pTexmap = pStdMat->GetSubTexmap(1);
							if(pTexmap)
							{
								StdUVGen* pStdUVGen = (StdUVGen*)pTexmap->GetTheUVGen();
								if(pStdUVGen)
								{
									// ÿ�����Ӧ�Ĳ��ʿ��ܲ�һ�����������ǵ�������Ҳ��һ��
									pMesh->_pVATUVOff[ikey][texIndex]._X = pStdUVGen->GetUOffs(time);
									pMesh->_pVATUVOff[ikey][texIndex]._Y = pStdUVGen->GetVOffs(time);
								}
							}
						}
					}
				}

				//pMesh->_pFaces[n]._NormalIndex[i] = vertexIndex;
				//pMesh->_pFaces[n]._PositionIndex[i] = vertexIndex;
				//pMesh->_pFaces[n]._TexVertexIndex[i] = texIndex;

				Point3 newNormal = GetVertexNormal(maxMesh,n,vertexIndex);
				newNormal = newNormal * Inverse(TransposeMatrix(tm));
				newNormal = newNormal.Normalize();
				pMesh->_pVANormals[ikey][vertexIndex] = VECTOR3(newNormal.x,newNormal.y,newNormal.z);
				pMesh->_pVANormals[ikey][vertexIndex].ChangeYZ();
			}

			//// ��Ӧ�Ĳ��ʵ�����,û�в�����None
			//IGameMaterial* pFaceMat = gm->GetMaterialFromFace(n);
			//if(pFaceMat)
			//{
			//	sprintf(pMesh->_pFaces[n]._szMatName,"%s",pFaceMat->GetMaterialName());
			//}
			//else
			//{
			//	sprintf(pMesh->_pFaces[n]._szMatName,"None");
			//}
		}
	}
}

void maxProject1::DumpMatrix(Matrix3 tm,MATRIX* pMatrix)
{
	AffineParts ap;
	float rotAngle;
	Point3 rotAxis;
	float scaleAxAngle;
	Point3 scaleAxis;
	Matrix3 m = tm;
	TSTR Buf;

	char sz[1024]={0};
	
	decomp_affine(m, &ap);

	// Quaternions are dumped as angle axis.
	AngAxisFromQ(ap.q, &rotAngle, rotAxis);
	AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);

	if(pMatrix == NULL)
	{
		// д�ļ�
		// Translation
		//sprintf(sz,"%f %f %f\n",ap.t.x,ap.t.y,ap.t.z);
		//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

		// д�ļ�
		// Rotation
		// ������Ԫ��
		//sprintf(sz,"%f %f %f %f\n",rotAxis.x, rotAxis.y, rotAxis.z, rotAngle);
		//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

		// д�ļ�
		// Scale
		// ������Ԫ��
		//sprintf(sz,"%f %f %f\n", ap.k.x, ap.k.y, ap.k.z);
		//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);

		sprintf(sz,"%f %f %f %f %f %f %f %f %f %f %f %f\n",m.GetRow(0).x,m.GetRow(0).y,m.GetRow(0).z,m.GetRow(1).x,m.GetRow(1).y,m.GetRow(1).z,
			m.GetRow(2).x,m.GetRow(2).y,m.GetRow(2).z,m.GetRow(3).x,m.GetRow(3).y,m.GetRow(3).z);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	}
	else
	{
		pMatrix->_Translation = VECTOR3(ap.t.x,ap.t.y,ap.t.z);
		//pMatrix->_RotationAxis = VECTOR3(rotAxis.x,rotAxis.y,rotAxis.z);
		//pMatrix->_Angle = rotAngle;
		pMatrix->_Quaternion = ap.q;
		pMatrix->_Scale = VECTOR3(ap.k.x,ap.k.y,ap.k.z);
	}
}

void maxProject1::DumpMesh(IGameMesh *gm,INode* pNode,MESH* pMesh)
{
	TSTR buf;
	// д�ļ�
	char sz[1024]={0};

	// ����staticFrameȡ0
	Mesh* maxMesh = FindMesh(pNode,0);
	Matrix3 tm = pNode->GetObjectTM(0);

	if(maxMesh)
	{
		maxMesh->buildNormals();
		maxMesh->checkNormals(TRUE);
		int num = maxMesh->getNumVerts();
		pMesh->_PositionCount = num;
		pMesh->_NormalCount = num;
		pMesh->_pPositions = new VECTOR3[num];
		pMesh->_pNormals = new VECTOR3[num];
		for(int i = 0;i<num;i++)
		{
			Point3 v; 
			v = maxMesh->getVert(i);
			v = v*tm;
			pMesh->_pPositions[i] = VECTOR3(v.x,v.y,v.z);
			pMesh->_pPositions[i].ChangeYZ();
			//v = maxMesh->getNormal(i);
			//pMesh->_pNormals[i] = VECTOR3(v.x,v.y,v.z);
			//pMesh->_pNormals[i].ChangeYZ();
		}

		ComputeVertexNormals(maxMesh,pMesh->_pNormals,tm);

		int texVerts = maxMesh->getNumTVerts();
		pMesh->_TexVertexCount = texVerts;
		pMesh->_pTexVertices = new VECTOR3[texVerts];
		for(int i=0;i<texVerts;i++)
		{
			UVVert texVert = maxMesh->getTVert(i);
			pMesh->_pTexVertices[i] = VECTOR3(texVert.x,1.0-texVert.y,0);
		}
	}

	//// dump Vertices
	//int numVerts = gm->GetNumberOfVerts();
	//pMesh->_PositionCount = numVerts;
	//pMesh->_pPositions = new VECTOR3[numVerts];
	//for(int i = 0;i<numVerts;i++)
	//{
	//	Point3 v; 
	//	if(gm->GetVertex(i,v,true))
	//	{
	//		// ������
	//		pMesh->_pPositions[i] = VECTOR3(v.x,v.y,v.z);
	//	}
	//}
	//
	//// dump Normals
	//int numNorms = gm->GetNumberOfNormals();
	//pMesh->_NormalCount = numNorms;
	//pMesh->_pNormals = new VECTOR3[numNorms];
	//for(int i = 0;i<numNorms;i++)
	//{
	//	Point3 n;
	//	if(gm->GetNormal(i,n,true))
	//	{
	//		// ��������
	//		pMesh->_pNormals[i] = VECTOR3(n.x,n.y,n.z);
	//	}
	//}

	//// dump Texture Verts
	//int texVerts = gm->GetNumberOfTexVerts();
	//pMesh->_TexVertexCount = texVerts;
	//pMesh->_pTexVertices = new VECTOR3[texVerts];
	//for(int i=0;i<texVerts;i++)
	//{
	//	Point2 n;
	//	if(gm->GetTexVertex(i,n))
	//	{
	//		// ����ӳ��
	//		pMesh->_pTexVertices[i] = VECTOR3(n.x,n.y + 1.0f,0);
	//	}
	//}

	// dump Face data
	int numFaces = gm->GetNumberOfFaces();
	pMesh->_FaceCount = numFaces;
	pMesh->_pFaces = new FACE[numFaces];

	for(int n=0;n<numFaces;n++)
	{
		for(int i=0;i<3;i++)
		{
			int vertexIndex = gm->GetFaceVertex(n,i);
			int normalIndex = gm->GetFaceVertexNormal(n,i);
			int texIndex = gm->GetFaceTextureVertex(n,i);

			pMesh->_pFaces[n]._NormalIndex[i] = vertexIndex;
			pMesh->_pFaces[n]._PositionIndex[i] = vertexIndex;
			pMesh->_pFaces[n]._TexVertexIndex[i] = texIndex;

			Point3 newNormal = GetVertexNormal(maxMesh,n,vertexIndex);
			newNormal = newNormal * Inverse(TransposeMatrix(tm));
			newNormal = newNormal.Normalize();
			pMesh->_pNormals[vertexIndex] = VECTOR3(newNormal.x,newNormal.y,newNormal.z);
			pMesh->_pNormals[vertexIndex].ChangeYZ();
		}

		// ��Ӧ�Ĳ��ʵ�����,û�в�����None
		IGameMaterial* pFaceMat = gm->GetMaterialFromFace(n);
		if(pFaceMat)
		{
			sprintf(pMesh->_pFaces[n]._szMatName,"%s",pFaceMat->GetMaterialName());
		}
		else
		{
			sprintf(pMesh->_pFaces[n]._szMatName,"None");
		}
	}
}

void maxProject1::UpdateMapForTex(std::map<int,VERTEXDESC*>* pMap,FACE* pFace,MESH* pMesh)
{
	if(!pMap || !pFace || !pMesh)
	{
		return;
	}
	// ѭ��������
	for(int i=0;i<3;i++)
	{
		int posIndex = pFace->_PositionIndex[i];
		int texIndex = pFace->_TexVertexIndex[i];
		int normalIndex = pFace->_NormalIndex[i];
		VERTEXDESC* thisVertex = new VERTEXDESC();
		thisVertex->_Position = pMesh->_pPositions[posIndex];
		thisVertex->_Tex = pMesh->_pTexVertices[texIndex];
		thisVertex->_Normal = pMesh->_pNormals[normalIndex];
		if(pMesh->_pSkinVertices!=NULL)
		{
			thisVertex->_Skin = pMesh->_pSkinVertices[posIndex];
		}
		// ����Ӱ��
		std::map<int,VERTEXDESC*>::iterator itr = pMap->begin();
		bool bFind = false;
		for(;itr!=pMap->end();++itr)
		{
			VERTEXDESC* tmpVertex = itr->second;
			// �����һ����λ����ȵ�
			if(tmpVertex->_Position == thisVertex->_Position)
			{
				// �������Ҳ���,��ֵ������Face
				// ���������,�����ѭ��
				if(tmpVertex->_Tex == thisVertex->_Tex)
				{
					itr->second->_Normal = thisVertex->_Normal;
					itr->second->_Skin = thisVertex->_Skin;
					pFace->_FinalIndex[i] = itr->first;
					bFind = true;
					break;
				}
			}
		}
		// ���û���ҵ�һ��ƥ���,�����һ����ֵ,Ȼ��ֵ������Face
		if(!bFind)
		{
			pMap->insert(pair<int,VERTEXDESC*>((int)pMap->size(),thisVertex));
			pFace->_FinalIndex[i] = (int)pMap->size() - 1;
		}
		else
		{
			delete thisVertex;
			thisVertex = NULL;
		}
	}
}

void maxProject1::UpdateMapForTexForVA(std::map<int,VERTEXDESC*>* pMap,FACE* pFace,MESH* pMesh,int ikey)
{
	if(!pMap || !pFace || !pMesh)
	{
		return;
	}
	// ѭ��������
	for(int i=0;i<3;i++)
	{
		int posIndex = pFace->_PositionIndex[i];
		int texIndex = pFace->_TexVertexIndex[i];
		int normalIndex = pFace->_NormalIndex[i];
		VERTEXDESC* thisVertex = new VERTEXDESC();
		thisVertex->_Position = pMesh->_pVAPositions[ikey][posIndex];
		thisVertex->_Tex = pMesh->_pTexVertices[texIndex];
		// ÿ�����Ӧ��UVOFF���ܲ�һ��������Ҫ���������
		thisVertex->_Tex._X -= pMesh->_pVATUVOff[ikey][texIndex]._X;
		thisVertex->_Tex._Y -= pMesh->_pVATUVOff[ikey][texIndex]._Y;
		thisVertex->_Normal = pMesh->_pVANormals[ikey][normalIndex];
		/*if(pMesh->_pSkinVertices!=NULL)
		{
			thisVertex->_Skin = pMesh->_pSkinVertices[posIndex];
		}*/
		// ����Ӱ��
		std::map<int,VERTEXDESC*>::iterator itr = pMap->begin();
		bool bFind = false;
		for(;itr!=pMap->end();++itr)
		{
			VERTEXDESC* tmpVertex = itr->second;
			// �����һ����λ����ȵ�
			if(tmpVertex->_Position == thisVertex->_Position)
			{
				// �������Ҳ���,��ֵ������Face
				// ���������,�����ѭ��
				if(tmpVertex->_Tex == thisVertex->_Tex)
				{
					itr->second->_Normal = thisVertex->_Normal;
					itr->second->_Skin = thisVertex->_Skin;
					pFace->_FinalIndex[i] = itr->first;
					bFind = true;
					break;
				}
			}
		}
		// ���û���ҵ�һ��ƥ���,�����һ����ֵ,Ȼ��ֵ������Face
		if(!bFind)
		{
			pMap->insert(pair<int,VERTEXDESC*>((int)pMap->size(),thisVertex));
			pFace->_FinalIndex[i] = (int)pMap->size() - 1;
		}
		else
		{
			delete thisVertex;
			thisVertex = NULL;
		}
	}
}

void maxProject1::UpdateTangentVector(VERTEXDESC* pV0,VERTEXDESC* pV1,VERTEXDESC* pV2)
{
	// Step 1. Compute the approximate tangent vector.
	double du1 = pV1->_Tex._X - pV0->_Tex._X;
	double dv1 = pV1->_Tex._Y - pV0->_Tex._Y;
	double du2 = pV2->_Tex._X - pV0->_Tex._X;
	double dv2 = pV2->_Tex._Y - pV0->_Tex._Y;

	double prod1 = (du1*dv2-dv1*du2);
	double prod2 = (du2*dv1-dv2*du1);
	if ((fabs(prod1) < 0.000001)||(fabs(prod2) < 0.000001)) return;

	double x = dv2/prod1;
	double y = dv1/prod2;

	VECTOR3 vec1 = pV1->_Position - pV0->_Position;
	VECTOR3 vec2 = pV2->_Position - pV0->_Position;
	VECTOR3 tangent = (vec1 * ((float)x)) + (vec2 * ((float)y));

	// Step 2. Orthonormalize the tangent.
	double component = (tangent * pV0->_Normal);
	tangent -= (pV0->_Normal * ((float)component));
	tangent.Normalize();

	// Step 3: Add the estimated tangent to the overall estimate for the vertex.
	pV0->_Tan += tangent;
}

void maxProject1::UpdateMapForNormal(std::map<int,VERTEXDESC*>* pMap,FACE* pFace,MESH* pMesh)
{
	if(!pMap || !pFace || !pMesh)
	{
		return;
	}
	// ѭ��������
	for(int i=0;i<3;i++)
	{
		int posIndex = pFace->_PositionIndex[i];
		int texIndex = pFace->_TexVertexIndex[i];
		int normalIndex = pFace->_NormalIndex[i];
		VERTEXDESC* thisVertex = new VERTEXDESC();
		thisVertex->_Position = pMesh->_pPositions[posIndex];
		thisVertex->_Tex = pMesh->_pTexVertices[texIndex];
		thisVertex->_Normal = pMesh->_pNormals[normalIndex];
		if(pMesh->_pSkinVertices!=NULL)
		{
			thisVertex->_Skin = pMesh->_pSkinVertices[posIndex];
		}
		// ����Ӱ��
		std::map<int,VERTEXDESC*>::iterator itr = pMap->begin();
		bool bFind = false;
		for(;itr!=pMap->end();++itr)
		{
			VERTEXDESC* tmpVertex = itr->second;
			// ���ʱ��FACE�����Ѿ���������
			if(itr->first == pFace->_FinalIndex[i])
			{
				// ���������ȣ���ʲôҲ����
				// ������߲��ȣ���������
				if(tmpVertex->_Normal == thisVertex->_Normal)
				{
					bFind = true;
				}
				thisVertex->_Position = tmpVertex->_Position;
				thisVertex->_Skin = tmpVertex->_Skin;
				thisVertex->_Tex = tmpVertex->_Tex;
				break;
			}
		}
		// ���û���ҵ�һ��ƥ���,�����һ����ֵ,Ȼ��ֵ������Face
		if(!bFind)
		{
			pMap->insert(pair<int,VERTEXDESC*>((int)pMap->size(),thisVertex));
			pFace->_FinalIndex[i] = (int)pMap->size() - 1;
		}
		else
		{
			delete thisVertex;
			thisVertex = NULL;
		}
	}
}

int	maxProject1::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	#pragma message(TODO("Implement the actual file Export here and"))

	int nResult = 0;
	if(!suppressPrompts)
	{
		nResult = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), maxProject1OptionsDlgProc, (LPARAM)this);
	}
	if(nResult!=1)
		return TRUE;

	// *******************************************
	// ��ʼ���ļ�
	m_saveFile = _tfopen(name,L"w+");
	if(m_saveFile==NULL)
	{
		::MessageBoxA(NULL,"ERROR","ERROR",NULL);
		return FALSE;
	}
	// *******************************************

	char sz[1024]={0};
	// �ļ�ͷ
	// ����ǹ�������,���ҵ�������ʱ
	if((this->m_bExportSkeletonMesh || this->m_bExportVertexAnime || this->m_bExportNodeAnime) && (this->m_bExportAnime))
	{
		sprintf(sz,"TANIME_100\n");
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// ����д�붯������
		if(this->m_bExportSkeletonMesh)
		{
			// ��������
			sprintf(sz,"SKELETONANIME\n");
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		}
		else if(this->m_bExportVertexAnime)
		{
			// ���㶯��
			sprintf(sz,"VERTEXANIME\n");
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		}
		else if(this->m_bExportNodeAnime)
		{
			// �켣����
			sprintf(sz,"NODEANIME\n");
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		}
	}
	else
	{
		sprintf(sz,"TMESH_100\n");
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	}

	// һЩ��ʼ������
	Interface * ip = GetCOREInterface();
	pIgame = GetIGameInterface();
	IGameConversionManager * cm = GetConversionManager();
	//	cm->SetUserCoordSystem(Whacky);
	// ����̶���D3D����ϵ
	cm->SetCoordSystem(IGameConversionManager::IGAME_D3D);
	//	pIgame->SetPropertyFile(_T("hello world"));
	bool exportSelected = (options & SCENE_EXPORT_SELECTED) ? true : false;
	pIgame->InitialiseIGame(exportSelected);
	// �����ȸ�һ���̶�ֵ0
	pIgame->SetStaticFrame(0);

	// ����
	// �����������ʱ
	if((this->m_bExportSkeletonMesh || this->m_bExportVertexAnime || this->m_bExportNodeAnime) && this->m_bExportAnime)
	{
		ExportSceneInfo();
	}
	// ����
	// �������ģ�͵�ʱ��
	else
	{
		ExportMaterials();
	}

	// �ڵ�
	m_listMesh.clear();
	m_pSkeletonRoot = new _SKELETON();
	m_pSkeletonRoot->_pChildList = new std::list<_SKELETON*>();

	for(int loop = 0; loop <pIgame->GetTopLevelNodeCount();loop++)
	{
		IGameNode * pGameNode = pIgame->GetTopLevelNode(loop);
		//check for selected state - we deal with targets in the light/camera section
		if(pGameNode->IsTarget())
			continue;
		ExportChildNodeInfo(pGameNode,m_pSkeletonRoot);
	}

	// �����һ�ֶ���
	if((this->m_bExportSkeletonMesh || this->m_bExportVertexAnime || this->m_bExportNodeAnime) && this->m_bExportAnime)
	{
		if(this->m_bExportSkeletonMesh)
		{
			// �ȵ�������
			sprintf(sz,"%s\n",m_szAnimeName);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// �����Ӹ��ĵ�һ�����ӿ�ʼд��
			// �Ȱѵ�һ����������
			sprintf(sz,"%d\n",m_pSkeletonRoot->_pChildList->size());
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			std::list<_SKELETON*>::iterator itr = m_pSkeletonRoot->_pChildList->begin();
			for(;itr!=m_pSkeletonRoot->_pChildList->end();++itr)
				CopyAnimeInfoToFile((*itr));
		}
		else if(this->m_bExportVertexAnime)
		{
			// ���㶯��
			// �ȵ�������
			sprintf(sz,"%s\n",m_szAnimeName);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// ��������
			CopyMeshInfoToFileForVA();
		}
		else if(this->m_bExportNodeAnime)
		{
			// �켣����
		}
	}
	// �����ģ��
	else
	{
		// ����ͳһ�������ļ�
		CopyMeshInfoToFile();
		// �����Ӹ��ĵ�һ�����ӿ�ʼд��
		// �Ȱѵ�һ����������
		sprintf(sz,"%d\n",m_pSkeletonRoot->_pChildList->size());
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		std::list<_SKELETON*>::iterator itr = m_pSkeletonRoot->_pChildList->begin();
		for(;itr!=m_pSkeletonRoot->_pChildList->end();++itr)
			CopySkeletonInfoToFile((*itr));
	}

	// ����
	m_listMesh.clear();
	delete m_pSkeletonRoot;
	m_pSkeletonRoot = NULL;
	pIgame->ReleaseIGame();

	// ** �ر��ļ� ************

	fclose(m_saveFile);
	::MessageBoxA(NULL,"�������","��ȷ",NULL);

	// ************************

	#pragma message(TODO("return TRUE If the file is exported properly"))
	return TRUE;
}

void maxProject1::CopyMeshInfoToFile()
{
	char sz[1024]={0};
	if(m_listMesh.size()<=0)
	{
		MessageBoxA(NULL,"Error","MeshSizeError",MB_OK);
		return;
	}

	// -------------------------- ͳһ����,����ָ�� -----------------------------
	VertexDescMap* tmpMap = new VertexDescMap[m_listMesh.size()];
	MeshList::iterator itr = m_listMesh.begin();
	int count = 0;
	for(;itr!=m_listMesh.end();++itr,++count)
	{
		MESH* pMesh = (MESH*)(*itr);
		for(int i=0;i<pMesh->_FaceCount;i++)
		{
			FACE* pFace = &pMesh->_pFaces[i];
			UpdateMapForTex(&tmpMap[count],pFace,pMesh);
		}
		/*for(int i=0;i<pMesh->_FaceCount;i++)
		{
			FACE* pFace = &pMesh->_pFaces[i];
			UpdateMapForNormal(&tmpMap[count],pFace,pMesh);
		}*/
	}
	// --------------------------------------------------------------------------

	// Mesh����
	// MeshCount
	sprintf(sz,"%d\n",m_listMesh.size());
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// ��������
	itr = m_listMesh.begin();
	count = 0;
	for(;itr!=m_listMesh.end();++itr,++count)
	{
		MESH* pMesh = *itr;
		// Calculate Tangent
		for(int i=0;i<pMesh->_FaceCount;i++)
		{
			FACE face = pMesh->_pFaces[i];
			int vertIndex[3] = {face._FinalIndex[0],face._FinalIndex[1],face._FinalIndex[2]};
			VERTEXDESC* vert[3];
			for(int v=0;v<3;v++)
			{
				VertexDescMap::iterator itrMap = tmpMap[count].find(vertIndex[v]);
				vert[v] = itrMap->second;
			}
			UpdateTangentVector(vert[0],vert[1],vert[2]);
			UpdateTangentVector(vert[1],vert[2],vert[0]);
			UpdateTangentVector(vert[2],vert[0],vert[1]);
		}
		// Mesh_Start
		sprintf(sz,"Mesh_Start\n");
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// VertexCount
		sprintf(sz,"%d\n",tmpMap[count].size());
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// Vertices
		VertexDescMap::iterator itrMap = tmpMap[count].begin();
		for(;itrMap!=tmpMap[count].end();++itrMap)
		{
			int pos = itrMap->first;
			VERTEXDESC desc = *(itrMap->second);
			// Position
			sprintf(sz,"%f %f %f\n",desc._Position._X,desc._Position._Y,desc._Position._Z);
		 	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// Normal
			sprintf(sz,"%f %f %f\n",desc._Normal._X,desc._Normal._Y,desc._Normal._Z);
		 	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// Tex
			sprintf(sz,"%f %f\n",desc._Tex._X,desc._Tex._Y);
		 	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// Tangent
			// Normalize Vector
			desc._Tan.Normalize();
			sprintf(sz,"%f %f %f\n",desc._Tan._X,desc._Tan._Y,desc._Tan._Z);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// Skin
			// BoneCount
			sprintf(sz,"%d\n",desc._Skin._BoneCount);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// ѭ����¼Ȩ����Ϣ
			for(int j=0;j<desc._Skin._BoneCount;j++)
			{
				sprintf(sz,"%d %f\n",desc._Skin._pBoneIDs[j],desc._Skin._pWeights[j]);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			}
		}
		// FaceCount
		sprintf(sz,"%d\n",pMesh->_FaceCount);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// Faces
		for(int i=0;i<pMesh->_FaceCount;i++)
		{
			FACE face = pMesh->_pFaces[i];
			// index
			sprintf(sz,"%d %d %d\n",face._FinalIndex[0],face._FinalIndex[1],face._FinalIndex[2]);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// MaterialName
			sprintf(sz,"%s\n",face._szMatName);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		}
		// Matrix
		sprintf(sz,"%f %f %f\n",pMesh->_Mat._Translation._X,pMesh->_Mat._Translation._Y,pMesh->_Mat._Translation._Z);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		//sprintf(sz,"%f %f %f\n",pMesh->_Mat._RotationAxis._X,pMesh->_Mat._RotationAxis._Y,pMesh->_Mat._RotationAxis._Z);
		//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		//sprintf(sz,"%f\n",pMesh->_Mat._Angle);
		//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		sprintf(sz,"%f %f %f %f\n",pMesh->_Mat._Quaternion.x,pMesh->_Mat._Quaternion.y,pMesh->_Mat._Quaternion.z,pMesh->_Mat._Quaternion.w);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		sprintf(sz,"%f %f %f\n",pMesh->_Mat._Scale._X,pMesh->_Mat._Scale._Y,pMesh->_Mat._Scale._Z);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		if(pMesh->_bSkin)
		{
			// Skin
			sprintf(sz,"IsSkin\n");
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// BoneCount
			sprintf(sz,"%d\n",pMesh->_BoneCount);
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// Bone Matrices
			for(int i=0;i<pMesh->_BoneCount;i++)
			{
				sprintf(sz,"%d\n",pMesh->_pBoneIDs[i]);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				sprintf(sz,"%f %f %f\n",pMesh->_pBoneMatrices[i]._Translation._X,pMesh->_pBoneMatrices[i]._Translation._Y,pMesh->_pBoneMatrices[i]._Translation._Z);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				//sprintf(sz,"%f %f %f\n",pMesh->_pBoneMatrices[i]._RotationAxis._X,pMesh->_pBoneMatrices[i]._RotationAxis._Y,pMesh->_pBoneMatrices[i]._RotationAxis._Z);
				//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				//sprintf(sz,"%f\n",pMesh->_pBoneMatrices[i]._Angle);
				//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				sprintf(sz,"%f %f %f %f\n",pMesh->_pBoneMatrices[i]._Quaternion.x,pMesh->_pBoneMatrices[i]._Quaternion.y,
					pMesh->_pBoneMatrices[i]._Quaternion.z,pMesh->_pBoneMatrices[i]._Quaternion.w);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				sprintf(sz,"%f %f %f\n",pMesh->_pBoneMatrices[i]._Scale._X,pMesh->_pBoneMatrices[i]._Scale._Y,pMesh->_pBoneMatrices[i]._Scale._Z);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			}
		}
		else
		{
			// Skin
			sprintf(sz,"NoSkin\n");
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		}
		// Mesh_End
		sprintf(sz,"Mesh_End\n");
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	}

	// ������Դ
	itr = m_listMesh.begin();
	count = 0;
	for(;itr!=m_listMesh.end();++itr,++count)
	{
		/*VertexDescMap::iterator itrMap = tmpMap[count].begin();
		for(;itrMap!=tmpMap[count].end();)
		{
			VERTEXDESC* desc = itrMap->second;
			if(desc)
			{
				delete desc;
				desc = NULL;
			}
		}*/
		tmpMap[count].clear();
	}
	delete [] tmpMap;
	tmpMap = NULL;
}

void maxProject1::CopyMeshInfoToFileForVA()
{
	char sz[1024]={0};
	if(m_listMesh.size()<=0)
	{
		MessageBoxA(NULL,"Error","MeshSizeError",MB_OK);
		return;
	}

	// -------------------------- ͳһ����,����ָ�� -----------------------------
	MeshList::iterator itr = m_listMesh.begin();
	int meshCount = 0;
	VertexDescMap** tmpMap = new VertexDescMap*[m_listMesh.size()];
	for(;itr!=m_listMesh.end();++itr,++meshCount)
	{
		int count = 0;
		MESH* pMesh = (MESH*)(*itr);
		if(pMesh->_KeyCount<=0)
		{
			MessageBoxA(NULL,"Error","KeyCountError",MB_OK);
			return;
		}
		// ����ÿһ֡�ĵ㼯
		tmpMap[meshCount] = new VertexDescMap[pMesh->_KeyCount];
		for(;count<pMesh->_KeyCount;count++)
		{
			for(int i=0;i<pMesh->_FaceCount;i++)
			{
				FACE* pFace = &pMesh->_pFaces[i];
				UpdateMapForTexForVA(&tmpMap[meshCount][count],pFace,pMesh,count);
			}
		}
		// --------------------------------------------------------------------------
	}

	// MeshCount
	sprintf(sz,"%d\n",m_listMesh.size());
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// Mesh����
	itr = m_listMesh.begin();
	meshCount = 0;
	for(;itr!=m_listMesh.end();++itr,++meshCount)
	{
		int count = 0;
		MESH* pMesh = (MESH*)(*itr);
		// Mesh_Start
		sprintf(sz,"Mesh_Start\n");
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// KeyCount
		sprintf(sz,"%d\n",pMesh->_KeyCount);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		// �����ؼ�֡
		for(;count<pMesh->_KeyCount;count++)
		{
			// Calculate Tangent
			for(int i=0;i<pMesh->_FaceCount;i++)
			{
				FACE face = pMesh->_pFaces[i];
				int vertIndex[3] = {face._FinalIndex[0],face._FinalIndex[1],face._FinalIndex[2]};
				VERTEXDESC* vert[3];
				for(int v=0;v<3;v++)
				{
					VertexDescMap::iterator itrMap = tmpMap[meshCount][count].find(vertIndex[v]);
					vert[v] = itrMap->second;
				}
				UpdateTangentVector(vert[0],vert[1],vert[2]);
				UpdateTangentVector(vert[1],vert[2],vert[0]);
				UpdateTangentVector(vert[2],vert[0],vert[1]);
			}
			// VertexCount
			sprintf(sz,"%d\n",tmpMap[meshCount][count].size());
			fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			// Vertices
			VertexDescMap::iterator itrMap = tmpMap[meshCount][count].begin();
			for(;itrMap!=tmpMap[meshCount][count].end();++itrMap)
			{
				int pos = itrMap->first;
				VERTEXDESC desc = *(itrMap->second);
				// Position
				sprintf(sz,"%f %f %f\n",desc._Position._X,desc._Position._Y,desc._Position._Z);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				// Normal
				sprintf(sz,"%f %f %f\n",desc._Normal._X,desc._Normal._Y,desc._Normal._Z);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				// Tex
				sprintf(sz,"%f %f\n",desc._Tex._X,desc._Tex._Y);
		 		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
				// Tangent
				// Normalize Vector
				desc._Tan.Normalize();
				sprintf(sz,"%f %f %f\n",desc._Tan._X,desc._Tan._Y,desc._Tan._Z);
				fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
			}
		}
		// Mesh_End
		sprintf(sz,"Mesh_End\n");
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	}

	// ������Դ
	itr = m_listMesh.begin();
	meshCount = 0;
	for(;itr!=m_listMesh.end();++itr,++meshCount)
	{
		MESH* pMesh = (MESH*)(*itr);
		int count = 0;
		for(;count<pMesh->_KeyCount;count++)
		{
			/*VertexDescMap::iterator itrMap = tmpMap[count].begin();
			for(;itrMap!=tmpMap[count].end();)
			{
			VERTEXDESC* desc = itrMap->second;
			if(desc)
			{
			delete desc;
			desc = NULL;
			}
			}*/
			tmpMap[meshCount][count].clear();
		}
		delete [] tmpMap[meshCount];
		tmpMap[meshCount] = NULL;
	}
	delete [] tmpMap;
	tmpMap = NULL;
}

void maxProject1::CopySkeletonInfoToFile(_SKELETON* pRoot)
{
	char sz[1024]={0};
	// Node_Start
	sprintf(sz,"Node_Start\n");
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// Name
	sprintf(sz,"%s\n",pRoot->_szName);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// ID
	sprintf(sz,"%d\n",pRoot->_ID);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// Matrix
	sprintf(sz,"%f %f %f\n",pRoot->_Mat._Translation._X,pRoot->_Mat._Translation._Y,pRoot->_Mat._Translation._Z);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	//sprintf(sz,"%f %f %f\n",pRoot->_Mat._RotationAxis._X,pRoot->_Mat._RotationAxis._Y,pRoot->_Mat._RotationAxis._Z);
	//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	//sprintf(sz,"%f\n",pRoot->_Mat._Angle);
	//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	sprintf(sz,"%f %f %f %f\n",pRoot->_Mat._Quaternion.x,pRoot->_Mat._Quaternion.y,pRoot->_Mat._Quaternion.z,pRoot->_Mat._Quaternion.w);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	sprintf(sz,"%f %f %f\n",pRoot->_Mat._Scale._X,pRoot->_Mat._Scale._Y,pRoot->_Mat._Scale._Z);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// ChildCount
	int nChild = (int)pRoot->_pChildList->size();
	sprintf(sz,"%d\n",nChild);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// Child
	std::list<_SKELETON*>::iterator itr = pRoot->_pChildList->begin();
	for(;itr!=pRoot->_pChildList->end();++itr)
		CopySkeletonInfoToFile((*itr));
	// Node_End
	sprintf(sz,"Node_End\n");
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
}

void maxProject1::CopyAnimeInfoToFile(_SKELETON *pRoot)
{
	char sz[1024]={0};
	// Node_Start
	sprintf(sz,"Node_Start\n");
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// ID
	sprintf(sz,"%d\n",pRoot->_ID);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// SampleKeys
	for(int i=0;i<pRoot->_KeyCount;i++)
	{
		// Matrix
		sprintf(sz,"%f %f %f\n",pRoot->_pKeys[i]._Mat._Translation._X,pRoot->_pKeys[i]._Mat._Translation._Y,pRoot->_pKeys[i]._Mat._Translation._Z);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		//sprintf(sz,"%f %f %f\n",pRoot->_pKeys[i]._Mat._RotationAxis._X,pRoot->_pKeys[i]._Mat._RotationAxis._Y,pRoot->_pKeys[i]._Mat._RotationAxis._Z);
		//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		//sprintf(sz,"%f\n",pRoot->_pKeys[i]._Mat._Angle);
		//fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		sprintf(sz,"%f %f %f %f\n",pRoot->_pKeys[i]._Mat._Quaternion.x,pRoot->_pKeys[i]._Mat._Quaternion.y,
			pRoot->_pKeys[i]._Mat._Quaternion.z,pRoot->_pKeys[i]._Mat._Quaternion.w);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
		sprintf(sz,"%f %f %f\n",pRoot->_pKeys[i]._Mat._Scale._X,pRoot->_pKeys[i]._Mat._Scale._Y,pRoot->_pKeys[i]._Mat._Scale._Z);
		fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	}
	// ChildCount
	int nChild = (int)pRoot->_pChildList->size();
	sprintf(sz,"%d\n",nChild);
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
	// Child
	std::list<_SKELETON*>::iterator itr = pRoot->_pChildList->begin();
	for(;itr!=pRoot->_pChildList->end();++itr)
		CopyAnimeInfoToFile((*itr));
	// Node_End
	sprintf(sz,"Node_End\n");
	fwrite(sz,sizeof(char),strlen(sz),m_saveFile);
}

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Contains the Dll Entry stuff
// AUTHOR: 
//***************************************************************************/
extern ClassDesc2* GetmaxProject1Desc();

HINSTANCE hInstance;
int controlsInit = FALSE;

// This function is called by Windows when the DLL is loaded.  This 
// function may also be called many times during time critical operations
// like rendering.  Therefore developers need to be careful what they
// do inside this function.  In the code below, note how after the DLL is
// loaded the first time only a few statements are executed.

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID /*lpvReserved*/)
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		// Hang on to this DLL's instance handle.
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hInstance);
	}
	return(TRUE);
}

// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
__declspec( dllexport ) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESCRIPTION);
}

// This function returns the number of plug-in classes this DLL
//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

// This function returns the number of plug-in classes this DLL
__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) {
		case 0: return GetmaxProject1Desc();
		default: return 0;
	}
}

// This function returns a pre-defined constant indicating the version of 
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

// This function is called once, right after your plugin has been loaded by 3ds Max. 
// Perform one-time plugin initialization in this method.
// Return TRUE if you deem your plugin successfully loaded, or FALSE otherwise. If 
// the function returns FALSE, the system will NOT load the plugin, it will then call FreeLibrary
// on your DLL, and send you a message.
__declspec( dllexport ) int LibInitialize(void)
{
	return TRUE; // TODO: Perform initialization here.
}

// This function is called once, just before the plugin is unloaded. 
// Perform one-time plugin un-initialization in this method."
// The system doesn't pay attention to a return value.
__declspec( dllexport ) int LibShutdown(void)
{
	return TRUE;// TODO: Perform un-initialization here.	
}

TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

