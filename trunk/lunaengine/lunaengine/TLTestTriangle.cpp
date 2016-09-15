#include "TLTestTriangle.h"
#include "TLString.h"
#include "TLMatrix4x4.h"
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

namespace TLunaEngine{

	TestTriangle::TestTriangle(void) : ta(1000.0f),mVB(TNULL),mIB(TNULL),mRB(TNULL),mSRV(TNULL),
		mVSAdd(TNULL),mGS(TNULL),mVS(TNULL),mPS(TNULL),mInputLayout(TNULL),mSamplerState(TNULL),
		mInputLayout2(TNULL),mDepthStencilState(TNULL),mBlendState(TNULL)
	{
		mCB[0] = mCB[1] = mCB[2] = TNULL;
	}

	TestTriangle::~TestTriangle(void)
	{
		if (mVB)
		{
			delete mVB;
			mVB = 0;
		}
		if (mIB)
		{
			delete mIB;
			mIB = 0;
		}
		if (mRB)
		{
			delete mRB;
			mRB = 0;
		}
		if (mCB[0])
		{
			delete mCB[0];
			mCB[0] = 0;
		}
		if (mCB[1])
		{
			delete mCB[1];
			mCB[1] = 0;
		}
		if (mCB[2])
		{
			delete mCB[2];
			mCB[2] = 0;
		}
		if (mVS)
		{
			delete mVS;
			mVS = 0;
		}
		if (mPS)
		{
			delete mPS;
			mPS = 0;
		}
		if (mVSAdd)
		{
			delete mVSAdd;
			mVSAdd = 0;
		}
		if (mGS)
		{
			delete mGS;
			mGS = 0;
		}
		if (mInputLayout)
		{
			delete mInputLayout;
			mInputLayout = 0;
		}
		if (mInputLayout2)
		{
			delete mInputLayout2;
			mInputLayout2 = 0;
		}
		if (mSamplerState)
		{
			delete mSamplerState;
			mSamplerState = 0;
		}
		if (mDepthStencilState)
		{
			delete mDepthStencilState;
			mDepthStencilState = 0;
		}
		if (mBlendState)
		{
			delete mBlendState;
			mBlendState = 0;
		}
	}

