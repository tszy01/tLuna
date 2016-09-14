#include "StdAfx.h"
#include "TLTriangle.h"
#include "TString.h"
#include "TMatrix4x4.h"
#include "TLGlobleClass.h"
#include "TLRenderMgr.h"
#include "TLIRenderDevice.h"
#include "TLIRenderDeviceUsedBlendState.h"
#include "TLIRenderDeviceUsedBuffer.h"
#include "TLIRenderDeviceUsedTex2D.h"
#include "TLIRenderDeviceUsedSRV.h"
#include "TLIRenderDeviceUsedVS.h"
#include "TLIRenderDeviceUsedPS.h"
#include "TLIRenderDeviceUsedGS.h"
#include "TLIRenderDeviceUsedDepthStencilState.h"
#include "TLIRenderDeviceUsedSamplerState.h"
#include "TLIRenderDeviceUsedInputLayout.h"
#include "TLIRenderDeviceCompiledShader.h"
#include "TLImage.h"

namespace TLunaEngine{

	TLTriangle::TLTriangle(void) : ta(1000.0f),mVB(NULL),mIB(NULL),mRB(NULL),mSRV(NULL),
		mVSAdd(NULL),mGS(NULL),mVS(NULL),mPS(NULL),mInputLayout(NULL),mSamplerState(NULL),
		mInputLayout2(NULL),mDepthStencilState(NULL),mBlendState(NULL)
	{
		mCB[0] = mCB[1] = mCB[2] = NULL;
	}

	TLTriangle::~TLTriangle(void)
	{
		SAFE_DELETE(mVB);
		SAFE_DELETE(mIB);
		SAFE_DELETE(mRB);
		SAFE_DELETE(mCB[0]);
		SAFE_DELETE(mCB[1]);
		SAFE_DELETE(mCB[2]);
		SAFE_DELETE(mVS);
		SAFE_DELETE(mPS);
		SAFE_DELETE(mVSAdd);
		SAFE_DELETE(mGS);
		SAFE_DELETE(mInputLayout);
		SAFE_DELETE(mInputLayout2);
		SAFE_DELETE(mSamplerState);
		SAFE_DELETE(mDepthStencilState);
		SAFE_DELETE(mBlendState);
	}

