#include "TLTestTriangle.h"
#include "TSString.h"
#include "TSMatrix4x4.h"
#include "TLGlobleClass.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice.h"
#include "TLRenderDeviceUsedBlendState.h"
#include "TLRenderDeviceUsedBuffer.h"
#include "TLRenderDeviceUsedTex2D.h"
#include "TLRenderDeviceUsedSRV.h"
#include "TLRenderDeviceUsedVS.h"
#include "TLRenderDeviceUsedPS.h"
#include "TLRenderDeviceUsedGS.h"
#include "TLRenderDeviceUsedDepthStencilState.h"
#include "TLRenderDeviceUsedSamplerState.h"
#include "TLRenderDeviceUsedInputLayout.h"
#include "TLRenderDeviceCompiledShader.h"
#include "TLImage.h"
#include "TLMemDef.h"

namespace TLunaEngine{

	TestTriangle::TestTriangle(TSun::TVOID) : ta(1000.0f),mVB(TSun::TNULL),mIB(TSun::TNULL),mRB(TSun::TNULL),mSRV(TSun::TNULL),
		mVSAdd(TSun::TNULL),mGS(TSun::TNULL),mVS(TSun::TNULL),mPS(TSun::TNULL),mInputLayout(TSun::TNULL),mSamplerState(TSun::TNULL),
		mInputLayout2(TSun::TNULL),mDepthStencilState(TSun::TNULL),mBlendState(TSun::TNULL)
	{
		mCB[0] = mCB[1] = mCB[2] = TSun::TNULL;
	}

