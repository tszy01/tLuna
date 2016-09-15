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
		Vector3<float> Pos;
		Vector2<float> Tex;
	};

	struct CBNeverChanges
	{
		Matrix4x4<float> mView;
	};

	struct CBChangeOnResize
	{
		Matrix4x4<float> mProjection;
	};

	struct CBChangesEveryFrame
	{
		Matrix4x4<float> mWorld;
		Matrix4x4<float> transMat;
	};

	class TestTriangle
	{
	public:
		TestTriangle(void);
		~TestTriangle(void);
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
		// ≥ı ºªØ
		TBOOL InitTriangle();
		// ªÊª≠
		void OnRender();
	public:
		float ta;
	};

}

#endif