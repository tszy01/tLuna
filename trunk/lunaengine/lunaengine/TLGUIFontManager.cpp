#include "TLGUIFontManager.h"
#include "TSString.h"
#include "TSLog.h"
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

TLunaEngine::GUIFontManager* TSun::Singleton<TLunaEngine::GUIFontManager>::m_Ptr = 0;

namespace TLunaEngine{
	GUIFontManager::GUIFontManager(TSun::TVOID) : 
	m_pUseFont(TSun::TNULL),
	mVB(TSun::TNULL),
	mVBSet(TSun::TNULL),
	mVS(TSun::TNULL),
	mPS(TSun::TNULL),
	mInputLayout(TSun::TNULL),
	mDepthStencilState(TSun::TNULL),
	mBlendState(TSun::TNULL),
	mSamplerState(TSun::TNULL),
	m_bufferWidth(0),
	m_bufferHeight(0),
	m_pRenderText(TSun::TNULL),
	m_nRenderTextLen(0),
	m_pDebugFont(0),
	library(0)
	{
	}

	GUIFontManager::~GUIFontManager(TSun::TVOID)
	{
		if (m_pRenderText)
		{
			delete[] m_pRenderText;
			m_pRenderText = 0;
		}
		m_pUseFont = TSun::TNULL;
		TSun::Map<TSun::TS32,GUIFont*>::Iterator itr = m_FontTable.begin();
		for(;itr!=m_FontTable.end();itr++)
		{
			GUIFont* pObj = itr->Value;
			if (pObj)
			{
				delete pObj;
				pObj = 0;
			}
			itr->Value = TSun::TNULL;
		}
		m_FontTable.clear();
		deleteDebugFont();
		FT_Done_FreeType(library);
		library = TSun::TNULL;
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

	TSun::TVOID GUIFontManager::DestroyAllFont()
	{
		m_pUseFont = TSun::TNULL;
		TSun::Map<TSun::TS32,GUIFont*>::Iterator itr = m_FontTable.begin();
		for(;itr!=m_FontTable.end();itr++)
		{
			GUIFont* pObj = itr->Value;
			if (pObj)
			{
				delete pObj;
				pObj = 0;
			}
			itr->Value = TSun::TNULL;
		}
		m_FontTable.clear();
	}

	TSun::TU32 GUIFontManager::AddFont(const TSun::TCHAR* filename,TSun::TU32 size,TSun::TU32 texPageSize,TSun::TS32 id)
	{
		GUIFont* font = new GUIFont();
		if(!font->InitFont(filename,size,texPageSize,id,library))
		{
			delete font;
			return -1;
		}
		m_FontTable.push_back(id,font);
		return (TSun::TU32)m_FontTable.size() - 1;
	}

	TSun::TBOOL GUIFontManager::AddFontFromFile(const TSun::TCHAR* filename)
	{
		// 先检测，清空原来的
		UseFont(-1);
		DestroyAllFont();
		// 读取文件
		FILE* stream = TSun::TNULL;
		TSun::String strFile(filename);
		if (!TSun::TxtFileReader::OpenTxtFile(strFile.GetString(),&stream))
		{
			return TSun::TFALSE;
		}
		// 匹配第一行字符
		TSun::TBOOL bEqual = TSun::TFALSE;
		TSun::TCHAR strResult[1024] = {0};
		TSun::TS32 nCount = 1024;
		if(!TSun::TxtFileReader::ReadLineString(strResult,stream,"TUI_FONT_100",&bEqual,nCount,TSun::TNULL))
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
		TSun::TS32 fontCount = 0;
		TSun::TxtFileReader::ReadLineInteger(&fontCount,stream,1,' ');
		// 循环读取
		for (TSun::TS32 i=0;i<fontCount;i++)
		{
			if(!LoadFont(stream))
			{
				TSun::TxtFileReader::CloseTxtFile(stream);
				return TSun::TFALSE;
			}
		}
		TSun::TxtFileReader::CloseTxtFile(stream);
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIFontManager::LoadFont(FILE* stream)
	{
		// FontID
		TSun::TS32 fontID = -1;
		TSun::TxtFileReader::ReadLineInteger(&fontID,stream,1,' ');
		// FontSize
		TSun::TU32 fontSize = 0;
		TSun::TxtFileReader::ReadLineUInteger(&fontSize,stream,1,' ');
		// texPageSize
		TSun::TU32 texPageSize = 0;
		TSun::TxtFileReader::ReadLineUInteger(&texPageSize,stream,1,' ');
		// FontFile
		TSun::TCHAR fontFile[1024] = {0};
		TSun::TxtFileReader::ReadLineString(fontFile,stream,TSun::TNULL,TSun::TNULL,1024,TSun::TNULL);
		// 加载
		TSun::String fullFile = GlobleClass::getSingletonPtr()->m_strResDir + fontFile;
		if(AddFont(fullFile.GetString(),fontSize,texPageSize,fontID)==-1)
			return TSun::TFALSE;
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIFontManager::Render(const TSun::TWCHAR* text,TSun::TU32 len, TSun::TS32 x, TSun::TS32 y, TSun::Vector4<TSun::TF32>& color)
	{
		if(m_pUseFont==TSun::TNULL)
			return TSun::TFALSE;
		if (text==TSun::TNULL || len<=0)
		{
			return TSun::TFALSE;
		}
		RenderDeviceUsedSRV* pSRV = m_pUseFont->getSRV(0);
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		// set device state
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
		// 分配渲染文字空间
		if (len > m_nRenderTextLen || !m_pRenderText)
		{
			if (m_pRenderText)
			{
				delete[] m_pRenderText;
				m_pRenderText = 0;
			}
			m_pRenderText = new TSun::TWCHAR[len];
			m_nRenderTextLen = len;
		}
		//::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,text,(TSun::TS32)len,m_pRenderText,(TSun::TS32)m_nRenderTextLen);
		memcpy(m_pRenderText, text, len * sizeof(TSun::TWCHAR));
		TSun::TWCHAR *wKeep = m_pRenderText;
		TSun::TS32 X = x;
		TSun::TS32 Y = y;
		TSun::TF32 xPlus = 0;
		TSun::TF32 yPlus = 0;
		TSun::TU32 n;
		TSun::TU32 nowPageIndex=0;

		while(*wKeep)
		{
			if ((*wKeep) == L' ')
			{
				TSun::TU32 left=0, top=0, right=0, bottom=0;
				TSun::TWCHAR c[2] = { (*wKeep),0 };
				m_pUseFont->GetDimension(c, left, right, top, bottom);
				xPlus += ((TSun::TF32)(right - left)) / m_bufferWidth * 2.0f;
				X += right - left;
			}
			else if ((*wKeep) == L'\n')
			{
				TSun::TU32 left = 0, top = 0, right = 0, bottom = 0;
				TSun::TWCHAR c[2] = { (*wKeep),0 };
				m_pUseFont->GetDimension(c, left, right, top, bottom);
				yPlus += ((TSun::TF32)(bottom - top)) / m_bufferHeight * 2.0f;
				Y += bottom - top;
				xPlus = 0;
				X = 0;
			}
			else
			{
				TSun::TBOOL catched = TSun::TFALSE;
				n = m_pUseFont->GetGlyphByChar(*wKeep, catched);
				if (n > 0) {
					TSun::TS32 imgw = 0;
					TSun::TS32 imgh = 0;
					TSun::TS32 texw = 0;
					TSun::TS32 texh = 0;
					TSun::TS32 offx = 0;
					TSun::TS32 offy = 0;
					TSun::TF32 texStartU = 0;
					TSun::TF32 texEndU = 0;
					TSun::TF32 texStartV = 0;
					TSun::TF32 texEndV = 0;
					TSun::TU32 pageIndex;
					m_pUseFont->PreDraw(n, &imgw, &imgh, &texw, &texh, &offx, &offy, &texStartU, &texEndU, &texStartV, &texEndV, &pageIndex);

					TSun::TF32 xFinal = -1.0f + ((TSun::TF32)(x + offx)) / m_bufferWidth * 2.0f + xPlus;
					TSun::TF32 yFinal = (-1.0f + ((TSun::TF32)(y + offy)) / m_bufferHeight * 2.0f) * -1.0f - yPlus;
					TSun::TF32 xLen = ((TSun::TF32)(imgw - 1)) / m_bufferWidth * 2.0f;
					TSun::TF32 yLen = ((TSun::TF32)(imgh - 1)) / m_bufferHeight * 2.0f;

					// vbSet
					GUI_VERTEX_DEF* pVertex;
					TLRenderDeviceMappedSubresource mappedRes;
					if (pDevice->mapResource(mVBSet, 0, RENDER_DEVICE_MAP_READ_WRITE, &mappedRes))
					{
						pVertex = (GUI_VERTEX_DEF*)mappedRes.pData;
						pVertex[0].Pos = TSun::Vector3<TSun::TF32>(xFinal, yFinal, 0);
						pVertex[0].Tex = TSun::Vector2<TSun::TF32>(texStartU, texStartV);
						pVertex[0].Color = color;
						pVertex[1].Pos = TSun::Vector3<TSun::TF32>(xFinal + xLen, yFinal - yLen, 0);
						pVertex[1].Tex = TSun::Vector2<TSun::TF32>(texEndU, texEndV);
						pVertex[1].Color = color;
						pVertex[2].Pos = TSun::Vector3<TSun::TF32>(xFinal, yFinal - yLen, 0);
						pVertex[2].Tex = TSun::Vector2<TSun::TF32>(texStartU, texEndV);
						pVertex[2].Color = color;
						pVertex[3].Pos = TSun::Vector3<TSun::TF32>(xFinal, yFinal, 0);
						pVertex[3].Tex = TSun::Vector2<TSun::TF32>(texStartU, texStartV);
						pVertex[3].Color = color;
						pVertex[4].Pos = TSun::Vector3<TSun::TF32>(xFinal + xLen, yFinal, 0);
						pVertex[4].Tex = TSun::Vector2<TSun::TF32>(texEndU, texStartV);
						pVertex[4].Color = color;
						pVertex[5].Pos = TSun::Vector3<TSun::TF32>(xFinal + xLen, yFinal - yLen, 0);
						pVertex[5].Tex = TSun::Vector2<TSun::TF32>(texEndU, texEndV);
						pVertex[5].Color = color;
						pDevice->unmapResource(mVBSet, 0);
						pDevice->copyResource(mVB, mVBSet);
					}
					// render
					if (catched || nowPageIndex != pageIndex)
					{
						pSRV = m_pUseFont->getSRV(pageIndex);
						pDevice->setShaderResourceView(RENDER_DEVICE_SHADER_USE_PS, 0, pSRV);
						nowPageIndex = pageIndex;
					}
					pDevice->draw(6, 0);

					X += m_pUseFont->GetWidthFromCharacter(*wKeep);
					xPlus += ((TSun::TF32)(texw + offx)) / m_bufferWidth * 2.0f;
				}
				else {
					X += m_pUseFont->GetWidthFromCharacter(*wKeep);
					TSun::Log::WriteLine(TSun::Log::LOG_LEVEL_ERROR, TSun::TTRUE, L"Can not find character in font tex!", __FILE__, __LINE__);
				}
			}
			++wKeep;
		}
		wKeep = TSun::TNULL;

		return TSun::TTRUE;
	}

	TSun::TBOOL GUIFontManager::Init(const TSun::TCHAR* effectFile,TSun::TU32 bufferWidth,TSun::TU32 bufferHeight)
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
			return TSun::TFALSE;
		}
		mVS = pDevice->createVertexShader(pCompiledVS);
		if(!mVS)
		{
			delete pCompiledVS;
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
			delete pCompiledVS;
			return TSun::TFALSE;
		}
		delete pCompiledVS;
		// PS
		RenderDeviceCompiledShader* pCompiledPS = pDevice->createCompiledShader();
		if(!pCompiledPS->compileShader(effectFile,"PS","ps"))
		{
			delete pCompiledPS;
			return TSun::TFALSE;
		}
		mPS = pDevice->createPixelShader(pCompiledPS);
		if(!mPS)
		{
			delete pCompiledPS;
			return TSun::TFALSE;
		}
		delete pCompiledPS;
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
		// Init FreeType
		if(FT_Init_FreeType(&library))
		{
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIFontManager::initDebugFont(const TSun::TCHAR* filename, TSun::TU32 size, TSun::TU32 texPageSize)
	{
		if(m_pDebugFont!=TSun::TNULL)
		{
			return TSun::TFALSE;
		}
		m_pDebugFont = new GUIFont();
		if(!m_pDebugFont->InitFont(filename,size,texPageSize,-1,library))
		{
			delete m_pDebugFont;
			m_pDebugFont = TSun::TNULL;
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

	TSun::TVOID GUIFontManager::deleteDebugFont()
	{
		if(m_pDebugFont!=TSun::TNULL)
		{
			delete m_pDebugFont;
			m_pDebugFont = TSun::TNULL;
		}
	}

	TSun::TBOOL GUIFontManager::RenderDebugFont(const TSun::TWCHAR* text, TSun::TU32 len, TSun::TS32 x, TSun::TS32 y, TSun::Vector4<TSun::TF32>& color)
	{
		if(!m_pDebugFont)
		{
			return TSun::TFALSE;
		}
		GUIFont* pFont = m_pUseFont;
		m_pUseFont = m_pDebugFont;
		TSun::TBOOL ret = Render(text,len,x,y,color);
		m_pUseFont = pFont;
		return ret;
	}
}