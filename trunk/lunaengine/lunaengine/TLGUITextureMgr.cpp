#include "StdAfx.h"
#include "TLGUITextureMgr.h"
#include "TString.h"
#include "TTxtFileReader.h"
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
using namespace TLunaEngine;

namespace TLunaEngine{
	T_SINGLETON_IMP(TLGUITextureMgr);

	TLGUITextureMgr::TLGUITextureMgr(void) : m_bufferHeight(0),m_bufferWidth(0),mVB(NULL),mVBSet(NULL),
		mVS(NULL),mPS(NULL),mInputLayout(NULL),mDepthStencilState(NULL),mBlendState(NULL),mSamplerState(NULL)
	{
	}

	TLGUITextureMgr::~TLGUITextureMgr(void)
	{
		m_bufferHeight = 0;
		m_bufferWidth = 0;
		SAFE_DELETE(mVB);
		SAFE_DELETE(mVBSet);
		SAFE_DELETE(mVS);
		SAFE_DELETE(mPS);
		SAFE_DELETE(mInputLayout);
		SAFE_DELETE(mDepthStencilState);
		SAFE_DELETE(mBlendState);
		SAFE_DELETE(mSamplerState);
		std::map<int,TLIRenderDeviceUsedSRV*>::iterator itr = mSRVList.begin();
		for(;itr!=mSRVList.end();++itr)
		{
			TLIRenderDeviceUsedSRV* pSRV = itr->second;
			SAFE_DELETE(pSRV);
			itr->second = NULL;
		}
		mSRVList.clear();
	}

	BOOL TLGUITextureMgr::Init(UINT bufferWidth, UINT bufferHeight, const char *effectFile)
	{
		m_bufferHeight = bufferHeight;
		m_bufferWidth = bufferWidth;
		if (!InitD3DObj(effectFile))
		{
			return FALSE;
		}
		return TRUE;
	}

	void TLGUITextureMgr::DestroyAllTex()
	{
		std::map<int,TLIRenderDeviceUsedSRV*>::iterator itr = mSRVList.begin();
		for(;itr!=mSRVList.end();++itr)
		{
			TLIRenderDeviceUsedSRV* pSRV = itr->second;
			SAFE_DELETE(pSRV);
			itr->second = NULL;
		}
		mSRVList.clear();
	}

	BOOL TLGUITextureMgr::LoadTexArray(FILE* stream)
	{
		int texID = -1;
		TTxtFileReader::ReadLineInteger(&texID,stream,1,' ');
		char strResult[1024] = {0};
		int nCount = 1024;
		TTxtFileReader::ReadLineString(strResult,stream,NULL,NULL,1024,NULL);
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		TString fullFile = TLGlobleClass::getSingletonPtr()->m_strResDir + strResult;
		TLImage* image = TLImage::createFromFile(fullFile.GetString());
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
		TLRenderDeviceSubresourceData initData;
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
		TLIRenderDeviceUsedSRV* pSRV = pDevice->createShaderResourceView(pTex,&srvDesc);
		if(!pSRV)
		{
			delete image;
			delete pTex;
			return FALSE;
		}
		delete image;
		delete pTex;
		mSRVList.insert(std::pair<int,TLIRenderDeviceUsedSRV*>(texID,pSRV));
		return TRUE;
	}

	BOOL TLGUITextureMgr::LoadTexFromFile(const char *file)
	{
		// 先检测，清空原来的
		DestroyAllTex();
		// 读取文件
		FILE* stream = NULL;
		TString strFile(file);
		if (!TTxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return FALSE;
		}
		// 匹配第一行字符
		bool bEqual = false;
		char strResult[1024] = {0};
		int nCount = 1024;
		if(!TTxtFileReader::ReadLineString(strResult,stream,"TUI_TEX_100",&bEqual,nCount,NULL))
		{
			TTxtFileReader::CloseTxtFile(stream);
			return FALSE;
		}
		if(!bEqual)
		{
			TTxtFileReader::CloseTxtFile(stream);
			return FALSE;
		}
		// 读取数量
		int texCount = 0;
		TTxtFileReader::ReadLineInteger(&texCount,stream,1,' ');
		// 循环读取
		for (int i=0;i<texCount;i++)
		{
			if(!LoadTexArray(stream))
			{
				TTxtFileReader::CloseTxtFile(stream);
				return FALSE;
			}
		}
		TTxtFileReader::CloseTxtFile(stream);
		return TRUE;
	}

