#ifndef _TLGUITEXTUREMGR_H_
#define _TLGUITEXTUREMGR_H_

#include "TLGUIFont.h"
#include "TLGUIDefine.h"
#include "TSSingleton.h"
#include "TSMap.h"

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
	class GUITextureMgr : public TSun::Singleton<GUITextureMgr>
	{
		friend class TSun::Singleton<GUITextureMgr>;
	protected:
		GUITextureMgr(TSun::TVOID);
		~GUITextureMgr(TSun::TVOID);
	private:
		TSun::TU32 m_bufferWidth;	// 后缓冲宽
		TSun::TU32 m_bufferHeight;// 后缓冲高
		RenderDeviceUsedBuffer* mVB;
		RenderDeviceUsedBuffer* mVBSet;
		RenderDeviceUsedVS* mVS;
		RenderDeviceUsedPS* mPS;
		RenderDeviceUsedDepthStencilState* mDepthStencilState;
		RenderDeviceUsedBlendState* mBlendState;
		RenderDeviceUsedSamplerState* mSamplerState;
		RenderDeviceUsedInputLayout* mInputLayout;
		TSun::Map<TSun::TS32,RenderDeviceUsedSRV*> mSRVList;
	public:
		// 初始化
		TSun::TBOOL Init(TSun::TU32 bufferWidth,TSun::TU32 bufferHeight,const TSun::TCHAR* effectFile);
		// 加载纹理
		TSun::TBOOL LoadTexFromFile(const TSun::TCHAR* file);
		// 销毁
		TSun::TVOID DestroyAllTex();
		// 渲染
		// 这里绘画区域传相对于屏幕的，纹理坐标传真正的纹理映射值（0-1）
		TSun::TBOOL DrawGUICtrl(TSun::TS32 x,TSun::TS32 y,TSun::TS32 width,TSun::TS32 height,TSun::TF32 texX,TSun::TF32 texY,TSun::TF32 texR,TSun::TF32 texB,TSun::TS32 texId,TSun::TF32 alpha);
	private:
		// 其它D3D资源
		TSun::TBOOL InitD3DObj(const TSun::TCHAR* effectFile);
		// 读取纹理ID，文件名
		TSun::TBOOL LoadTexArray(FILE* stream);
	};

}

#endif