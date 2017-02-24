#ifndef _TLTESTTRIANGLE_H_
#define _TLTESTTRIANGLE_H_

#include "TSVector3.h"
#include "TSVector2.h"
#include "TSVector4.h"
#include "TSMatrix4x4.h"

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

	struct SimpleVertex
	{
		TSun::Vector3<TSun::TF32> Pos;
		TSun::Vector2<TSun::TF32> Tex;
	};

	struct CBNeverChanges
	{
		TSun::Matrix4x4<TSun::TF32> mView;
	};

	struct CBChangeOnResize
	{
		TSun::Matrix4x4<TSun::TF32> mProjection;
	};

	struct CBChangesEveryFrame
	{
		TSun::Matrix4x4<TSun::TF32> mWorld;
		TSun::Matrix4x4<TSun::TF32> transMat;
	};

	class TestTriangle
	{
	public:
		TestTriangle(TSun::TVOID);
		~TestTriangle(TSun::TVOID);
	private:
		RenderDeviceUsedBuffer* mVB;
		RenderDeviceUsedBuffer* mRB;
		RenderDeviceUsedBuffer* mIB;
		RenderDeviceUsedBuffer* mCB[3];
		RenderDeviceUsedSRV* mSRV;
		RenderDeviceUsedVS* mVS;
		RenderDeviceUsedVS* mVSAdd;
		RenderDeviceUsedPS* mPS;
		RenderDeviceUsedGS* mGS;
		RenderDeviceUsedSamplerState* mSamplerState;
		RenderDeviceUsedDepthStencilState* mDepthStencilState;
		RenderDeviceUsedBlendState* mBlendState;
		RenderDeviceUsedInputLayout* mInputLayout;
		RenderDeviceUsedInputLayout* mInputLayout2;
	public:
		// 初始化
		TSun::TBOOL InitTriangle();
		// 绘画
		TSun::TVOID OnRender();
	public:
		TSun::TF32 ta;
	};

}

#endif