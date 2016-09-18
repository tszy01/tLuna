#pragma once

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
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

#include "3dsmaxsdk_preinclude.h"
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
//SIMPLE TYPE
#include <list>
#include <map>
#include <vector>
using namespace std;


extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

// 向量
typedef struct _VECTOR3
{
	// X
	float _X;
	// Y
	float _Y;
	// Z
	float _Z;

	_VECTOR3()
	{
		_X = 0.0f;
		_Y = 0.0f;
		_Z = 0.0f;
	}

	_VECTOR3(float x,float y,float z)
	{
		_X = x;
		_Y = y;
		_Z = z;
	}

	void Normalize()
	{
		float sum = _X * _X + _Y *_Y + _Z * _Z;
		if(sum == 0.0f)
			return;
		float sqsum = sqrt(sum);
		_X = _X / sqsum;
		_Y = _Y / sqsum;
		_Z = _Z / sqsum;
	}

	void operator=(_VECTOR3& right)
	{
		_X = right._X;
		_Y = right._Y;
		_Z = right._Z;
	}

	_VECTOR3 operator-(_VECTOR3& right)
	{
		float x = _X - right._X;
		float y = _Y - right._Y;
		float z = _Z - right._Z;
		return _VECTOR3(x,y,z);
	}

	void operator-=(_VECTOR3& right)
	{
		_X -= right._X;
		_Y -= right._Y;
		_Z -= right._Z;
	}

	_VECTOR3 operator+(_VECTOR3& right)
	{
		float x = _X + right._X;
		float y = _Y + right._Y;
		float z = _Z + right._Z;
		return _VECTOR3(x,y,z);
	}

	void operator+=(_VECTOR3& right)
	{
		_X += right._X;
		_Y += right._Y;
		_Z += right._Z;
	}

	float operator*(_VECTOR3& right)
	{
		return _X*right._X + _Y*right._Y + _Z*right._Z;
	}

	_VECTOR3 operator*(float scale)
	{
		return _VECTOR3(_X*scale,_Y*scale,_Z*scale);
	}

	bool operator==(_VECTOR3& right)
	{
		if(abs(_X-right._X)<=0.001f && abs(_Y-right._Y)<=0.001f 
			&& abs(_Z-right._Z)<=0.001f)
			return true;
		return false;
	}

	void ChangeYZ()
	{
		float tmp = _Z;
		_Z = _Y;
		_Y = tmp;
	}
}VECTOR3;

// 临时矩阵类型
typedef struct _MATRIX
{
	// 移动
	VECTOR3 _Translation;
	// 旋转
	//VECTOR3 _RotationAxis;
	//float _Angle;
	Quat _Quaternion;
	// 缩放
	VECTOR3 _Scale;

	_MATRIX()
	{
		_Translation = VECTOR3();
		//_RotationAxis = VECTOR3();
		//_Angle = 0.0f;
		_Quaternion = Quat();
		_Scale = VECTOR3();
	}

	void operator=(_MATRIX& right)
	{
		_Translation = right._Translation;
		//_RotationAxis = right._RotationAxis;
		//_Angle = right._Angle;
		_Quaternion = right._Quaternion;
		_Scale = right._Scale;
	}

	void ChangeYZ()
	{
		_Translation.ChangeYZ();
		//_RotationAxis.ChangeYZ();
		float tmp = _Quaternion.y;
		_Quaternion.y = _Quaternion.z;
		_Quaternion.z = tmp;
		_Scale.ChangeYZ();
	}
}MATRIX;

