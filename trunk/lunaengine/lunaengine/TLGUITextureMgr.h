#pragma once

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TSingleton.h"
#include <map>
using namespace TLunaEngine;
using namespace std;

namespace TLunaEngine{
	class TLIRenderDeviceUsedBuffer;
	class TLIRenderDeviceUsedSRV;
	class TLIRenderDeviceUsedVS;
	class TLIRenderDeviceUsedPS;
	class TLIRenderDeviceUsedGS;
	class TLIRenderDeviceUsedDepthStencilState;
	class TLIRenderDeviceUsedBlendState;
	class TLIRenderDeviceUsedSamplerState;
	class TLIRenderDeviceUsedInputLayout;
	/*
	 *	负责GUI的渲染，统一加载资源
	 */
	class TLGUITextureMgr
	{
	T_SINGLETON_DEF(TLGUITextureMgr);
	public:
		TLGUITextureMgr(void);
		~TLGUITextureMgr(void);
	private:
		UINT m_bufferWidth;	// 后缓冲宽
		UINT m_bufferHeight;// 后缓冲高
		TLIRenderDeviceUsedBuffer* mVB;
		TLIRenderDeviceUsedBuffer* mVBSet;
		TLIRenderDeviceUsedVS* mVS;
		TLIRenderDeviceUsedPS* mPS;
		TLIRenderDeviceUsedDepthStencilState* mDepthStencilState;
		TLIRenderDeviceUsedBlendState* mBlendState;
		TLIRenderDeviceUsedSamplerState* mSamplerState;
		TLIRenderDeviceUsedInputLayout* mInputLayout;
		std::map<int,TLIRenderDeviceUsedSRV*> mSRVList;
	public:
		// 初始化
		BOOL Init(UINT bufferWidth,UINT bufferHeight,const char* effectFile);
		// 加载纹理
		BOOL LoadTexFromFile(const char* file);
		// 销毁
		void DestroyAllTex();
		// 渲染
		// 这里绘画区域传相对于屏幕的，纹理坐标传真正的纹理映射值（0-1）
		bool DrawGUICtrl(LONG x,LONG y,LONG width,LONG height,float texX,float texY,float texR,float texB,int texId,float alpha);
	private:
		// 其它D3D资源
		BOOL InitD3DObj(const char* effectFile);
		// 读取纹理ID，文件名
		BOOL LoadTexArray(FILE* stream);
	};

}