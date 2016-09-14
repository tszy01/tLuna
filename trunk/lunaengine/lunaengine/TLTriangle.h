#pragma once

#include "TVector3.h"
#include "TVector2.h"
#include "TVector4.h"
#include "TMatrix4x4.h"

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

	struct SimpleVertex
	{
		TVector3<float> Pos;
		TVector2<float> Tex;
	};

	struct CBNeverChanges
	{
		TMatrix4x4<float> mView;
	};

	struct CBChangeOnResize
	{
		TMatrix4x4<float> mProjection;
	};

	struct CBChangesEveryFrame
	{
		TMatrix4x4<float> mWorld;
		TMatrix4x4<float> transMat;
	};

	class TLTriangle
	{
	public:
		TLTriangle(void);
		~TLTriangle(void);
	private:
		TLIRenderDeviceUsedBuffer* mVB;
		TLIRenderDeviceUsedBuffer* mRB;
		TLIRenderDeviceUsedBuffer* mIB;
		TLIRenderDeviceUsedBuffer* mCB[3];
		TLIRenderDeviceUsedSRV* mSRV;
		TLIRenderDeviceUsedVS* mVS;
		TLIRenderDeviceUsedVS* mVSAdd;
		TLIRenderDeviceUsedPS* mPS;
		TLIRenderDeviceUsedGS* mGS;
		TLIRenderDeviceUsedSamplerState* mSamplerState;
		TLIRenderDeviceUsedDepthStencilState* mDepthStencilState;
		TLIRenderDeviceUsedBlendState* mBlendState;
		TLIRenderDeviceUsedInputLayout* mInputLayout;
		TLIRenderDeviceUsedInputLayout* mInputLayout2;
	public:
		// ≥ı ºªØ
		BOOL InitTriangle();
		// ªÊª≠
		void OnRender();
	public:
		float ta;
	};

}