// 矩阵
typedef struct _MATRIX16
{
	// 4x4
	float _value[4][4];

	_MATRIX16()
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<0;j++)
				_value[i][j] = 0.0f;
	}

	void Identity()
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<0;j++)
				_value[i][j] = 0.0f;
		for(int i=0;i<4;i++)
			_value[i][i] = 1.0f;
	}

	static _MATRIX16 FromScale(float x,float y,float z)
	{
		_MATRIX16 mat;
		mat.Identity();
		mat._value[0][0] = x;
		mat._value[1][1] = y;
		mat._value[2][2] = z;
		return mat;
	}

	static _MATRIX16 FromTranslation(float x,float y,float z)
	{
		_MATRIX16 mat;
		mat.Identity();
		mat._value[3][0] = x;
		mat._value[3][1] = y;
		mat._value[3][2] = z;
		return mat;
	}

	static _MATRIX16 FromAngle(float axisX,float axisY,float axisZ,float angle)
	{
		_MATRIX16 mat;
		mat.Identity();

		float cost = cos(angle), sint = sin(angle), one_sub_cost = 1 - cost;
		_VECTOR3 vec;
		vec._X = axisX;
		vec._Y = axisY;
		vec._Z = axisZ;
		vec.Normalize();

		/**mat = D3DXMATRIXA16(v.x * v.x * one_sub_cost + cost, v.x * v.y * one_sub_cost + v.z * sint, v.x * v.z * one_sub_cost - v.y * sint, 0, \
							v.x * v.y * one_sub_cost - v.z * sint, v.y * v.y * one_sub_cost + cost, v.y * v.z * one_sub_cost + v.x * sint, 0, \
							v.x * v.z * one_sub_cost + v.y * sint, v.y * v.z * one_sub_cost - v.x * sint, v.z * v.z * one_sub_cost + cost, 0, \
							0, 0, 0, 1);*/

		mat._value[0][0] = vec._X * vec._X * one_sub_cost + cost;
		mat._value[0][1] = vec._X * vec._Y * one_sub_cost + vec._Z * sint;
		mat._value[0][2] = vec._X * vec._Z * one_sub_cost - vec._Y * sint;
		mat._value[0][3] = 0;

		mat._value[1][0] = vec._X * vec._Y * one_sub_cost - vec._Z * sint;
		mat._value[1][1] = vec._Y * vec._Y * one_sub_cost + cost;
		mat._value[1][2] = vec._Y * vec._Z * one_sub_cost + vec._X * sint;
		mat._value[1][3] = 0;

		mat._value[2][0] = vec._X * vec._Z * one_sub_cost + vec._Y * sint;
		mat._value[2][1] = vec._Y * vec._Z * one_sub_cost - vec._X * sint;
		mat._value[2][2] = vec._Z * vec._Z * one_sub_cost + cost;
		mat._value[2][3] = 0;

		mat._value[3][0] = 0;
		mat._value[3][1] = 0;
		mat._value[3][2] = 0;
		mat._value[3][3] = 1;
		return mat;
	}

	_MATRIX16 operator*(_MATRIX16 right)
	{
		_MATRIX16 mat;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				float tmp = 0;
				for(int k=0;k<4;k++)
					tmp += _value[i][k] * right._value[k][j];
				mat._value[i][j] = tmp;
			}
		}
		return mat;
	}

	void operator=(_MATRIX16& right)
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<0;j++)
				_value[i][j] = right._value[i][j];
	}
}MATRIX16;

// 纹理结构体
typedef struct _TEXTURE
{
	// 文件名
	char _szFile[256];
	// 纹理动画
	VECTOR3* _UVOff;
	// 动画帧数量
	int _keyCount;

	_TEXTURE()
	{
		memset(_szFile,0,256);
		_UVOff = 0;
		_keyCount = 0;
	}

	~_TEXTURE()
	{
		if(_UVOff)
		{
			delete [] _UVOff;
			_UVOff = 0;
		}
		_keyCount = 0;
	}
}TEXTURE;

// 纹理映射结构体
typedef struct _TEXTUREMAP
{
	// 数量
	int _Count;
	// 纹理数组
	TEXTURE* _pTexs;

	_TEXTUREMAP()
	{
		_Count = 0;
		_pTexs = NULL;
	}

	void Release()
	{
		if(_pTexs)
		{
			delete [] _pTexs;
			_pTexs = NULL;
		}
	}
}TEXTUREMAP;

// 材质的结构体
typedef struct _MATERIAL
{
	// 名字
	char _szName[256];
	// Diffuse
	float _Diffuse[3];
	// Ambient
	float _Ambient[3];
	// Specular
	float _Specular[3];
	// TextureMap
	TEXTUREMAP _TexMap;

	_MATERIAL()
	{
		memset(_szName,0,256);
		for(int i=0;i<3;i++)
		{
			_Diffuse[i] = 0.0f;
			_Ambient[i] = 0.0f;
			_Specular[i] = 0.0f;
		}
	}

	~_MATERIAL()
	{
		_TexMap.Release();
	}
}MATERIAL;

typedef std::list<MATERIAL*> MatList;

