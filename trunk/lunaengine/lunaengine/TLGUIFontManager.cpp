#include "StdAfx.h"
#include "TLGUIFontManager.h"
#include "TString.h"
#include "TLog.h"
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
using namespace TLunaEngine;

namespace TLunaEngine{
	T_SINGLETON_IMP(TLGUIFontManager);

	TLGUIFontManager::TLGUIFontManager(void) : 
	m_pUseFont(NULL),
	mVB(NULL),
	mVBSet(NULL),
	mVS(NULL),
	mPS(NULL),
	mInputLayout(NULL),
	mDepthStencilState(NULL),
	mBlendState(NULL),
	mSamplerState(NULL),
	m_bufferWidth(0),
	m_bufferHeight(0),
	m_pRenderText(NULL),
	m_nRenderTextLen(0),
	m_pDebugFont(0),
	library(0)
	{
	}

	TLGUIFontManager::~TLGUIFontManager(void)
	{
		SAFE_DELETE_ARRAY(m_pRenderText);
		m_pUseFont = NULL;
		std::map<int,TLGUIFont*>::iterator itr = m_FontTable.begin();
		for(;itr!=m_FontTable.end();itr++)
		{
			TLGUIFont* pObj = itr->second;
			SAFE_DELETE(pObj);
		}
		m_FontTable.clear();
		deleteDebugFont();
		FT_Done_FreeType(library);
		library = NULL;
		SAFE_DELETE(mVB);
		SAFE_DELETE(mVBSet);
		SAFE_DELETE(mVS);
		SAFE_DELETE(mPS);
		SAFE_DELETE(mInputLayout);
		SAFE_DELETE(mDepthStencilState);
		SAFE_DELETE(mBlendState);
		SAFE_DELETE(mSamplerState);
	}

	void TLGUIFontManager::DestroyAllFont()
	{
		m_pUseFont = NULL;
		std::map<int,TLGUIFont*>::iterator itr = m_FontTable.begin();
		for(;itr!=m_FontTable.end();itr++)
		{
			TLGUIFont* pObj = itr->second;
			SAFE_DELETE(pObj);
		}
		m_FontTable.clear();
	}

	int TLGUIFontManager::AddFont(const char* filename,UINT size,UINT texPageSize,int id)
	{
		TLGUIFont* font = new TLGUIFont();
		if(!font->InitFont(filename,size,texPageSize,id,library))
		{
			delete font;
			return -1;
		}
		m_FontTable.insert(std::pair<int,TLGUIFont*>(id,font));
		return m_FontTable.size() - 1;
	}

	bool TLGUIFontManager::AddFontFromFile(const char* filename)
	{
		// 先检测，清空原来的
		UseFont(-1);
		DestroyAllFont();
		// 读取文件
		FILE* stream = NULL;
		TString strFile(filename);
		if (!TTxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return false;
		}
		// 匹配第一行字符
		bool bEqual = false;
		char strResult[1024] = {0};
		int nCount = 1024;
		if(!TTxtFileReader::ReadLineString(strResult,stream,"TUI_FONT_100",&bEqual,nCount,NULL))
		{
			TTxtFileReader::CloseTxtFile(stream);
			return false;
		}
		if(!bEqual)
		{
			TTxtFileReader::CloseTxtFile(stream);
			return false;
		}
		// 读取数量
		int fontCount = 0;
		TTxtFileReader::ReadLineInteger(&fontCount,stream,1,' ');
		// 循环读取
		for (int i=0;i<fontCount;i++)
		{
			if(!LoadFont(stream))
			{
				TTxtFileReader::CloseTxtFile(stream);
				return false;
			}
		}
		TTxtFileReader::CloseTxtFile(stream);
		return true;
	}

