#include "TLGUIFontManager.h"
#include "TLString.h"
#include "TLLog.h"
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

namespace TLunaEngine{
	GUIFontManager* Singleton<GUIFontManager>::m_Ptr = 0;

	GUIFontManager::GUIFontManager(TVOID) : 
	m_pUseFont(TNULL),
	mVB(TNULL),
	mVBSet(TNULL),
	mVS(TNULL),
	mPS(TNULL),
	mInputLayout(TNULL),
	mDepthStencilState(TNULL),
	mBlendState(TNULL),
	mSamplerState(TNULL),
	m_bufferWidth(0),
	m_bufferHeight(0),
	m_pRenderText(TNULL),
	m_nRenderTextLen(0),
	m_pDebugFont(0),
	library(0)
	{
	}

	GUIFontManager::~GUIFontManager(TVOID)
	{
		if (m_pRenderText)
		{
			delete[] m_pRenderText;
			m_pRenderText = 0;
		}
		m_pUseFont = TNULL;
		Map<TS32,GUIFont*>::Iterator itr = m_FontTable.begin();
		for(;itr!=m_FontTable.end();itr++)
		{
			GUIFont* pObj = itr->Value;
			if (pObj)
			{
				delete pObj;
				pObj = 0;
			}
			itr->Value = TNULL;
		}
		m_FontTable.clear();
		deleteDebugFont();
		FT_Done_FreeType(library);
		library = TNULL;
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
	}

	TVOID GUIFontManager::DestroyAllFont()
	{
		m_pUseFont = TNULL;
		Map<TS32,GUIFont*>::Iterator itr = m_FontTable.begin();
		for(;itr!=m_FontTable.end();itr++)
		{
			GUIFont* pObj = itr->Value;
			if (pObj)
			{
				delete pObj;
				pObj = 0;
			}
			itr->Value = TNULL;
		}
		m_FontTable.clear();
	}

	TU32 GUIFontManager::AddFont(const TCHAR* filename,TU32 size,TU32 texPageSize,TS32 id)
	{
		GUIFont* font = new GUIFont();
		if(!font->InitFont(filename,size,texPageSize,id,library))
		{
			delete font;
			return -1;
		}
		m_FontTable.push_back(id,font);
		return (TU32)m_FontTable.size() - 1;
	}

	TBOOL GUIFontManager::AddFontFromFile(const TCHAR* filename)
	{
		// 先检测，清空原来的
		UseFont(-1);
		DestroyAllFont();
		// 读取文件
		FILE* stream = TNULL;
		String strFile(filename);
		if (!TxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return TFALSE;
		}
		// 匹配第一行字符
		TBOOL bEqual = TFALSE;
		TCHAR strResult[1024] = {0};
		TS32 nCount = 1024;
		if(!TxtFileReader::ReadLineString(strResult,stream,"TUI_FONT_100",&bEqual,nCount,TNULL))
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
		TS32 fontCount = 0;
		TxtFileReader::ReadLineInteger(&fontCount,stream,1,' ');
		// 循环读取
		for (TS32 i=0;i<fontCount;i++)
		{
			if(!LoadFont(stream))
			{
				TxtFileReader::CloseTxtFile(stream);
				return TFALSE;
			}
		}
		TxtFileReader::CloseTxtFile(stream);
		return TTRUE;
	}

	TBOOL GUIFontManager::LoadFont(FILE* stream)
	{
		// FontID
		TS32 fontID = -1;
		TxtFileReader::ReadLineInteger(&fontID,stream,1,' ');
		// FontSize
		TU32 fontSize = 0;
		TxtFileReader::ReadLineUInteger(&fontSize,stream,1,' ');
		// texPageSize
		TU32 texPageSize = 0;
		TxtFileReader::ReadLineUInteger(&texPageSize,stream,1,' ');
		// FontFile
		TCHAR fontFile[1024] = {0};
		TxtFileReader::ReadLineString(fontFile,stream,TNULL,TNULL,1024,TNULL);
		// 加载
		String fullFile = GlobleClass::getSingletonPtr()->m_strResDir + fontFile;
		if(AddFont(fullFile.GetString(),fontSize,texPageSize,fontID)==-1)
			return TFALSE;
		return TTRUE;
	}

