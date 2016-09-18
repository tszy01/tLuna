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
		GUITextureMgr(void);
		~GUITextureMgr(void);
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
		std::map<int,RenderDeviceUsedSRV*> mSRVList;
	public:
		// 初始化
		TBOOL Init(TU32 bufferWidth,TU32 bufferHeight,const char* effectFile);
		// 加载纹理
		TBOOL LoadTexFromFile(const char* file);
		// 销毁
		void DestroyAllTex();
		// 渲染
		// 这里绘画区域传相对于屏幕的，纹理坐标传真正的纹理映射值（0-1）
		bool DrawGUICtrl(TS32 x,TS32 y,TS32 width,TS32 height,float texX,float texY,float texR,float texB,int texId,float alpha);
	private:
		// 其它D3D资源
		TBOOL InitD3DObj(const char* effectFile);
		// 读取纹理ID，文件名
		TBOOL LoadTexArray(FILE* stream);
	};

}

#endif