	TestTriangle::~TestTriangle(TSun::TVOID)
	{
		if (mVB)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mVB);
			mVB = 0;
		}
		if (mIB)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mIB);
			mIB = 0;
		}
		if (mRB)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mRB);
			mRB = 0;
		}
		if (mCB[0])
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mCB[0]);
			mCB[0] = 0;
		}
		if (mCB[1])
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mCB[1]);
			mCB[1] = 0;
		}
		if (mCB[2])
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mCB[2]);
			mCB[2] = 0;
		}
		if (mSRV)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSRV, mSRV);
			mSRV = 0;
		}
		if (mVS)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedVS, mVS);
			mVS = 0;
		}
		if (mPS)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedPS, mPS);
			mPS = 0;
		}
		if (mVSAdd)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedVS, mVSAdd);
			mVSAdd = 0;
		}
		if (mGS)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedGS, mGS);
			mGS = 0;
		}
		if (mInputLayout)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedInputLayout, mInputLayout);
			mInputLayout = 0;
		}
		if (mInputLayout2)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedInputLayout, mInputLayout2);
			mInputLayout2 = 0;
		}
		if (mSamplerState)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSamplerState, mSamplerState);
			mSamplerState = 0;
		}
		if (mDepthStencilState)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedDepthStencilState, mDepthStencilState);
			mDepthStencilState = 0;
		}
		if (mBlendState)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBlendState, mBlendState);
			mBlendState = 0;
		}
	}

	TSun::TBOOL TestTriangle::InitTriangle()
	{
		// Init D3DObj
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// Init Shader
		TSun::String strShader = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\Triangle0.fx";
		// VS
		RenderDeviceCompiledShader* pCompiledVS = pDevice->createCompiledShader();
		if(!pCompiledVS->compileShader(strShader.GetString(),"VS","vs"))
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVS);
			return TSun::TFALSE;
		}
		mVS = pDevice->createVertexShader(pCompiledVS);
		if(!mVS)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVS);
			return TSun::TFALSE;
		}
		// input layout
		TLRenderDeviceInputElementDesc renderLI[2];
		renderLI[0].AlignedByteOffset = 0;
		renderLI[0].Format = RENDER_DEVICE_FORMAT_R32G32B32_FLOAT;
		renderLI[0].SemanticIndex = 0;
		renderLI[0].SemanticName = TSun::String("POSITION");
		renderLI[0].InputSlot = 0;
		renderLI[1].AlignedByteOffset = 12;
		renderLI[1].Format = RENDER_DEVICE_FORMAT_R32G32_FLOAT;
		renderLI[1].SemanticIndex = 0;
		renderLI[1].SemanticName = TSun::String("TEXCOORD");
		renderLI[1].InputSlot = 0;
		mInputLayout = pDevice->createInputLayout(renderLI,2,pCompiledVS);
		if(!mInputLayout)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVS);
			return TSun::TFALSE;
		}
		T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVS);
		// PS
		RenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(strShader.GetString(),"PS","ps"))
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledPS);
			return TSun::TFALSE;
		}
		mPS = pDevice->createPixelShader(pCompiledPS);
		if(!mPS)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledPS);
			return TSun::TFALSE;
		}
		T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledPS);
		// VSAdd
		RenderDeviceCompiledShader* pCompiledVSAdd = pDevice->createCompiledShader();
		if(!pCompiledVSAdd->compileShader(strShader.GetString(),"AddVS","vs"))
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVSAdd);
			return TSun::TFALSE;
		}
		mVSAdd = pDevice->createVertexShader(pCompiledVSAdd);
		if(!mVSAdd)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVSAdd);
			return TSun::TFALSE;
		}
		// input layout
		mInputLayout2 = pDevice->createInputLayout(renderLI,2,pCompiledVSAdd);
		if(!mInputLayout2)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVSAdd);
			return TSun::TFALSE;
		}
		// GS
		TLRenderDeviceSODeclarationEntry entry[2];
		entry[0].ComponentCount = 3;
		entry[0].SemanticIndex = 0;
		entry[0].SemanticName = TSun::String("POSITION");
		entry[0].StartComponent = 0;
		entry[1].ComponentCount = 2;
		entry[1].SemanticIndex = 0;
		entry[1].SemanticName = TSun::String("TEXCOORD");
		entry[1].StartComponent = 0;
		mGS = pDevice->createGeometryShaderWithStreamOutput(pCompiledVSAdd,entry,2);
		if(!mGS)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVSAdd);
			return TSun::TFALSE;
		}
		T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVSAdd);
		// VB
		SimpleVertex vertices[] =
		{
			{ TSun::Vector3Float( -1.0f, 1.0f, -1.0f ), TSun::Vector2Float( 0.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, 1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, 1.0f, 1.0f ), TSun::Vector2Float( 1.0f, 1.0f ) },
			{ TSun::Vector3Float( -1.0f, 1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 1.0f ) },

			{ TSun::Vector3Float( -1.0f, -1.0f, -1.0f ), TSun::Vector2Float( 0.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, -1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, -1.0f, 1.0f ), TSun::Vector2Float( 1.0f, 1.0f ) },
			{ TSun::Vector3Float( -1.0f, -1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 1.0f ) },

			{ TSun::Vector3Float( -1.0f, -1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 0.0f ) },
			{ TSun::Vector3Float( -1.0f, -1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 0.0f ) },
			{ TSun::Vector3Float( -1.0f, 1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 1.0f ) },
			{ TSun::Vector3Float( -1.0f, 1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 1.0f ) },

			{ TSun::Vector3Float( 1.0f, -1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, -1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, 1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 1.0f ) },
			{ TSun::Vector3Float( 1.0f, 1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 1.0f ) },

			{ TSun::Vector3Float( -1.0f, -1.0f, -1.0f ), TSun::Vector2Float( 0.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, -1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, 1.0f, -1.0f ), TSun::Vector2Float( 1.0f, 1.0f ) },
			{ TSun::Vector3Float( -1.0f, 1.0f, -1.0f ), TSun::Vector2Float( 0.0f, 1.0f ) },

			{ TSun::Vector3Float( -1.0f, -1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, -1.0f, 1.0f ), TSun::Vector2Float( 1.0f, 0.0f ) },
			{ TSun::Vector3Float( 1.0f, 1.0f, 1.0f ), TSun::Vector2Float( 1.0f, 1.0f ) },
			{ TSun::Vector3Float( -1.0f, 1.0f, 1.0f ), TSun::Vector2Float( 0.0f, 1.0f ) },
		};
		TLRenderDeviceBufferDesc vbDesc;
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER;
		vbDesc.ByteWidth = sizeof(SimpleVertex) * 24;
		vbDesc.CPUAccessFlags = TSun::TFALSE;
		TLRenderDeviceSubresourceData initData;
		initData.pSysMem = vertices;
		mVB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mVB)
		{
			return TSun::TFALSE;
		}
		// RB
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER | RENDER_DEVICE_BIND_FLAG_STREAM_OUTPUT;
		mRB = pDevice->createBuffer(&vbDesc,TSun::TNULL);
		if(!mRB)
		{
			return TSun::TFALSE;
		}
		// IB
		TSun::TU32 indices[] =
		{
			3,1,0,
			2,1,3,

			6,4,5,
			7,4,6,

			11,9,8,
			10,9,11,

			14,12,13,
			15,12,14,

			19,17,16,
			18,17,19,

			22,20,21,
			23,20,22
		};
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_INDEX_BUFFER;
		vbDesc.ByteWidth = sizeof(TSun::TU32)*36;
		initData.pSysMem = indices;
		mIB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mIB)
		{
			return TSun::TFALSE;
		}
		// CB0
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBNeverChanges);
		mCB[0] = pDevice->createBuffer(&vbDesc,TSun::TNULL);
		if(!mCB[0])
		{
			return TSun::TFALSE;
		}
		// CB1
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBChangeOnResize);
		mCB[1] = pDevice->createBuffer(&vbDesc,TSun::TNULL);
		if(!mCB[1])
		{
			return TSun::TFALSE;
		}
		// CB2
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBChangesEveryFrame);
		mCB[2] = pDevice->createBuffer(&vbDesc,TSun::TNULL);
		if(!mCB[2])
		{
			return TSun::TFALSE;
		}
		// Shader Resource
		TSun::String strTex = GlobleClass::getSingletonPtr()->m_strResDir + "texture\\seafloor.dds";
		/*Image* image = Image::createFromFile(strTex.GetString());
		if(!image)
			return TSun::TFALSE;
		TSun::TU32 imgw,imgh;
		image->getImageSize(&imgw,&imgh);
		RENDER_DEVICE_FORMAT imgFormat = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
		if(image->getPixelFormat() == Image::PIXEL_FORMAT_R8)
		{
			imgFormat = RENDER_DEVICE_FORMAT_R8_UNORM;
		}
		TLRenderDeviceTex2DDesc texDesc;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = TSun::TFALSE;
		texDesc.Cube = TSun::TFALSE;
		texDesc.Format = imgFormat;
		texDesc.GenerateMips = TSun::TFALSE;
		texDesc.Height = imgh;
		texDesc.MipLevels = 1;
		texDesc.SampleCount = 1;
		texDesc.SampleQuality = 0;
		texDesc.Width = imgw;
		initData.pSysMem = image->getBuffer();
		initData.SysMemPitch = imgw*image->getPixelSize();
		RenderDeviceUsedTex2D* pTex = pDevice->createTex2D(&texDesc,&initData);
		if(!pTex)
		{
			delete image;
			return TSun::TFALSE;
		}
		TLRenderDeviceSRVDesc srvDesc;
		srvDesc.Format = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = RENDER_DEVICE_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Params[0] = 0;
		srvDesc.Params[1] = 1;
		mSRV = pDevice->createShaderResourceView(pTex,&srvDesc);
		if(!mSRV)
		{
			delete image;
			delete pTex;
			return TSun::TFALSE;
		}
		delete image;
		delete pTex;*/
		if(!pDevice->createTex2DFromDDSFile(strTex.GetString(),TSun::TNULL,&mSRV))
			return TSun::TFALSE;
		// sampler state
		TLRenderDeviceSamplerDesc samplerDesc;
		samplerDesc.AddressU = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxLOD = (TSun::TF32)TSun::T_FLT_MAX;
		samplerDesc.MinLOD = 0;
		samplerDesc.Filter = RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR;
		mSamplerState = pDevice->createSamplerState(&samplerDesc);
		if(!mSamplerState)
		{
			return TSun::TFALSE;
		}
		// blend state
		TLRenderDeviceBlendDesc blendDesc;
		blendDesc.RenderTarget[0].BlendEnable = TSun::TFALSE;
		blendDesc.RenderTarget[0].SrcBlend = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = RENDER_DEVICE_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = RENDER_DEVICE_BLEND_OP_ADD;
		mBlendState = pDevice->createBlendState(&blendDesc);
		if(!mBlendState)
		{
			return TSun::TFALSE;
		}
		// depth stencil state
		TLRenderDeviceDepthStencilDesc depthDesc;
		depthDesc.DepthEnable = TSun::TTRUE;
		depthDesc.DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = RENDER_DEVICE_COMPARISON_LESS;
		mDepthStencilState = pDevice->createDepthStencilState(&depthDesc);
		if(!mDepthStencilState)
		{
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

	TSun::TVOID TestTriangle::OnRender()
	{
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// Update our time
		static TSun::TF32 t = 0.0f;
		static TSun::TU32 dwTimeStart = 0;
		TSun::TU32 dwTimeCur = GetTickCount();
		if( dwTimeStart == 0 )
			dwTimeStart = dwTimeCur;
		t = ( dwTimeCur - dwTimeStart ) / ta;
		// Rotate cube around the origin
		TSun::Matrix4x4<TSun::TF32> transMatrix = TSun::Matrix4x4<TSun::TF32>();
		transMatrix.SetRotY(t*TSun::RADTODEG);
		transMatrix.Transpose();
		// World
		TSun::Matrix4x4<TSun::TF32> worldMatrix = TSun::Matrix4x4<TSun::TF32>();
		worldMatrix.Identity();
		worldMatrix.Transpose();
		// view
		TSun::Vector3<TSun::TF32> Eye( 0.0f, 3.0f, -6.0f );
		TSun::Vector3<TSun::TF32> At( 0.0f, 1.0f, 0.0f );
		TSun::Vector3<TSun::TF32> Up( 0.0f, 1.0f, 0.0f );
		TSun::Matrix4x4<TSun::TF32> cameraLookAt = TSun::Matrix4x4<TSun::TF32>();
		cameraLookAt.BuildCameraLookAtMatrixLH(Eye,At,Up);
		cameraLookAt.Transpose();
		// projection
		TSun::Matrix4x4<TSun::TF32> projMatrix = TSun::Matrix4x4<TSun::TF32>();
		projMatrix.BuildProjectionMatrixPerspectiveFovLH( TSun::PI * 0.25f,1280.0f / 720.0f,0.1f,100.0f);
		projMatrix.Transpose();
		// update resources
		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = cameraLookAt;
		pDevice->updateSubresource(mCB[0],0,TSun::TNULL,&cbNeverChanges,0,0);
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = projMatrix;
		pDevice->updateSubresource(mCB[1],0,TSun::TNULL,&cbChangesOnResize,0,0);
		CBChangesEveryFrame cb;
		cb.mWorld = worldMatrix;
		cb.transMat = transMatrix;
		pDevice->updateSubresource(mCB[2],0,TSun::TNULL,&cb,0,0);

		pDevice->setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDevice->setInputLayout(mInputLayout2);
		// Cal
		TSun::TU32 offset = 0;
		TSun::TU32 stride = sizeof(SimpleVertex);
		pDevice->setVertexBuffer(0,TSun::TNULL,TSun::TNULL,TSun::TNULL);
		pDevice->setSOTarget(mRB,&offset);
		pDevice->setVertexBuffer(0,mVB,&stride,&offset);
		pDevice->setIndexBuffer(mIB,RENDER_DEVICE_FORMAT_R32_UINT,0);
		pDevice->setVertexShader(mVSAdd);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,2,mCB[2]);
		pDevice->setGeometryShader(mGS);
		pDevice->setPixelShader(TSun::TNULL);
		pDevice->draw(24,0);
		pDevice->setSOTarget(TSun::TNULL,TSun::TNULL);

		// Render
		pDevice->setInputLayout(mInputLayout);
		pDevice->setVertexBuffer(0,mRB,&stride,&offset);
		pDevice->setIndexBuffer(mIB,RENDER_DEVICE_FORMAT_R32_UINT,0);
		pDevice->setVertexShader(mVS);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,0,mCB[0]);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,1,mCB[1]);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,2,mCB[2]);
		pDevice->setGeometryShader(TSun::TNULL);
		pDevice->setPixelShader(mPS);
		pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS,0,mSRV);
		pDevice->setSamplerState(RENDER_DEVICE_SHADER_USE_PS,0,mSamplerState);
		TSun::TF32 blendFactor[4] = {0,0,0,0};
		pDevice->setBlendState(mBlendState,blendFactor,0xffffffff);
		pDevice->setDepthStencilState(mDepthStencilState);
		pDevice->drawIndexed(36,0,0);
	}

}