	TBOOL GUIFontManager::Render(const TWCHAR* text,TU32 len, TS32 x, TS32 y, Vector4<TF32>& color)
	{
		if(m_pUseFont==TNULL)
			return TFALSE;
		if (text==TNULL || len<=0)
		{
			return TFALSE;
		}
		RenderDeviceUsedSRV* pSRV = m_pUseFont->getSRV(0);
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// set device state
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
		// 分配渲染文字空间
		if (len > m_nRenderTextLen || !m_pRenderText)
		{
			if (m_pRenderText)
			{
				delete[] m_pRenderText;
				m_pRenderText = 0;
			}
			m_pRenderText = new TWCHAR[len];
			m_nRenderTextLen = len;
		}
		//::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,text,(TS32)len,m_pRenderText,(TS32)m_nRenderTextLen);
		memcpy(m_pRenderText, text, len * sizeof(TWCHAR));
		TWCHAR *wKeep = m_pRenderText;
		TS32 X = x;
		TS32 Y = y;
		TF32 xPlus = 0;
		TU32 n;
		TU32 nowPageIndex=0;

		while(*wKeep)
		{
			TBOOL catched = TFALSE;
			n = m_pUseFont->GetGlyphByChar(*wKeep,catched);
			if ( n > 0){
				TS32 imgw=0;
				TS32 imgh=0;
				TS32 texw=0;
				TS32 texh=0;
				TS32 offx=0;
				TS32 offy=0;
				TF32 texStartU=0;
				TF32 texEndU=0;
				TF32 texStartV=0;
				TF32 texEndV=0;
				TU32 pageIndex;
				m_pUseFont->PreDraw(n,&imgw,&imgh,&texw,&texh,&offx,&offy,&texStartU,&texEndU,&texStartV,&texEndV,&pageIndex);

				TF32 xFinal = -1.0f + ((TF32)(x+offx))/m_bufferWidth * 2.0f + xPlus;
				TF32 yFinal = (-1.0f + ((TF32)(y+offy))/m_bufferHeight * 2.0f) * -1.0f;
				TF32 xLen = ((TF32)(imgw-1))/m_bufferWidth * 2.0f;
				TF32 yLen = ((TF32)(imgh-1))/m_bufferHeight * 2.0f;

				// vbSet
				GUI_VERTEX_DEF* pVertex;
				TLRenderDeviceMappedSubresource mappedRes;
				if(pDevice->mapResource(mVBSet,0,RENDER_DEVICE_MAP_READ_WRITE,&mappedRes))
				{
					pVertex = (GUI_VERTEX_DEF*)mappedRes.pData;
					pVertex[0].Pos = TLunaEngine::Vector3<TF32>(xFinal,yFinal,0);
					pVertex[0].Tex = TLunaEngine::Vector2<TF32>(texStartU,texStartV);
					pVertex[0].Color = color;
					pVertex[1].Pos = TLunaEngine::Vector3<TF32>(xFinal+xLen,yFinal-yLen,0);
					pVertex[1].Tex = TLunaEngine::Vector2<TF32>(texEndU,texEndV);
					pVertex[1].Color = color;
					pVertex[2].Pos = TLunaEngine::Vector3<TF32>(xFinal,yFinal-yLen,0);
					pVertex[2].Tex = TLunaEngine::Vector2<TF32>(texStartU,texEndV);
					pVertex[2].Color = color;
					pVertex[3].Pos = TLunaEngine::Vector3<TF32>(xFinal,yFinal,0);
					pVertex[3].Tex = TLunaEngine::Vector2<TF32>(texStartU,texStartV);
					pVertex[3].Color = color;
					pVertex[4].Pos = TLunaEngine::Vector3<TF32>(xFinal+xLen,yFinal,0);
					pVertex[4].Tex = TLunaEngine::Vector2<TF32>(texEndU,texStartV);
					pVertex[4].Color = color;
					pVertex[5].Pos = TLunaEngine::Vector3<TF32>(xFinal+xLen,yFinal-yLen,0);
					pVertex[5].Tex = TLunaEngine::Vector2<TF32>(texEndU,texEndV);
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
				xPlus += ((TF32)(texw+offx))/m_bufferWidth * 2.0f;
			} else {
				X += m_pUseFont->GetWidthFromCharacter(*wKeep);
				TLunaEngine::Log::WriteLine(TLunaEngine::Log::LOG_LEVEL_ERROR,TTRUE,"Can not find character in font tex!",__FILE__,__LINE__);
			}

			++wKeep;
		}
		wKeep = TNULL;

		return TTRUE;
	}

	TBOOL GUIFontManager::Init(const TCHAR* effectFile,TU32 bufferWidth,TU32 bufferHeight)
	{
		m_bufferWidth = bufferWidth;
		m_bufferHeight = bufferHeight;
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
		// Init FreeType
		if(FT_Init_FreeType(&library))
		{
			return TFALSE;
		}
		return TTRUE;
	}

	TBOOL GUIFontManager::initDebugFont(const TCHAR* filename, TU32 size, TU32 texPageSize)
	{
		if(m_pDebugFont!=TNULL)
		{
			return TFALSE;
		}
		m_pDebugFont = new GUIFont();
		if(!m_pDebugFont->InitFont(filename,size,texPageSize,-1,library))
		{
			delete m_pDebugFont;
			m_pDebugFont = TNULL;
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID GUIFontManager::deleteDebugFont()
	{
		if(m_pDebugFont!=TNULL)
		{
			delete m_pDebugFont;
			m_pDebugFont = TNULL;
		}
	}

	TBOOL GUIFontManager::RenderDebugFont(const TWCHAR* text, TU32 len, TS32 x, TS32 y, Vector4<TF32>& color)
	{
		if(!m_pDebugFont)
		{
			return TFALSE;
		}
		GUIFont* pFont = m_pUseFont;
		m_pUseFont = m_pDebugFont;
		TBOOL ret = Render(text,len,x,y,color);
		m_pUseFont = pFont;
		return ret;
	}
}