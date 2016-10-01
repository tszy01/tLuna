#ifndef _TLGUITEXTUREMGR_H_
#define _TLGUITEXTUREMGR_H_

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TLSingleton.h"
#include <map>

namespace TLunaEngine{
	class RenderDeviceUsedBuffer;
	class RenderDeviceUsedSRV;
	class RenderDeviceUsedVS;
	class RenderDeviceUsedPS;
	class RenderDeviceUsedGS;
	class RenderDeviceUsedDepthStencilState;
	class RenderDeviceUsedBlendState;
	class RenderDeviceUsedSamplerState;
	class RenderDeviceUsedInputLayout;
	/*
	 *	负责GUI的渲染，统一加载资源
	 */
	class GUITextureMgr : public Singleton<GUITextureMgr>
	{
		friend class Singleton<GUITextureMgr>;
	protected:
		GUITextureMgr(TVOID);
		~GUITextureMgr(TVOID);
	private:
		TU32 m_bufferWidth;	// 后缓冲宽
		TU32 m_bufferHeight;// 后缓冲高
		RenderDeviceUsedBuffer* mVB;
		RenderDeviceUsedBuffer* mVBSet;
		RenderDeviceUsedVS* mVS;
		RenderDeviceUsedPS* mPS;
		RenderDeviceUsedDepthStencilState* mDepthStencilState;
		RenderDeviceUsedBlendState* mBlendState;
		RenderDeviceUsedSamplerState* mSamplerState;
		RenderDeviceUsedInputLayout* mInputLayout;
		std::map<TS32,RenderDeviceUsedSRV*> mSRVList;
	public:
		// 初始化
		TBOOL Init(TU32 bufferWidth,TU32 bufferHeight,const TCHAR* effectFile);
		// 加载纹理
		TBOOL LoadTexFromFile(const TCHAR* file);
		// 销毁
		TVOID DestroyAllTex();
		// 渲染
		// 这里绘画区域传相对于屏幕的，纹理坐标传真正的纹理映射值（0-1）
		TBOOL DrawGUICtrl(TS32 x,TS32 y,TS32 width,TS32 height,TF32 texX,TF32 texY,TF32 texR,TF32 texB,TS32 texId,TF32 alpha);
	private:
		// 其它D3D资源
		TBOOL InitD3DObj(const TCHAR* effectFile);
		// 读取纹理ID，文件名
		TBOOL LoadTexArray(FILE* stream);
	};

}

#endif