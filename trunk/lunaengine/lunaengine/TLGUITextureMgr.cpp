#include "TLGUITextureMgr.h"
#include "TLString.h"
#include "TLTxtFileReader.h"
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
	GUITextureMgr* Singleton<GUITextureMgr>::m_Ptr = 0;

	GUITextureMgr::GUITextureMgr(TVOID) : m_bufferHeight(0),m_bufferWidth(0),mVB(TNULL),mVBSet(TNULL),
		mVS(TNULL),mPS(TNULL),mInputLayout(TNULL),mDepthStencilState(TNULL),mBlendState(TNULL),mSamplerState(TNULL)
	{
	}

	GUITextureMgr::~GUITextureMgr(TVOID)
	{
		m_bufferHeight = 0;
		m_bufferWidth = 0;
		if (mVB)
		{
			delete mVB;
			mVB = 0;
		}
		if (mVBSet)
		{
			delete mVBSet;
			mVBSet = 0;
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
		if (mInputLayout)
		{
			delete mInputLayout;
			mInputLayout = 0;
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
		Map<TS32,RenderDeviceUsedSRV*>::Iterator itr = mSRVList.begin();
		for(;itr!=mSRVList.end();++itr)
		{
			RenderDeviceUsedSRV* pSRV = itr->Value;
			if (pSRV)
			{
				delete pSRV;
				pSRV = 0;
			}
			itr->Value = TNULL;
		}
		mSRVList.clear();
	}

	TBOOL GUITextureMgr::Init(TU32 bufferWidth, TU32 bufferHeight, const TCHAR* effectFile)
	{
		m_bufferHeight = bufferHeight;
		m_bufferWidth = bufferWidth;
		if (!InitD3DObj(effectFile))
		{
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID GUITextureMgr::DestroyAllTex()
	{
		Map<TS32,RenderDeviceUsedSRV*>::Iterator itr = mSRVList.begin();
		for(;itr!=mSRVList.end();++itr)
		{
			RenderDeviceUsedSRV* pSRV = itr->Value;
			if (pSRV)
			{
				delete pSRV;
				pSRV = 0;
			}
			itr->Value = TNULL;
		}
		mSRVList.clear();
	}

	TBOOL GUITextureMgr::LoadTexArray(FILE* stream)
	{
		TS32 texID = -1;
		TxtFileReader::ReadLineInteger(&texID,stream,1,' ');
		TCHAR strResult[1024] = {0};
		TS32 nCount = 1024;
		TxtFileReader::ReadLineString(strResult,stream,TNULL,TNULL,1024,TNULL);
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		String fullFile = GlobleClass::getSingletonPtr()->m_strResDir + strResult;
		Image* image = Image::createFromFile(fullFile.GetString());
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
		TLRenderDeviceSubresourceData initData;
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
		RenderDeviceUsedSRV* pSRV = pDevice->createShaderResourceView(pTex,&srvDesc);
		if(!pSRV)
		{
			delete image;
			delete pTex;
			return TFALSE;
		}
		delete image;
		delete pTex;
		mSRVList.push_back(texID,pSRV);
		return TTRUE;
	}

	TBOOL GUITextureMgr::LoadTexFromFile(const TCHAR* file)
	{
		// 先检测，清空原来的
		DestroyAllTex();
		// 读取文件
		FILE* stream = TNULL;
		String strFile(file);
		if (!TxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return TFALSE;
		}
		// 匹配第一行字符
		TBOOL bEqual = TFALSE;
		TCHAR strResult[1024] = {0};
		TS32 nCount = 1024;
		if(!TxtFileReader::ReadLineString(strResult,stream,"TUI_TEX_100",&bEqual,nCount,TNULL))
		{
			TxtFileReader::CloseTxtFile(stream);
			return TFALSE;
		}
		if(!bEqual)
		{
			TxtFileReader::CloseTxtFile(stream);
			return TFALSE;
		}
		// 读取数量
		TS32 texCount = 0;
		TxtFileReader::ReadLineInteger(&texCount,stream,1,' ');
		// 循环读取
		for (TS32 i=0;i<texCount;i++)
		{
			if(!LoadTexArray(stream))
			{
				TxtFileReader::CloseTxtFile(stream);
				return TFALSE;
			}
		}
		TxtFileReader::CloseTxtFile(stream);
		return TTRUE;
	}

	TBOOL GUITextureMgr::InitD3DObj(const TCHAR* effectFile)
	{
		// Init D3DObj
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// Init Shader
		// VS
		RenderDeviceCompiledShader* pCompiledVS = pDevice->createCompiledShader();
		if(!pCompiledVS->compileShader(effectFile,"VS","vs"))
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
		TLRenderDeviceInputElementDesc renderLI[3];
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
		renderLI[2].AlignedByteOffset = 20;
		renderLI[2].Format = RENDER_DEVICE_FORMAT_R32G32B32A32_FLOAT;
		renderLI[2].SemanticIndex = 0;
		renderLI[2].SemanticName = String("COLOR");
		renderLI[2].InputSlot = 0;
		mInputLayout = pDevice->createInputLayout(renderLI,3,pCompiledVS);
		if(!mInputLayout)
		{
			delete pCompiledVS;
			return TFALSE;
		}
		delete pCompiledVS;
		// PS
		RenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(effectFile,"PS","ps"))
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
		// VB
		GUI_VERTEX_DEF vertices[] =
		{
			{ TLunaEngine::Vector3<TF32>( -1.0f, 1.0f, 0.0f ), TLunaEngine::Vector2<TF32>( 0.0f, 0.0f ), TLunaEngine::Vector4<TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::Vector3<TF32>( 1.0f, -1.0f, 0.0f ), TLunaEngine::Vector2<TF32>( 1.0f,1.0f ), TLunaEngine::Vector4<TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::Vector3<TF32>( -1.0f, -1.0f, 0.0f ), TLunaEngine::Vector2<TF32>( 0.0f, 1.0f ), TLunaEngine::Vector4<TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::Vector3<TF32>( -1.0f, 1.0f, 0.0f ), TLunaEngine::Vector2<TF32>( 0.0f, 0.0f ), TLunaEngine::Vector4<TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::Vector3<TF32>( 1.0f, 1.0f, 0.0f ), TLunaEngine::Vector2<TF32>( 1.0f, 0.0f ), TLunaEngine::Vector4<TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::Vector3<TF32>( 1.0f, -1.0f, 0.0f ), TLunaEngine::Vector2<TF32>( 1.0f, 1.0f ), TLunaEngine::Vector4<TF32>(1.0f,1.0f,1.0f,1.0f) },
		};
		TLRenderDeviceBufferDesc vbDesc;
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER;
		vbDesc.ByteWidth = sizeof(GUI_VERTEX_DEF) * 6;
		vbDesc.CPUAccessFlags = TFALSE;
		TLRenderDeviceSubresourceData initData;
		initData.pSysMem = vertices;
		mVB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mVB)
		{
			return TFALSE;
		}
		// VBSet
		vbDesc.BindFlags = 0;
		vbDesc.CPUAccessFlags = TTRUE;
		mVBSet = pDevice->createBuffer(&vbDesc,TNULL);
		// blend state
		TLRenderDeviceBlendDesc blendDesc;
		blendDesc.RenderTarget[0].BlendEnable = TTRUE;
		blendDesc.RenderTarget[0].SrcBlend = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = RENDER_DEVICE_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = RENDER_DEVICE_BLEND_OP_ADD;
		mBlendState = pDevice->createBlendState(&blendDesc);
		if(!mBlendState)
		{
			return TFALSE;
		}
		// depth stencil state
		TLRenderDeviceDepthStencilDesc depthDesc;
		depthDesc.DepthEnable = TFALSE;
		depthDesc.DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ZERO;
		mDepthStencilState = pDevice->createDepthStencilState(&depthDesc);
		if(!mDepthStencilState)
		{
			return TFALSE;
		}
		// sampler state
		TLRenderDeviceSamplerDesc samplerDesc;
		samplerDesc.AddressU = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR;
		mSamplerState = pDevice->createSamplerState(&samplerDesc);
		if(!mSamplerState)
		{
			return TFALSE;
		}
		return TTRUE;
	}

	TBOOL GUITextureMgr::DrawGUICtrl(TS32 x, TS32 y, TS32 width, TS32 height, TF32 texX, TF32 texY, TF32 texR, TF32 texB, TS32 texId,TF32 alpha)
	{
		RenderDeviceUsedSRV* pSRV = TNULL;
		Map<TS32,RenderDeviceUsedSRV*>::Iterator itr = mSRVList.find(texId);
		if(itr!=mSRVList.end())
		{
			pSRV = itr->Value;
		}
		if (pSRV == TNULL)
		{
			return TFALSE;
		}
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		TLunaEngine::Vector4<TF32> color(1.0f,1.0f,1.0f,alpha);
		// 计算
		TF32 xFinal = -1.0f + ((TF32)(x))/m_bufferWidth * 2.0f;
		TF32 yFinal = (-1.0f + ((TF32)(y))/m_bufferHeight * 2.0f) * -1.0f;
		TF32 xLen = ((TF32)(width))/m_bufferWidth * 2.0f;
		TF32 yLen = ((TF32)(height))/m_bufferHeight * 2.0f;
		// vbSet
		GUI_VERTEX_DEF* pVertex;
		TLRenderDeviceMappedSubresource mappedRes;
		if(pDevice->mapResource(mVBSet,0,RENDER_DEVICE_MAP_READ_WRITE,&mappedRes))
		{
			pVertex = (GUI_VERTEX_DEF*)mappedRes.pData;
			pVertex[0].Pos = TLunaEngine::Vector3<TF32>(xFinal,yFinal,0);
			pVertex[0].Tex = TLunaEngine::Vector2<TF32>(texX,texY);
			pVertex[0].Color = color;
			pVertex[1].Pos = TLunaEngine::Vector3<TF32>(xFinal+xLen,yFinal-yLen,0);
			pVertex[1].Tex = TLunaEngine::Vector2<TF32>(texR,texB);
			pVertex[1].Color = color;
			pVertex[2].Pos = TLunaEngine::Vector3<TF32>(xFinal,yFinal-yLen,0);
			pVertex[2].Tex = TLunaEngine::Vector2<TF32>(texX,texB);
			pVertex[2].Color = color;
			pVertex[3].Pos = TLunaEngine::Vector3<TF32>(xFinal,yFinal,0);
			pVertex[3].Tex = TLunaEngine::Vector2<TF32>(texX,texY);
			pVertex[3].Color = color;
			pVertex[4].Pos = TLunaEngine::Vector3<TF32>(xFinal+xLen,yFinal,0);
			pVertex[4].Tex = TLunaEngine::Vector2<TF32>(texR,texY);
			pVertex[4].Color = color;
			pVertex[5].Pos = TLunaEngine::Vector3<TF32>(xFinal+xLen,yFinal-yLen,0);
			pVertex[5].Tex = TLunaEngine::Vector2<TF32>(texR,texB);
			pVertex[5].Color = color;
			pDevice->unmapResource(mVBSet,0);
			pDevice->copyResource(mVB,mVBSet);
		}

		// render
		pDevice->setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDevice->setInputLayout(mInputLayout);
		TU32 stride = sizeof(GUI_VERTEX_DEF);
		TU32 offset = 0;
		pDevice->setVertexBuffer(0,mVB,&stride,&offset);
		pDevice->setVertexShader(mVS);
		pDevice->setPixelShader(mPS);
		pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS,0,pSRV);
		pDevice->setSamplerState(RENDER_DEVICE_SHADER_USE_PS,0,mSamplerState);
		TF32 blendFactor[4] = {0,0,0,0};
		pDevice->setBlendState(mBlendState,blendFactor,0xffffffff);
		pDevice->setDepthStencilState(mDepthStencilState);
		pDevice->draw(6,0);
		return TTRUE;
	}

}