// 蒙皮点信息
typedef struct _SKINVERTEX
{
	// NumOfBones
	int _BoneCount;
	// BoneID
	int* _pBoneIDs;
	// Weight
	float* _pWeights;

	_SKINVERTEX()
	{
		_BoneCount = 0;
		_pBoneIDs = NULL;
		_pWeights = NULL;
	}

	~_SKINVERTEX()
	{
		Clear();
	}

	void Clear()
	{
		_BoneCount = 0;
		if(_pBoneIDs)
		{
			delete [] _pBoneIDs;
			_pBoneIDs = NULL;
		}
		if(_pWeights)
		{
			delete [] _pWeights;
			_pWeights = NULL;
		}
	}

	void operator=(_SKINVERTEX& right)
	{
		if(right._BoneCount > 0)
		{
			// 先删除原来的
			Clear();
			_BoneCount = right._BoneCount;
			_pBoneIDs = new int[_BoneCount];
			_pWeights = new float[_BoneCount];
			memcpy(_pBoneIDs,right._pBoneIDs,sizeof(int)*_BoneCount);
			memcpy(_pWeights,right._pWeights,sizeof(float)*_BoneCount);
		}
	}
}SKINVERTEX;

// 面
typedef struct _FACE
{
	// 顶点位置索引
	int _PositionIndex[3];
	// 法线索引
	int _NormalIndex[3];
	// 纹理映射点索引
	int _TexVertexIndex[3];
	// 对应材质名称
	char _szMatName[256];
	// 最终的点索引
	// 这一项在加载时不用
	int _FinalIndex[3];

	_FACE()
	{
		for(int i=0;i<3;i++)
		{
			_PositionIndex[i] = -1;
			_NormalIndex[i] = -1;
			_TexVertexIndex[i] = -1;
			_FinalIndex[i] = -1;
		}
		memset(_szMatName,0,256);
	}
	
	~_FACE()
	{
		for(int i=0;i<3;i++)
		{
			_PositionIndex[i] = -1;
			_NormalIndex[i] = -1;
			_TexVertexIndex[i] = -1;
			_FinalIndex[i] = -1;
		}
		memset(_szMatName,0,256);
	}
}FACE;

// 临时顶点结构
typedef struct _VERTEXDESC
{
	// 位置
	VECTOR3 _Position;
	// 法线
	VECTOR3 _Normal;
	// 纹理
	VECTOR3 _Tex;
	// Tangent
	VECTOR3 _Tan;
	// 权重
	SKINVERTEX _Skin;

	_VERTEXDESC()
	{
		_Position = VECTOR3(0,0,0);
		_Normal = VECTOR3(0,0,0);
		_Tex = VECTOR3(0,0,0);
		_Tan = VECTOR3(0,0,0);
		_Skin = SKINVERTEX();
	}

	void Reset()
	{
		_Position = VECTOR3(0,0,0);
		_Normal = VECTOR3(0,0,0);
		_Tex = VECTOR3(0,0,0);
		_Tan = VECTOR3(0,0,0);
		_Skin = SKINVERTEX();
	}

	void operator=(_VERTEXDESC& right)
	{
		_Position = right._Position;
		_Normal = right._Normal;
		_Tex = right._Tex;
		_Skin = right._Skin;
		_Tan = right._Tan;
	}
}VERTEXDESC;

typedef std::map<int,VERTEXDESC*> VertexDescMap;