	bool TLGUIFontManager::LoadFont(FILE* stream)
	{
		// FontID
		int fontID = -1;
		TTxtFileReader::ReadLineInteger(&fontID,stream,1,' ');
		// FontSize
		UINT fontSize = 0;
		TTxtFileReader::ReadLineUInteger(&fontSize,stream,1,' ');
		// texPageSize
		UINT texPageSize = 0;
		TTxtFileReader::ReadLineUInteger(&texPageSize,stream,1,' ');
		// FontFile
		char fontFile[1024] = {0};
		TTxtFileReader::ReadLineString(fontFile,stream,NULL,NULL,1024,NULL);
		// 加载
		TString fullFile = TLGlobleClass::getSingletonPtr()->m_strResDir + fontFile;
		if(AddFont(fullFile.GetString(),fontSize,texPageSize,fontID)==-1)
			return false;
		return true;
	}

	bool TLGUIFontManager::Render(const char* text,size_t len, int x, int y, TVector4<float>& color)
	{
		if(m_pUseFont==NULL)
			return false;
		if (text==NULL || len<=0)
		{
			return false;
		}
		TLIRenderDeviceUsedSRV* pSRV = m_pUseFont->getSRV(0);
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		// set device state
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
		// 分配渲染文字空间
		if ((int)len > m_nRenderTextLen || !m_pRenderText)
		{
			SAFE_DELETE_ARRAY(m_pRenderText);
			m_pRenderText = new wchar_t[len];
			m_nRenderTextLen = len;
		}
		::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,text,len,m_pRenderText,m_nRenderTextLen);
		wchar_t *wKeep = m_pRenderText;
		int X = x;
		int Y = y;
		float xPlus = 0;
		UINT n;
		UINT nowPageIndex=0;

		while(*wKeep)
		{
			bool catched = false;
			n = m_pUseFont->GetGlyphByChar(*wKeep,catched);
			if ( n > 0){
				INT imgw=0;
				INT imgh=0;
				INT texw=0;
				INT texh=0;
				INT offx=0;
				INT offy=0;
				FLOAT texStartU=0;
				FLOAT texEndU=0;
				FLOAT texStartV=0;
				FLOAT texEndV=0;
				UINT pageIndex;
				m_pUseFont->PreDraw(n,&imgw,&imgh,&texw,&texh,&offx,&offy,&texStartU,&texEndU,&texStartV,&texEndV,&pageIndex);

				float xFinal = -1.0f + ((float)(x+offx))/m_bufferWidth * 2.0f + xPlus;
				float yFinal = (-1.0f + ((float)(y+offy))/m_bufferHeight * 2.0f) * -1.0f;
				float xLen = ((float)(imgw-1))/m_bufferWidth * 2.0f;
				float yLen = ((float)(imgh-1))/m_bufferHeight * 2.0f;

				// vbSet
				_TLGUI_VERTEX_DEF* pVertex;
				TLRenderDeviceMappedSubresource mappedRes;
				if(pDevice->mapResource(mVBSet,0,RENDER_DEVICE_MAP_READ_WRITE,&mappedRes))
				{
					pVertex = (_TLGUI_VERTEX_DEF*)mappedRes.pData;
					pVertex[0].Pos = TLunaEngine::TVector3<float>(xFinal,yFinal,0);
					pVertex[0].Tex = TLunaEngine::TVector2<float>(texStartU,texStartV);
					pVertex[0].Color = color;
					pVertex[1].Pos = TLunaEngine::TVector3<float>(xFinal+xLen,yFinal-yLen,0);
					pVertex[1].Tex = TLunaEngine::TVector2<float>(texEndU,texEndV);
					pVertex[1].Color = color;
					pVertex[2].Pos = TLunaEngine::TVector3<float>(xFinal,yFinal-yLen,0);
					pVertex[2].Tex = TLunaEngine::TVector2<float>(texStartU,texEndV);
					pVertex[2].Color = color;
					pVertex[3].Pos = TLunaEngine::TVector3<float>(xFinal,yFinal,0);
					pVertex[3].Tex = TLunaEngine::TVector2<float>(texStartU,texStartV);
					pVertex[3].Color = color;
					pVertex[4].Pos = TLunaEngine::TVector3<float>(xFinal+xLen,yFinal,0);
					pVertex[4].Tex = TLunaEngine::TVector2<float>(texEndU,texStartV);
					pVertex[4].Color = color;
					pVertex[5].Pos = TLunaEngine::TVector3<float>(xFinal+xLen,yFinal-yLen,0);
					pVertex[5].Tex = TLunaEngine::TVector2<float>(texEndU,texEndV);
					pVertex[5].Color = color;
					pDevice->unmapResource(mVBSet,0);
					pDevice->copyResource(mVB,mVBSet);
				}
				// render
				if(catched || nowPageIndex!=pageIndex)
				{
					pSRV = m_pUseFont->getSRV(pageIndex);
					pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS,0,pSRV);
					nowPageIndex = pageIndex;
				}
				pDevice->draw(6,0);

				X += m_pUseFont->GetWidthFromCharacter(*wKeep);
				xPlus += ((float)(texw+offx))/m_bufferWidth * 2.0f;
			} else {
				X += m_pUseFont->GetWidthFromCharacter(*wKeep);
				TLunaEngine::TLog::WriteLine(TLunaEngine::TLog::LOG_LEVEL_ERROR,true,__FILE__,__LINE__,"Can not find char in font tex!");
			}