	BOOL TLGUITextureMgr::InitD3DObj(const char* effectFile)
	{
		// Init D3DObj
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		// Init Shader
		// VS
		TLIRenderDeviceCompiledShader* pCompiledVS = pDevice->createCompiledShader();
		if(!pCompiledVS->compileShader(effectFile,"VS","vs"))
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
		TLRenderDeviceInputElementDesc renderLI[3];
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
		renderLI[2].AlignedByteOffset = 20;
		renderLI[2].Format = RENDER_DEVICE_FORMAT_R32G32B32A32_FLOAT;
		renderLI[2].SemanticIndex = 0;
		renderLI[2].SemanticName = TString("COLOR");
		renderLI[2].InputSlot = 0;
		mInputLayout = pDevice->createInputLayout(renderLI,3,pCompiledVS);
		if(!mInputLayout)
		{
			delete pCompiledVS;
			return FALSE;
		}
		delete pCompiledVS;
		// PS
		TLIRenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(effectFile,"PS","ps"))
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
		// VB
		_TLGUI_VERTEX_DEF vertices[] =
		{
			{ TLunaEngine::TVector3<float>( -1.0f, 1.0f, 0.0f ), TLunaEngine::TVector2<float>( 0.0f, 0.0f ), TLunaEngine::TVector4<float>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::TVector3<float>( 1.0f, -1.0f, 0.0f ), TLunaEngine::TVector2<float>( 1.0f,1.0f ), TLunaEngine::TVector4<float>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::TVector3<float>( -1.0f, -1.0f, 0.0f ), TLunaEngine::TVector2<float>( 0.0f, 1.0f ), TLunaEngine::TVector4<float>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::TVector3<float>( -1.0f, 1.0f, 0.0f ), TLunaEngine::TVector2<float>( 0.0f, 0.0f ), TLunaEngine::TVector4<float>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::TVector3<float>( 1.0f, 1.0f, 0.0f ), TLunaEngine::TVector2<float>( 1.0f, 0.0f ), TLunaEngine::TVector4<float>(1.0f,1.0f,1.0f,1.0f) },
			{ TLunaEngine::TVector3<float>( 1.0f, -1.0f, 0.0f ), TLunaEngine::TVector2<float>( 1.0f, 1.0f ), TLunaEngine::TVector4<float>(1.0f,1.0f,1.0f,1.0f) },
		};
		TLRenderDeviceBufferDesc vbDesc;
		vbDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER;
		vbDesc.ByteWidth = sizeof(_TLGUI_VERTEX_DEF) * 6;
		vbDesc.CPUAccessFlags = FALSE;
		TLRenderDeviceSubresourceData initData;
		initData.pSysMem = vertices;
		mVB = pDevice->createBuffer(&vbDesc,&initData);
		if(!mVB)
		{
			return FALSE;
		}
		// VBSet
		vbDesc.BindFlags = 0;
		vbDesc.CPUAccessFlags = TRUE;
		mVBSet = pDevice->createBuffer(&vbDesc,NULL);
		// blend state
		TLRenderDeviceBlendDesc blendDesc;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = RENDER_DEVICE_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = RENDER_DEVICE_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = RENDER_DEVICE_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = RENDER_DEVICE_BLEND_OP_ADD;
		mBlendState = pDevice->createBlendState(&blendDesc);
		if(!mBlendState)
		{
			return FALSE;
		}
		// depth stencil state
		TLRenderDeviceDepthStencilDesc depthDesc;
		depthDesc.DepthEnable = FALSE;
		depthDesc.DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ZERO;
		mDepthStencilState = pDevice->createDepthStencilState(&depthDesc);
		if(!mDepthStencilState)
		{
			return FALSE;
		}
		// sampler state
		TLRenderDeviceSamplerDesc samplerDesc;
		samplerDesc.AddressU = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR;
		mSamplerState = pDevice->createSamplerState(&samplerDesc);
		if(!mSamplerState)
		{
			return FALSE;
		}
		return TRUE;
	}

	bool TLGUITextureMgr::DrawGUICtrl(LONG x, LONG y, LONG width, LONG height, float texX, float texY, float texR, float texB, int texId,float alpha)
	{
		TLIRenderDeviceUsedSRV* pSRV = NULL;
		std::map<int,TLIRenderDeviceUsedSRV*>::iterator itr = mSRVList.find(texId);
		if(itr!=mSRVList.end())
		{
			pSRV = itr->second;
		}
		if (pSRV == NULL)
		{
			return false;
		}
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		TLunaEngine::TVector4<float> color(1.0f,1.0f,1.0f,alpha);
		// 计算
		float xFinal = -1.0f + ((float)(x))/m_bufferWidth * 2.0f;
		float yFinal = (-1.0f + ((float)(y))/m_bufferHeight * 2.0f) * -1.0f;
		float xLen = ((float)(width))/m_bufferWidth * 2.0f;
		float yLen = ((float)(height))/m_bufferHeight * 2.0f;
		// vbSet
		_TLGUI_VERTEX_DEF* pVertex;
		TLRenderDeviceMappedSubresource mappedRes;
		if(pDevice->mapResource(mVBSet,0,RENDER_DEVICE_MAP_READ_WRITE,&mappedRes))
		{
			pVertex = (_TLGUI_VERTEX_DEF*)mappedRes.pData;
			pVertex[0].Pos = TLunaEngine::TVector3<float>(xFinal,yFinal,0);
			pVertex[0].Tex = TLunaEngine::TVector2<float>(texX,texY);
			pVertex[0].Color = color;
			pVertex[1].Pos = TLunaEngine::TVector3<float>(xFinal+xLen,yFinal-yLen,0);
			pVertex[1].Tex = TLunaEngine::TVector2<float>(texR,texB);
			pVertex[1].Color = color;
			pVertex[2].Pos = TLunaEngine::TVector3<float>(xFinal,yFinal-yLen,0);
			pVertex[2].Tex = TLunaEngine::TVector2<float>(texX,texB);
			pVertex[2].Color = color;
			pVertex[3].Pos = TLunaEngine::TVector3<float>(xFinal,yFinal,0);
			pVertex[3].Tex = TLunaEngine::TVector2<float>(texX,texY);
			pVertex[3].Color = color;
			pVertex[4].Pos = TLunaEngine::TVector3<float>(xFinal+xLen,yFinal,0);
			pVertex[4].Tex = TLunaEngine::TVector2<float>(texR,texY);
			pVertex[4].Color = color;
			pVertex[5].Pos = TLunaEngine::TVector3<float>(xFinal+xLen,yFinal-yLen,0);
			pVertex[5].Tex = TLunaEngine::TVector2<float>(texR,texB);
			pVertex[5].Color = color;
			pDevice->unmapResource(mVBSet,0);
			pDevice->copyResource(mVB,mVBSet);
		}

		// render
		pDevice->setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDevice->setInputLayout(mInputLayout);
		UINT stride = sizeof(_TLGUI_VERTEX_DEF);
		UINT offset = 0;
		pDevice->setVertexBuffer(0,mVB,&stride,&offset);
		pDevice->setVertexShader(mVS);
		pDevice->setPixelShader(mPS);
		pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS,0,pSRV);
		pDevice->setSamplerState(RENDER_DEVICE_SHADER_USE_PS,0,mSamplerState);
		FLOAT blendFactor[4] = {0,0,0,0};
		pDevice->setBlendState(mBlendState,blendFactor,0xffffffff);
		pDevice->setDepthStencilState(mDepthStencilState);
		pDevice->draw(6,0);
		return true;
	}

}