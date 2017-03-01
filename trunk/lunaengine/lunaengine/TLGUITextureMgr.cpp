#include "TLGUITextureMgr.h"
#include "TSString.h"
#include "TSTxtFileReader.h"
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

TLunaEngine::GUITextureMgr* TSun::Singleton<TLunaEngine::GUITextureMgr>::m_Ptr = 0;

namespace TLunaEngine{
	GUITextureMgr::GUITextureMgr(TSun::TVOID) : m_bufferHeight(0),m_bufferWidth(0),mVB(TSun::TNULL),mVBSet(TSun::TNULL),
		mVS(TSun::TNULL),mPS(TSun::TNULL),mInputLayout(TSun::TNULL),mDepthStencilState(TSun::TNULL),mBlendState(TSun::TNULL),mSamplerState(TSun::TNULL)
	{
	}

	GUITextureMgr::~GUITextureMgr(TSun::TVOID)
	{
		m_bufferHeight = 0;
		m_bufferWidth = 0;
		if (mVB)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mVB);
			mVB = 0;
		}
		if (mVBSet)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer, mVBSet);
			mVBSet = 0;
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
		if (mInputLayout)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedInputLayout, mInputLayout);
			mInputLayout = 0;
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
		TSun::Map<TSun::TS32,RenderDeviceUsedSRV*>::Iterator itr = mSRVList.begin();
		for(;itr!=mSRVList.end();++itr)
		{
			RenderDeviceUsedSRV* pSRV = itr->Value;
			if (pSRV)
			{
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSRV, pSRV);
				pSRV = 0;
			}
			itr->Value = TSun::TNULL;
		}
		mSRVList.clear();
	}

	TSun::TBOOL GUITextureMgr::Init(TSun::TU32 bufferWidth, TSun::TU32 bufferHeight, const TSun::TCHAR* effectFile)
	{
		m_bufferHeight = bufferHeight;
		m_bufferWidth = bufferWidth;
		if (!InitD3DObj(effectFile))
		{
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

	TSun::TVOID GUITextureMgr::DestroyAllTex()
	{
		TSun::Map<TSun::TS32,RenderDeviceUsedSRV*>::Iterator itr = mSRVList.begin();
		for(;itr!=mSRVList.end();++itr)
		{
			RenderDeviceUsedSRV* pSRV = itr->Value;
			if (pSRV)
			{
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSRV, pSRV);
				pSRV = 0;
			}
			itr->Value = TSun::TNULL;
		}
		mSRVList.clear();
	}

	TSun::TBOOL GUITextureMgr::LoadTexArray(FILE* stream)
	{
		TSun::TS32 texID = -1;
		TSun::TxtFileReader::ReadLineInteger(&texID,stream,1,' ');
		TSun::TCHAR strResult[1024] = {0};
		TSun::TS32 nCount = 1024;
		TSun::TxtFileReader::ReadLineString(strResult,stream,TSun::TNULL,TSun::TNULL,1024,TSun::TNULL);
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		TSun::String fullFile = GlobleClass::getSingletonPtr()->m_strResDir + strResult;
		Image* image = Image::createFromFile(fullFile.GetString());
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
		TLRenderDeviceSubresourceData initData;
		initData.pSysMem = image->getBuffer();
		initData.SysMemPitch = imgw*image->getPixelSize();
		RenderDeviceUsedTex2D* pTex = pDevice->createTex2D(&texDesc,&initData);
		if(!pTex)
		{
			T_DELETE(getEngineStructMemAllocator(), Image, image);
			return TSun::TFALSE;
		}
		TLRenderDeviceSRVDesc srvDesc;
		srvDesc.Format = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = RENDER_DEVICE_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Params[0] = 0;
		srvDesc.Params[1] = 1;
		RenderDeviceUsedSRV* pSRV = pDevice->createShaderResourceView(pTex,&srvDesc);
		if(!pSRV)
		{
			T_DELETE(getEngineStructMemAllocator(), Image, image);
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedTex2D, pTex);
			return TSun::TFALSE;
		}
		T_DELETE(getEngineStructMemAllocator(), Image, image);
		T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedTex2D, pTex);
		mSRVList.push_back(texID,pSRV);
		return TSun::TTRUE;
	}

	TSun::TBOOL GUITextureMgr::LoadTexFromFile(const TSun::TCHAR* file)
	{
		// 先检测，清空原来的
		DestroyAllTex();
		// 读取文件
		FILE* stream = TSun::TNULL;
		TSun::String strFile(file);
		if (!TSun::TxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return TSun::TFALSE;
		}
		// 匹配第一行字符
		TSun::TBOOL bEqual = TSun::TFALSE;
		TSun::TCHAR strResult[1024] = {0};
		TSun::TS32 nCount = 1024;
		if(!TSun::TxtFileReader::ReadLineString(strResult,stream,"TUI_TEX_100",&bEqual,nCount,TSun::TNULL))
		{
			TSun::TxtFileReader::CloseTxtFile(stream);
			return TSun::TFALSE;
		}
		if(!bEqual)
		{
			TSun::TxtFileReader::CloseTxtFile(stream);
			return TSun::TFALSE;
		}
		// 读取数量
		TSun::TS32 texCount = 0;
		TSun::TxtFileReader::ReadLineInteger(&texCount,stream,1,' ');
		// 循环读取
		for (TSun::TS32 i=0;i<texCount;i++)
		{
			if(!LoadTexArray(stream))
			{
				TSun::TxtFileReader::CloseTxtFile(stream);
				return TSun::TFALSE;
			}
		}
		TSun::TxtFileReader::CloseTxtFile(stream);
		return TSun::TTRUE;
	}

	TSun::TBOOL GUITextureMgr::InitD3DObj(const TSun::TCHAR* effectFile)
	{
		// Init D3DObj
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// Init Shader
		// VS
		RenderDeviceCompiledShader* pCompiledVS = pDevice->createCompiledShader();
		if(!pCompiledVS->compileShader(effectFile,"VS","vs"))
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
		TLRenderDeviceInputElementDesc renderLI[3];
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
		renderLI[2].AlignedByteOffset = 20;
		renderLI[2].Format = RENDER_DEVICE_FORMAT_R32G32B32A32_FLOAT;
		renderLI[2].SemanticIndex = 0;
		renderLI[2].SemanticName = TSun::String("COLOR");
		renderLI[2].InputSlot = 0;
		mInputLayout = pDevice->createInputLayout(renderLI,3,pCompiledVS);
		if(!mInputLayout)
		{
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVS);
			return TSun::TFALSE;
		}
		T_DELETE(getRenderStructMemAllocator(), RenderDeviceCompiledShader, pCompiledVS);
		// PS
		RenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(effectFile,"PS","ps"))
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
		// VB
		GUI_VERTEX_DEF vertices[] =
		{
			{ TSun::Vector3<TSun::TF32>( -1.0f, 1.0f, 0.0f ), TSun::Vector2<TSun::TF32>( 0.0f, 0.0f ), TSun::Vector4<TSun::TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TSun::Vector3<TSun::TF32>( 1.0f, -1.0f, 0.0f ), TSun::Vector2<TSun::TF32>( 1.0f,1.0f ), TSun::Vector4<TSun::TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TSun::Vector3<TSun::TF32>( -1.0f, -1.0f, 0.0f ), TSun::Vector2<TSun::TF32>( 0.0f, 1.0f ), TSun::Vector4<TSun::TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TSun::Vector3<TSun::TF32>( -1.0f, 1.0f, 0.0f ), TSun::Vector2<TSun::TF32>( 0.0f, 0.0f ), TSun::Vector4<TSun::TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TSun::Vector3<TSun::TF32>( 1.0f, 1.0f, 0.0f ), TSun::Vector2<TSun::TF32>( 1.0f, 0.0f ), TSun::Vector4<TSun::TF32>(1.0f,1.0f,1.0f,1.0f) },
			{ TSun::Vector3<TSun::TF32>( 1.0f, -1.0f, 0.0f ), TSun::Vector2<TSun::TF32>( 1.0f, 1.0f ), TSun::Vector4<TSun::TF32>(1.0f,1.0f,1.0f,1.0f) },
		};
		TLRenderDeviceBufferDesc vbDesc;
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER;
		vbDesc.ByteWidth = sizeof(GUI_VERTEX_DEF) * 6;
		vbDesc.CPUAccessFlags = TSun::TFALSE;
		TLRenderDeviceSubresourceData initData;
		initData.pSysMem = vertices;
		mVB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mVB)
		{
			return TSun::TFALSE;
		}
		// VBSet
		vbDesc.BindFlags = 0;
		vbDesc.CPUAccessFlags = TSun::TTRUE;
		mVBSet = pDevice->createBuffer(&vbDesc,TSun::TNULL);
		// blend state
		TLRenderDeviceBlendDesc blendDesc;
		blendDesc.RenderTarget[0].BlendEnable = TSun::TTRUE;
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
		depthDesc.DepthEnable = TSun::TFALSE;
		depthDesc.DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ZERO;
		mDepthStencilState = pDevice->createDepthStencilState(&depthDesc);
		if(!mDepthStencilState)
		{
			return TSun::TFALSE;
		}
		// sampler state
		TLRenderDeviceSamplerDesc samplerDesc;
		samplerDesc.AddressU = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR;
		mSamplerState = pDevice->createSamplerState(&samplerDesc);
		if(!mSamplerState)
		{
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL GUITextureMgr::DrawGUICtrl(TSun::TS32 x, TSun::TS32 y, TSun::TS32 width, TSun::TS32 height, TSun::TF32 texX, TSun::TF32 texY, TSun::TF32 texR, TSun::TF32 texB, TSun::TS32 texId,TSun::TF32 alpha)
	{
		RenderDeviceUsedSRV* pSRV = TSun::TNULL;
		TSun::Map<TSun::TS32,RenderDeviceUsedSRV*>::Iterator itr = mSRVList.find(texId);
		if(itr!=mSRVList.end())
		{
			pSRV = itr->Value;
		}
		if (pSRV == TSun::TNULL)
		{
			return TSun::TFALSE;
		}
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		TSun::Vector4<TSun::TF32> color(1.0f,1.0f,1.0f,alpha);
		// 计算
		TSun::TF32 xFinal = -1.0f + ((TSun::TF32)(x))/m_bufferWidth * 2.0f;
		TSun::TF32 yFinal = (-1.0f + ((TSun::TF32)(y))/m_bufferHeight * 2.0f) * -1.0f;
		TSun::TF32 xLen = ((TSun::TF32)(width))/m_bufferWidth * 2.0f;
		TSun::TF32 yLen = ((TSun::TF32)(height))/m_bufferHeight * 2.0f;
		// vbSet
		GUI_VERTEX_DEF* pVertex;
		TLRenderDeviceMappedSubresource mappedRes;
		if(pDevice->mapResource(mVBSet,0,RENDER_DEVICE_MAP_READ_WRITE,&mappedRes))
		{
			pVertex = (GUI_VERTEX_DEF*)mappedRes.pData;
			pVertex[0].Pos = TSun::Vector3<TSun::TF32>(xFinal,yFinal,0);
			pVertex[0].Tex = TSun::Vector2<TSun::TF32>(texX,texY);
			pVertex[0].Color = color;
			pVertex[1].Pos = TSun::Vector3<TSun::TF32>(xFinal+xLen,yFinal-yLen,0);
			pVertex[1].Tex = TSun::Vector2<TSun::TF32>(texR,texB);
			pVertex[1].Color = color;
			pVertex[2].Pos = TSun::Vector3<TSun::TF32>(xFinal,yFinal-yLen,0);
			pVertex[2].Tex = TSun::Vector2<TSun::TF32>(texX,texB);
			pVertex[2].Color = color;
			pVertex[3].Pos = TSun::Vector3<TSun::TF32>(xFinal,yFinal,0);
			pVertex[3].Tex = TSun::Vector2<TSun::TF32>(texX,texY);
			pVertex[3].Color = color;
			pVertex[4].Pos = TSun::Vector3<TSun::TF32>(xFinal+xLen,yFinal,0);
			pVertex[4].Tex = TSun::Vector2<TSun::TF32>(texR,texY);
			pVertex[4].Color = color;
			pVertex[5].Pos = TSun::Vector3<TSun::TF32>(xFinal+xLen,yFinal-yLen,0);
			pVertex[5].Tex = TSun::Vector2<TSun::TF32>(texR,texB);
			pVertex[5].Color = color;
			pDevice->unmapResource(mVBSet,0);
			pDevice->copyResource(mVB,mVBSet);
		}

		// render
		pDevice->setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDevice->setInputLayout(mInputLayout);
		TSun::TU32 stride = sizeof(GUI_VERTEX_DEF);
		TSun::TU32 offset = 0;
		pDevice->setVertexBuffer(0,mVB,&stride,&offset);
		pDevice->setVertexShader(mVS);
		pDevice->setPixelShader(mPS);
		pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS,0,pSRV);
		pDevice->setSamplerState(RENDER_DEVICE_SHADER_USE_PS,0,mSamplerState);
		TSun::TF32 blendFactor[4] = {0,0,0,0};
		pDevice->setBlendState(mBlendState,blendFactor,0xffffffff);
		pDevice->setDepthStencilState(mDepthStencilState);
		pDevice->draw(6,0);
		return TSun::TTRUE;
	}

}