// Mesh
typedef struct _MESH
{
	// Matrix
	MATRIX _Mat;
	// 顶点位置数量
	int _PositionCount;
	// 顶点
	VECTOR3* _pPositions;
	// 顶点动画用位置
	VECTOR3** _pVAPositions;
	// 关键帧数量
	int _KeyCount;
	// 权重
	SKINVERTEX* _pSkinVertices;
	// 法线数量
	int _NormalCount;
	// 法线
	VECTOR3* _pNormals;
	// 顶点动画用法线
	VECTOR3** _pVANormals;
	// 映射点数量
	int _TexVertexCount;
	// 映射点
	VECTOR3* _pTexVertices;
	// 顶点动画用纹理偏移
	VECTOR3** _pVATUVOff;
	// 面数量
	int _FaceCount;
	// 面
	FACE* _pFaces;
	// 骨骼的数量
	int _BoneCount;
	// 骨骼的初始矩阵
	MATRIX* _pBoneMatrices;
	// 骨骼的初始矩阵对应的ID
	int* _pBoneIDs;
	// 蒙皮的初始矩阵
	MATRIX _SkinMatrix;
	// 是否为骨骼动画
	bool _bSkin;

	_MESH()
	{
		_Mat = MATRIX();
		_PositionCount = 0;
		_FaceCount = 0;
		_KeyCount = 0;
		_NormalCount = 0;
		_TexVertexCount = 0;
		_pPositions = NULL;
		_pVANormals = NULL;
		_pVAPositions = NULL;
		_pVATUVOff = NULL;
		_pFaces = NULL;
		_pNormals = NULL;
		_pTexVertices = NULL;
		_pSkinVertices = NULL;
		_BoneCount = 0;
		_pBoneMatrices = NULL;
		_pBoneIDs = NULL;
		_SkinMatrix = MATRIX();
		_bSkin = false;
	}

	~_MESH()
	{
		for(int i=0;i<_KeyCount;i++)
		{
			VECTOR3* pV = _pVANormals[i];
			if(pV)
			{
				delete [] pV;
				_pVANormals[i] = NULL;
			}
			pV = _pVAPositions[i];
			if(pV)
			{
				delete [] pV;
				_pVAPositions[i] = NULL;
			}
			pV = _pVATUVOff[i];
			if(pV)
			{
				delete [] pV;
				_pVATUVOff[i] = NULL;
			}
		}
		if (_pVANormals)
		{
			delete [] _pVANormals;
			_pVANormals = NULL;
		}
		if (_pVAPositions)
		{
			delete [] _pVAPositions;
			_pVAPositions = NULL;
		}
		if(_pVATUVOff)
		{
			delete [] _pVATUVOff;
			_pVATUVOff = NULL;
		}
		if(_pPositions)
		{
			delete [] _pPositions;
			_pPositions = NULL;
		}
		if(_pFaces)
		{
			delete [] _pFaces;
			_pFaces = NULL;
		}
		if(_pNormals)
		{
			delete [] _pNormals;
			_pNormals = NULL;
		}
		if(_pTexVertices)
		{
			delete [] _pTexVertices;
			_pTexVertices = NULL;
		}
		if(_pSkinVertices)
		{
			delete [] _pSkinVertices;
			_pSkinVertices = NULL;
		}
		if(_pBoneMatrices)
		{
			delete [] _pBoneMatrices;
			_pBoneMatrices = NULL;
		}
		if(_pBoneIDs)
		{
			delete [] _pBoneIDs;
			_pBoneIDs = NULL;
		}
		_PositionCount = 0;
		_FaceCount = 0;
		_NormalCount = 0;
		_TexVertexCount = 0;
		_BoneCount = 0;
		_bSkin = false;
	}
}MESH;

typedef std::list<MESH*> MeshList;

// 帧信息
typedef struct _SAMPLEKEY
{
	// Time
	int _Time;
	// Matrix
	MATRIX _Mat;

	_SAMPLEKEY()
	{
		_Time = 0;
		_Mat = MATRIX();
	}

	void operator=(_SAMPLEKEY& right)
	{
		_Time = right._Time;
		_Mat = right._Mat;
	}
}SAMPLEKEY;

// Node信息
typedef struct _SKELETON
{
	// 名字
	char _szName[256];
	// ID
	int _ID;
	// Parent
	_SKELETON* _pParent;
	// Child
	std::list<_SKELETON*>* _pChildList;
	// Matrix
	MATRIX _Mat;
	// KeyCount
	int _KeyCount;
	// KeyArray
	SAMPLEKEY* _pKeys;

	_SKELETON()
	{
		memset(_szName,0,256);
		_ID = -1;
		_pParent = NULL;
		_pChildList = NULL;
		_Mat = MATRIX();
		_KeyCount = 0;
		_pKeys = NULL;
	}

	~_SKELETON()
	{
		memset(_szName,0,256);
		_ID = -1;
		_pParent = NULL;
		if(_pChildList)
		{
			std::list<_SKELETON*>::iterator itr = _pChildList->begin();
			for(;itr!=_pChildList->end();++itr)
			{
				SKELETON *p = *itr;
				delete p;
				p = NULL;
			}
			_pChildList->clear();
			delete _pChildList;
			_pChildList = NULL;
		}
		_KeyCount = 0;
		if(_pKeys)
		{
			delete [] _pKeys;
			_pKeys = NULL;
		}
	}
}SKELETON;

typedef std::list<_SKELETON*> SkeletonList;

class VNormal 
{ 
   public: 
     Point3 norm; 
     DWORD smooth; 
     VNormal *next; 
     BOOL init; 
     VNormal() {smooth=0;next=NULL;init=FALSE;norm=Point3(0,0,0);} 
     VNormal(Point3 &n,DWORD s) {next=NULL;init=TRUE;norm=n;smooth=s;} 
     ~VNormal() {delete next;} 
     void AddNormal(Point3 &n,DWORD s); 
     Point3 &GetNormal(DWORD s); 
     void Normalize(); 
};