	TBOOL TestTriangle::InitTriangle()
	{
		// Init D3DObj
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// Init Shader
		String strShader = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\Triangle0.fx";
		// VS
		RenderDeviceCompiledShader* pCompiledVS = pDevice->createCompiledShader();
		if(!pCompiledVS->compileShader(strShader.GetString(),"VS","vs"))
		{
			delete pCompiledVS;
			return TFALSE;
		}
		mVS = pDevice->createVertexShader(pCompiledVS);
		if(!mVS)
		{
			delete pCompiledVS;
			return TFALSE;
		}
		// input layout
		TLRenderDeviceInputElementDesc renderLI[2];
		renderLI[0].AlignedByteOffset = 0;
		renderLI[0].Format = RENDER_DEVICE_FORMAT_R32G32B32_FLOAT;
		renderLI[0].SemanticIndex = 0;
		renderLI[0].SemanticName = String("POSITION");
		renderLI[0].InputSlot = 0;
		renderLI[1].AlignedByteOffset = 12;
		renderLI[1].Format = RENDER_DEVICE_FORMAT_R32G32_FLOAT;
		renderLI[1].SemanticIndex = 0;
		renderLI[1].SemanticName = String("TEXCOORD");
		renderLI[1].InputSlot = 0;
		mInputLayout = pDevice->createInputLayout(renderLI,2,pCompiledVS);
		if(!mInputLayout)
		{
			delete pCompiledVS;
			return TFALSE;
		}
		delete pCompiledVS;
		// PS
		RenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(strShader.GetString(),"PS","ps"))
		{
			delete pCompiledPS;
			return TFALSE;
		}
		mPS = pDevice->createPixelShader(pCompiledPS);
		if(!mPS)
		{
			delete pCompiledPS;
			return TFALSE;
		}
		delete pCompiledPS;
		// VSAdd
		RenderDeviceCompiledShader* pCompiledVSAdd = pDevice->createCompiledShader();
		if(!pCompiledVSAdd->compileShader(strShader.GetString(),"AddVS","vs"))
		{
			delete pCompiledVSAdd;
			return TFALSE;
		}
		mVSAdd = pDevice->createVertexShader(pCompiledVSAdd);
		if(!mVSAdd)
		{
			delete pCompiledVSAdd;
			return TFALSE;
		}
		// input layout
		mInputLayout2 = pDevice->createInputLayout(renderLI,2,pCompiledVSAdd);
		if(!mInputLayout2)
		{
			delete pCompiledVSAdd;
			return TFALSE;
		}
		// GS
		TLRenderDeviceSODeclarationEntry entry[2];
		entry[0].ComponentCount = 3;
		entry[0].SemanticIndex = 0;
		entry[0].SemanticName = String("POSITION");
		entry[0].StartComponent = 0;
		entry[1].ComponentCount = 2;
		entry[1].SemanticIndex = 0;
		entry[1].SemanticName = String("TEXCOORD");
		entry[1].StartComponent = 0;
		mGS = pDevice->createGeometryShaderWithStreamOutput(pCompiledVSAdd,entry,2);
		if(!mGS)
		{
			delete pCompiledVSAdd;
			return TFALSE;
		}
		delete pCompiledVSAdd;
		// VB
		SimpleVertex vertices[] =
		{
			{ Vector3Float( -1.0f, 1.0f, -1.0f ), Vector2Float( 0.0f, 0.0f ) },
			{ Vector3Float( 1.0f, 1.0f, -1.0f ), Vector2Float( 1.0f, 0.0f ) },
			{ Vector3Float( 1.0f, 1.0f, 1.0f ), Vector2Float( 1.0f, 1.0f ) },
			{ Vector3Float( -1.0f, 1.0f, 1.0f ), Vector2Float( 0.0f, 1.0f ) },

			{ Vector3Float( -1.0f, -1.0f, -1.0f ), Vector2Float( 0.0f, 0.0f ) },
			{ Vector3Float( 1.0f, -1.0f, -1.0f ), Vector2Float( 1.0f, 0.0f ) },
			{ Vector3Float( 1.0f, -1.0f, 1.0f ), Vector2Float( 1.0f, 1.0f ) },
			{ Vector3Float( -1.0f, -1.0f, 1.0f ), Vector2Float( 0.0f, 1.0f ) },

			{ Vector3Float( -1.0f, -1.0f, 1.0f ), Vector2Float( 0.0f, 0.0f ) },
			{ Vector3Float( -1.0f, -1.0f, -1.0f ), Vector2Float( 1.0f, 0.0f ) },
			{ Vector3Float( -1.0f, 1.0f, -1.0f ), Vector2Float( 1.0f, 1.0f ) },
			{ Vector3Float( -1.0f, 1.0f, 1.0f ), Vector2Float( 0.0f, 1.0f ) },

			{ Vector3Float( 1.0f, -1.0f, 1.0f ), Vector2Float( 0.0f, 0.0f ) },
			{ Vector3Float( 1.0f, -1.0f, -1.0f ), Vector2Float( 1.0f, 0.0f ) },
			{ Vector3Float( 1.0f, 1.0f, -1.0f ), Vector2Float( 1.0f, 1.0f ) },
			{ Vector3Float( 1.0f, 1.0f, 1.0f ), Vector2Float( 0.0f, 1.0f ) },

			{ Vector3Float( -1.0f, -1.0f, -1.0f ), Vector2Float( 0.0f, 0.0f ) },
			{ Vector3Float( 1.0f, -1.0f, -1.0f ), Vector2Float( 1.0f, 0.0f ) },
			{ Vector3Float( 1.0f, 1.0f, -1.0f ), Vector2Float( 1.0f, 1.0f ) },
			{ Vector3Float( -1.0f, 1.0f, -1.0f ), Vector2Float( 0.0f, 1.0f ) },

			{ Vector3Float( -1.0f, -1.0f, 1.0f ), Vector2Float( 0.0f, 0.0f ) },
			{ Vector3Float( 1.0f, -1.0f, 1.0f ), Vector2Float( 1.0f, 0.0f ) },
			{ Vector3Float( 1.0f, 1.0f, 1.0f ), Vector2Float( 1.0f, 1.0f ) },
			{ Vector3Float( -1.0f, 1.0f, 1.0f ), Vector2Float( 0.0f, 1.0f ) },
		};
		TLRenderDeviceBufferDesc vbDesc;
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER;
		vbDesc.ByteWidth = sizeof(SimpleVertex) * 24;
		vbDesc.CPUAccessFlags = TFALSE;
		TLRenderDeviceSubresourceData initData;
		initData.pSysMem = vertices;
		mVB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mVB)
		{
			return TFALSE;
		}
		// RB
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER | RENDER_DEVICE_BIND_FLAG_STREAM_OUTPUT;
		mRB = pDevice->createBuffer(&vbDesc,TNULL);
		if(!mRB)
		{
			return TFALSE;
		}
		// IB
		TU32 indices[] =
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
		vbDesc.ByteWidth = sizeof(TU32)*36;
		initData.pSysMem = indices;
		mIB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mIB)
		{
			return TFALSE;
		}
		// CB0
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBNeverChanges);
		mCB[0] = pDevice->createBuffer(&vbDesc,TNULL);
		if(!mCB[0])
		{
			return TFALSE;
		}
		// CB1
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBChangeOnResize);
		mCB[1] = pDevice->createBuffer(&vbDesc,TNULL);
		if(!mCB[1])
		{
			return TFALSE;
		}
		// CB2
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBChangesEveryFrame);
		mCB[2] = pDevice->createBuffer(&vbDesc,TNULL);
		if(!mCB[2])
		{
			return TFALSE;
		}
		// Shader Resource
		String strTex = GlobleClass::getSingletonPtr()->m_strResDir + "texture\\seafloor.dds";
		/*Image* image = Image::createFromFile(strTex.GetString());
		if(!image)
			return TFALSE;
		TU32 imgw,imgh;
		image->getImageSize(&imgw,&imgh);
		RENDER_DEVICE_FORMAT imgFormat = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
		if(image->getPixelFormat() == Image::PIXEL_FORMAT_R8)
		{
			imgFormat = RENDER_DEVICE_FORMAT_R8_UNORM;
		}
		TLRenderDeviceTex2DDesc texDesc;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = TFALSE;
		texDesc.Cube = TFALSE;
		texDesc.Format = imgFormat;
		texDesc.GenerateMips = TFALSE;
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
			return TFALSE;
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
			return TFALSE;
		}
		delete image;
		delete pTex;*/
		if(!pDevice->createTex2DFromDDSFile(strTex.GetString(),TNULL,&mSRV))
			return TFALSE;
		// sampler state
		TLRenderDeviceSamplerDesc samplerDesc;
		samplerDesc.AddressU = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxLOD = FLT_MAX;
		samplerDesc.MinLOD = 0;
		samplerDesc.Filter = RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR;
		mSamplerState = pDevice->createSamplerState(&samplerDesc);
		if(!mSamplerState)
		{
			return TFALSE;
		}
		// blend state
		TLRenderDeviceBlendDesc blendDesc;
		blendDesc.RenderTarget[0].BlendEnable = TFALSE;
		blendDesc.RenderTarget[0].SrcBlend = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = RENDER_DEVICE_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = RENDER_DEVICE_BLEND_OP_ADD;
		mBlendState = pDevice->createBlendState(&blendDesc);
		if(!mBlendState)
		{
			return false;
		}
		// depth stencil state
		TLRenderDeviceDepthStencilDesc depthDesc;
		depthDesc.DepthEnable = TTRUE;
		depthDesc.DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = RENDER_DEVICE_COMPARISON_LESS;
		mDepthStencilState = pDevice->createDepthStencilState(&depthDesc);
		if(!mDepthStencilState)
		{
			return false;
		}
		return TTRUE;
	}

	void TestTriangle::OnRender()
	{
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// Update our time
		static float t = 0.0f;
		static TU32 dwTimeStart = 0;
		TU32 dwTimeCur = GetTickCount();
		if( dwTimeStart == 0 )
			dwTimeStart = dwTimeCur;
		t = ( dwTimeCur - dwTimeStart ) / ta;
		// Rotate cube around the origin
		TLunaEngine::Matrix4x4<TF32> transMatrix = TLunaEngine::Matrix4x4<TF32>();
		transMatrix.SetRotY(t*TLunaEngine::RADTODEG);
		transMatrix.Transpose();
		// World
		TLunaEngine::Matrix4x4<TF32> worldMatrix = TLunaEngine::Matrix4x4<TF32>();
		worldMatrix.Identity();
		worldMatrix.Transpose();
		// view
		TLunaEngine::Vector3<TF32> Eye( 0.0f, 3.0f, -6.0f );
		TLunaEngine::Vector3<TF32> At( 0.0f, 1.0f, 0.0f );
		TLunaEngine::Vector3<TF32> Up( 0.0f, 1.0f, 0.0f );
		TLunaEngine::Matrix4x4<TF32> cameraLookAt = TLunaEngine::Matrix4x4<TF32>();
		cameraLookAt.BuildCameraLookAtMatrixLH(Eye,At,Up);
		cameraLookAt.Transpose();
		// projection
		TLunaEngine::Matrix4x4<TF32> projMatrix = TLunaEngine::Matrix4x4<TF32>();
		projMatrix.BuildProjectionMatrixPerspectiveFovLH( TLunaEngine::PI * 0.25f,1280.0f / 720.0f,0.1f,100.0f);
		projMatrix.Transpose();
		// update resources
		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = cameraLookAt;
		pDevice->updateSubresource(mCB[0],0,TNULL,&cbNeverChanges,0,0);
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = projMatrix;
		pDevice->updateSubresource(mCB[1],0,TNULL,&cbChangesOnResize,0,0);
		CBChangesEveryFrame cb;
		cb.mWorld = worldMatrix;
		cb.transMat = transMatrix;
		pDevice->updateSubresource(mCB[2],0,TNULL,&cb,0,0);

		pDevice->setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDevice->setInputLayout(mInputLayout2);
		// Cal
		TU32 offset = 0;
		TU32 stride = sizeof(SimpleVertex);
		pDevice->setVertexBuffer(0,TNULL,TNULL,TNULL);
		pDevice->setSOTarget(mRB,&offset);
		pDevice->setVertexBuffer(0,mVB,&stride,&offset);
		pDevice->setIndexBuffer(mIB,RENDER_DEVICE_FORMAT_R32_UINT,0);
		pDevice->setVertexShader(mVSAdd);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,2,mCB[2]);
		pDevice->setGeometryShader(mGS);
		pDevice->setPixelShader(TNULL);
		pDevice->draw(24,0);
		pDevice->setSOTarget(TNULL,TNULL);

		// Render
		pDevice->setInputLayout(mInputLayout);
		pDevice->setVertexBuffer(0,mRB,&stride,&offset);
		pDevice->setIndexBuffer(mIB,RENDER_DEVICE_FORMAT_R32_UINT,0);
		pDevice->setVertexShader(mVS);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,0,mCB[0]);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,1,mCB[1]);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,2,mCB[2]);
		pDevice->setGeometryShader(TNULL);
		pDevice->setPixelShader(mPS);
		pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS,0,mSRV);
		pDevice->setSamplerState(RENDER_DEVICE_SHADER_USE_PS,0,mSamplerState);
		TF32 blendFactor[4] = {0,0,0,0};
		pDevice->setBlendState(mBlendState,blendFactor,0xffffffff);
		pDevice->setDepthStencilState(mDepthStencilState);
		pDevice->drawIndexed(36,0,0);
	}

}