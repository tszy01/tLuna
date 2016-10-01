#ifndef _TLTESTTRIANGLE_H_
#define _TLTESTTRIANGLE_H_

#include "TLVector3.h"
#include "TLVector2.h"
#include "TLVector4.h"
#include "TLMatrix4x4.h"

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
		Vector3<TF32> Pos;
		Vector2<TF32> Tex;
	};

	struct CBNeverChanges
	{
		Matrix4x4<TF32> mView;
	};

	struct CBChangeOnResize
	{
		Matrix4x4<TF32> mProjection;
	};

	struct CBChangesEveryFrame
	{
		Matrix4x4<TF32> mWorld;
		Matrix4x4<TF32> transMat;
	};

	class TestTriangle
	{
	public:
		TestTriangle(TVOID);
		~TestTriangle(TVOID);
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
		TBOOL InitTriangle();
		// 绘画
		TVOID OnRender();
	public:
		TF32 ta;
	};

}

#endif