			++wKeep;
		}
		wKeep = NULL;

		return true;
	}

	bool TLGUIFontManager::Init(const char* effectFile,UINT bufferWidth,UINT bufferHeight)
	{
		m_bufferWidth = bufferWidth;
		m_bufferHeight = bufferHeight;
		// Init D3DObj
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		// Init Shader
		// VS
		TLIRenderDeviceCompiledShader* pCompiledVS = pDevice->createCompiledShader();
		if(!pCompiledVS->compileShader(effectFile,"VS","vs"))
		{
			delete pCompiledVS;
			return false;
		}
		mVS = pDevice->createVertexShader(pCompiledVS);
		if(!mVS)
		{
			delete pCompiledVS;
			return false;
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
			return false;
		}
		delete pCompiledVS;
		// PS
		TLIRenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(effectFile,"PS","ps"))
		{
			delete pCompiledPS;
			return false;
		}
		mPS = pDevice->createPixelShader(pCompiledPS);
		if(!mPS)
		{
			delete pCompiledPS;
			return false;
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
			return false;
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
			return false;
		}
		// depth stencil state
		TLRenderDeviceDepthStencilDesc depthDesc;
		depthDesc.DepthEnable = FALSE;
		depthDesc.DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ZERO;
		mDepthStencilState = pDevice->createDepthStencilState(&depthDesc);
		if(!mDepthStencilState)
		{
			return false;
		}
		// sampler state
		TLRenderDeviceSamplerDesc samplerDesc;
		samplerDesc.AddressU = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = RENDER_DEVICE_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR;
		mSamplerState = pDevice->createSamplerState(&samplerDesc);
		if(!mSamplerState)
		{
			return false;
		}
		// Init FreeType
		if(FT_Init_FreeType(&library))
		{
			return false;
		}
		return true;
	}

	bool TLGUIFontManager::initDebugFont(const char* filename, UINT size, UINT texPageSize)
	{
		if(m_pDebugFont!=NULL)
		{
			return false;
		}
		m_pDebugFont = new TLGUIFont();
		if(!m_pDebugFont->InitFont(filename,size,texPageSize,-1,library))
		{
			delete m_pDebugFont;
			m_pDebugFont = NULL;
			return false;
		}
		return true;
	}

	void TLGUIFontManager::deleteDebugFont()
	{
		if(m_pDebugFont!=NULL)
		{
			delete m_pDebugFont;
			m_pDebugFont = NULL;
		}
	}

	bool TLGUIFontManager::RenderDebugFont(const char* text, size_t len, int x, int y, TVector4<float>& color)
	{
		if(!m_pDebugFont)
		{
			return false;
		}
		TLGUIFont* pFont = m_pUseFont;
		m_pUseFont = m_pDebugFont;
		bool ret = Render(text,len,x,y,color);
		m_pUseFont = pFont;
		return ret;
	}
}