	BOOL TLTriangle::InitTriangle()
	{
		// Init D3DObj
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		// Init Shader
		TString strShader = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\Triangle0.fx";
		// VS
		TLIRenderDeviceCompiledShader* pCompiledVS = pDevice->createCompiledShader();
		if(!pCompiledVS->compileShader(strShader.GetString(),"VS","vs"))
		{
			delete pCompiledVS;
			return FALSE;
		}
		mVS = pDevice->createVertexShader(pCompiledVS);
		if(!mVS)
		{
			delete pCompiledVS;
			return FALSE;
		}
		// input layout
		TLRenderDeviceInputElementDesc renderLI[2];
		renderLI[0].AlignedByteOffset = 0;
		renderLI[0].Format = RENDER_DEVICE_FORMAT_R32G32B32_FLOAT;
		renderLI[0].SemanticIndex = 0;
		renderLI[0].SemanticName = TString("POSITION");
		renderLI[0].InputSlot = 0;
		renderLI[1].AlignedByteOffset = 12;
		renderLI[1].Format = RENDER_DEVICE_FORMAT_R32G32_FLOAT;
		renderLI[1].SemanticIndex = 0;
		renderLI[1].SemanticName = TString("TEXCOORD");
		renderLI[1].InputSlot = 0;
		mInputLayout = pDevice->createInputLayout(renderLI,2,pCompiledVS);
		if(!mInputLayout)
		{
			delete pCompiledVS;
			return FALSE;
		}
		delete pCompiledVS;
		// PS
		TLIRenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(strShader.GetString(),"PS","ps"))
		{
			delete pCompiledPS;
			return FALSE;
		}
		mPS = pDevice->createPixelShader(pCompiledPS);
		if(!mPS)
		{
			delete pCompiledPS;
			return FALSE;
		}
		delete pCompiledPS;
		// VSAdd
		TLIRenderDeviceCompiledShader* pCompiledVSAdd = pDevice->createCompiledShader();
		if(!pCompiledVSAdd->compileShader(strShader.GetString(),"AddVS","vs"))
		{
			delete pCompiledVSAdd;
			return FALSE;
		}
		mVSAdd = pDevice->createVertexShader(pCompiledVSAdd);
		if(!mVSAdd)
		{
			delete pCompiledVSAdd;
			return FALSE;
		}
		// input layout
		mInputLayout2 = pDevice->createInputLayout(renderLI,2,pCompiledVSAdd);
		if(!mInputLayout2)
		{
			delete pCompiledVSAdd;
			return FALSE;
		}
		// GS
		TLRenderDeviceSODeclarationEntry entry[2];
		entry[0].ComponentCount = 3;
		entry[0].SemanticIndex = 0;
		entry[0].SemanticName = TString("POSITION");
		entry[0].StartComponent = 0;
		entry[1].ComponentCount = 2;
		entry[1].SemanticIndex = 0;
		entry[1].SemanticName = TString("TEXCOORD");
		entry[1].StartComponent = 0;
		mGS = pDevice->createGeometryShaderWithStreamOutput(pCompiledVSAdd,entry,2);
		if(!mGS)
		{
			delete pCompiledVSAdd;
			return FALSE;
		}
		delete pCompiledVSAdd;
		// VB
		SimpleVertex vertices[] =
		{
			{ TVecFloat3( -1.0f, 1.0f, -1.0f ), TVecFloat2( 0.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, 1.0f, -1.0f ), TVecFloat2( 1.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, 1.0f, 1.0f ), TVecFloat2( 1.0f, 1.0f ) },
			{ TVecFloat3( -1.0f, 1.0f, 1.0f ), TVecFloat2( 0.0f, 1.0f ) },

			{ TVecFloat3( -1.0f, -1.0f, -1.0f ), TVecFloat2( 0.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, -1.0f, -1.0f ), TVecFloat2( 1.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, -1.0f, 1.0f ), TVecFloat2( 1.0f, 1.0f ) },
			{ TVecFloat3( -1.0f, -1.0f, 1.0f ), TVecFloat2( 0.0f, 1.0f ) },

			{ TVecFloat3( -1.0f, -1.0f, 1.0f ), TVecFloat2( 0.0f, 0.0f ) },
			{ TVecFloat3( -1.0f, -1.0f, -1.0f ), TVecFloat2( 1.0f, 0.0f ) },
			{ TVecFloat3( -1.0f, 1.0f, -1.0f ), TVecFloat2( 1.0f, 1.0f ) },
			{ TVecFloat3( -1.0f, 1.0f, 1.0f ), TVecFloat2( 0.0f, 1.0f ) },

			{ TVecFloat3( 1.0f, -1.0f, 1.0f ), TVecFloat2( 0.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, -1.0f, -1.0f ), TVecFloat2( 1.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, 1.0f, -1.0f ), TVecFloat2( 1.0f, 1.0f ) },
			{ TVecFloat3( 1.0f, 1.0f, 1.0f ), TVecFloat2( 0.0f, 1.0f ) },

			{ TVecFloat3( -1.0f, -1.0f, -1.0f ), TVecFloat2( 0.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, -1.0f, -1.0f ), TVecFloat2( 1.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, 1.0f, -1.0f ), TVecFloat2( 1.0f, 1.0f ) },
			{ TVecFloat3( -1.0f, 1.0f, -1.0f ), TVecFloat2( 0.0f, 1.0f ) },

			{ TVecFloat3( -1.0f, -1.0f, 1.0f ), TVecFloat2( 0.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, -1.0f, 1.0f ), TVecFloat2( 1.0f, 0.0f ) },
			{ TVecFloat3( 1.0f, 1.0f, 1.0f ), TVecFloat2( 1.0f, 1.0f ) },
			{ TVecFloat3( -1.0f, 1.0f, 1.0f ), TVecFloat2( 0.0f, 1.0f ) },
		};
		TLRenderDeviceBufferDesc vbDesc;
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER;
		vbDesc.ByteWidth = sizeof(SimpleVertex) * 24;
		vbDesc.CPUAccessFlags = FALSE;
		TLRenderDeviceSubresourceData initData;
		initData.pSysMem = vertices;
		mVB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mVB)
		{
			return FALSE;
		}
		// RB
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER | RENDER_DEVICE_BIND_FLAG_STREAM_OUTPUT;
		mRB = pDevice->createBuffer(&vbDesc,NULL);
		if(!mRB)
		{
			return FALSE;
		}
		// IB
		DWORD indices[] =
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
		vbDesc.ByteWidth = sizeof(DWORD)*36;
		initData.pSysMem = indices;
		mIB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mIB)
		{
			return FALSE;
		}
		// CB0
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBNeverChanges);
		mCB[0] = pDevice->createBuffer(&vbDesc,NULL);
		if(!mCB[0])
		{
			return FALSE;
		}
		// CB1
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBChangeOnResize);
		mCB[1] = pDevice->createBuffer(&vbDesc,NULL);
		if(!mCB[1])
		{
			return FALSE;
		}
		// CB2
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER;
		vbDesc.ByteWidth = sizeof(CBChangesEveryFrame);
		mCB[2] = pDevice->createBuffer(&vbDesc,NULL);
		if(!mCB[2])
		{
			return FALSE;
		}
		// Shader Resource
		TString strTex = TLGlobleClass::getSingletonPtr()->m_strResDir + "texture\\seafloor.dds";
		/*TLImage* image = TLImage::createFromFile(strTex.GetString());
		if(!image)
			return FALSE;
		UINT imgw,imgh;
		image->getImageSize(&imgw,&imgh);
		RENDER_DEVICE_FORMAT imgFormat = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
		if(image->getPixelFormat() == TLImage::PIXEL_FORMAT_R8)
		{
			imgFormat = RENDER_DEVICE_FORMAT_R8_UNORM;
		}
		TLRenderDeviceTex2DDesc texDesc;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = FALSE;
		texDesc.Cube = FALSE;
		texDesc.Format = imgFormat;
		texDesc.GenerateMips = FALSE;
		texDesc.Height = imgh;
		texDesc.MipLevels = 1;
		texDesc.SampleCount = 1;
		texDesc.SampleQuality = 0;
		texDesc.Width = imgw;
		initData.pSysMem = image->getBuffer();
		initData.SysMemPitch = imgw*image->getPixelSize();
		TLIRenderDeviceUsedTex2D* pTex = pDevice->createTex2D(&texDesc,&initData);
		if(!pTex)
		{
			delete image;
			return FALSE;
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
			return FALSE;
		}
		delete image;
		delete pTex;*/
		if(!pDevice->createTex2DFromDDSFile(strTex.GetString(),NULL,&mSRV))
			return FALSE;
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
			return FALSE;
		}
		// blend state
		TLRenderDeviceBlendDesc blendDesc;
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
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
		depthDesc.DepthEnable = TRUE;
		depthDesc.DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = RENDER_DEVICE_COMPARISON_LESS;
		mDepthStencilState = pDevice->createDepthStencilState(&depthDesc);
		if(!mDepthStencilState)
		{
			return false;
		}
		return TRUE;
	}

	void TLTriangle::OnRender()
	{
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		// Update our time
		static float t = 0.0f;
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if( dwTimeStart == 0 )
			dwTimeStart = dwTimeCur;
		t = ( dwTimeCur - dwTimeStart ) / ta;
		// Rotate cube around the origin
		TLunaEngine::TMatrix4x4<TF32> transMatrix = TLunaEngine::TMatrix4x4<TF32>();
		transMatrix.SetRotY(t*TLunaEngine::RADTODEG);
		transMatrix.Transpose();
		// World
		TLunaEngine::TMatrix4x4<TF32> worldMatrix = TLunaEngine::TMatrix4x4<TF32>();
		worldMatrix.Identity();
		worldMatrix.Transpose();
		// view
		TLunaEngine::TVector3<TF32> Eye( 0.0f, 3.0f, -6.0f );
		TLunaEngine::TVector3<TF32> At( 0.0f, 1.0f, 0.0f );
		TLunaEngine::TVector3<TF32> Up( 0.0f, 1.0f, 0.0f );
		TLunaEngine::TMatrix4x4<TF32> cameraLookAt = TLunaEngine::TMatrix4x4<TF32>();
		cameraLookAt.BuildCameraLookAtMatrixLH(Eye,At,Up);
		cameraLookAt.Transpose();
		// projection
		TLunaEngine::TMatrix4x4<TF32> projMatrix = TLunaEngine::TMatrix4x4<TF32>();
		projMatrix.BuildProjectionMatrixPerspectiveFovLH( TLunaEngine::PI * 0.25f,1280.0f / 720.0f,0.1f,100.0f);
		projMatrix.Transpose();
		// update resources
		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = cameraLookAt;
		pDevice->updateSubresource(mCB[0],0,NULL,&cbNeverChanges,0,0);
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = projMatrix;
		pDevice->updateSubresource(mCB[1],0,NULL,&cbChangesOnResize,0,0);
		CBChangesEveryFrame cb;
		cb.mWorld = worldMatrix;
		cb.transMat = transMatrix;
		pDevice->updateSubresource(mCB[2],0,NULL,&cb,0,0);

		pDevice->setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDevice->setInputLayout(mInputLayout2);
		// Cal
		UINT offset = 0;
		UINT stride = sizeof(SimpleVertex);
		pDevice->setVertexBuffer(0,NULL,NULL,NULL);
		pDevice->setSOTarget(mRB,&offset);
		pDevice->setVertexBuffer(0,mVB,&stride,&offset);
		pDevice->setIndexBuffer(mIB,RENDER_DEVICE_FORMAT_R32_UINT,0);
		pDevice->setVertexShader(mVSAdd);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,2,mCB[2]);
		pDevice->setGeometryShader(mGS);
		pDevice->setPixelShader(NULL);
		pDevice->draw(24,0);
		pDevice->setSOTarget(NULL,NULL);

		// Render
		pDevice->setInputLayout(mInputLayout);
		pDevice->setVertexBuffer(0,mRB,&stride,&offset);
		pDevice->setIndexBuffer(mIB,RENDER_DEVICE_FORMAT_R32_UINT,0);
		pDevice->setVertexShader(mVS);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,0,mCB[0]);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,1,mCB[1]);
		pDevice->setConstantBuffer(RENDER_DEVICE_SHADER_USE_VS,2,mCB[2]);
		pDevice->setGeometryShader(NULL);
		pDevice->setPixelShader(mPS);
		pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS,0,mSRV);
		pDevice->setSamplerState(RENDER_DEVICE_SHADER_USE_PS,0,mSamplerState);
		FLOAT blendFactor[4] = {0,0,0,0};
		pDevice->setBlendState(mBlendState,blendFactor,0xffffffff);
		pDevice->setDepthStencilState(mDepthStencilState);
		pDevice->drawIndexed(36,0,0);
	}

}