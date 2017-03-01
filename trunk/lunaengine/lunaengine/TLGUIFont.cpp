#include "TLGUIFont.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice.h"
#include "TLRenderDeviceUsedTex2D.h"
#include "TLRenderDeviceUsedSRV.h"
#include "TSMap.h"
#include "TLMemDef.h"

namespace TLunaEngine{

	// ------------------ FontGlyph ----------------------
	FontGlyph::FontGlyph() : texd(TSun::TNULL),pageIndex(-1)
	{
	}

	FontGlyph::~FontGlyph()
	{
		if(texd)
		{
			T_DELETE_ARRAY(getEngineStructMemAllocator(), TSun::TU32, texd);
			texd = TSun::TNULL;
		}
	}

	TSun::TVOID FontGlyph::cache(TSun::TU32 idx,TSun::TU32 size)
	{
		if(cached || texd)
			return;
		FT_Set_Pixel_Sizes(*face,0,size);
		if (!FT_Load_Glyph(*face,idx,FT_LOAD_NO_HINTING|FT_LOAD_NO_BITMAP)){
			FT_GlyphSlot glyph = (*face)->glyph;
			FT_Bitmap  bits;
			if (glyph->format == ft_glyph_format_outline ){
				if (!FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL)){
					bits = glyph->bitmap;
					TSun::TUByte *pt = bits.buffer;
					top = glyph->bitmap_top;
					left = glyph->bitmap_left;
					imgw = 1;
					imgh = 1;
					texw = bits.width;
					texh = bits.rows;
					for(;;){
						if (imgw > texw){
							break;
						} else {
							imgw <<= 1;
						}
					}
					for(;;){
						if (imgh > texh){
							break;
						} else {
							imgh <<= 1;
						}
					}
					if (imgw > imgh){
						imgh = imgw;
					} else {
						imgw = imgh;
					}
					texd = T_NEW_ARRAY(getEngineStructMemAllocator(), TSun::TU32, imgw*imgh);
					memset(texd,0,imgw*imgh*sizeof(TSun::TU32));
					TSun::TU32 *texp = texd;
					offset = size - bits.rows;
					TSun::TBOOL cflag = TSun::TFALSE;
					for (TSun::TS32 i = 0;i < bits.rows;i++){
						TSun::TU32 *rowp = texp;
						for (TSun::TS32 j = 0;j < bits.width;j++){
							if (*pt){
								if (cflag){
									*rowp = *pt;
									*rowp *= 0x01010101;
								} else {
									*rowp = *pt << 24;
									*rowp |= 0xffffff;
								}
							} else {
								*rowp = 0;
							}
							pt++;
							rowp++;
						}
						texp += imgw;
					}
					cached = TSun::TTRUE;
				}
			}
		}
	}

	// ------------------ GUIFont ------------------------
	GUIFont::GUIFont(TSun::TVOID) : 
	m_Glyphs(TSun::TNULL),
	face(TSun::TNULL),
	library(TSun::TNULL),
	m_Id(-1),
	mFontSize(0),
	mPageCount(0),
	mPageSize(512)
	{
	}

	GUIFont::~GUIFont(TSun::TVOID)
	{
		TSun::List<TSun::TU32*>::Iterator itrBuffer = mPageBufferList.begin();
		for(;itrBuffer!=mPageBufferList.end();++itrBuffer)
		{
			TSun::TU32* del = *itrBuffer;
			if (del)
			{
				T_DELETE_ARRAY(getEngineStructMemAllocator(), TSun::TU32, del);
				(*itrBuffer) = 0;
			}
		}
		mPageBufferList.clear();
		TSun::List<RenderDeviceUsedSRV*>::Iterator itrSRV = mSRVList.begin();
		for(;itrSRV!=mSRVList.end();++itrSRV)
		{
			RenderDeviceUsedSRV* del = *itrSRV;
			if (del)
			{
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSRV, del);
				del = 0;
			}
		}
		mSRVList.clear();
		mPageCount=0;
		if (m_Glyphs)
		{
			T_DELETE_ARRAY(getEngineStructMemAllocator(), FontGlyph, m_Glyphs);
			m_Glyphs = TSun::TNULL;
		}
		FT_Done_Face(face);
		face = TSun::TNULL;
		library = TSun::TNULL;
	}

	TSun::TBOOL GUIFont::InitFont(const TSun::TCHAR* filename, TSun::TU32 size,TSun::TU32 texPageSize,TSun::TS32 id,FT_Library lib)
	{
		if(filename==TSun::TNULL || lib==TSun::TNULL || texPageSize<=0 || size<=0)
		{
			return TSun::TFALSE;
		}
		// Load Freetype
		library = lib;
		if (FT_New_Face( library,filename,0,&face )){
			return	TSun::TFALSE;
		}
		// 创建文字个体
		m_Glyphs = T_NEW_ARRAY(getEngineStructMemAllocator(), FontGlyph, face->num_glyphs);
		for (TSun::TS32 i = 0;i < face->num_glyphs;i++){
			m_Glyphs[i].face = &face;
			m_Glyphs[i].cached = TSun::TFALSE;
		}
		m_Id = id;
		mFontSize = size;
		mPageSize = texPageSize;
		return TSun::TTRUE;
	}

	TSun::TU32 GUIFont::GetGlyphByChar(TSun::TWCHAR c,TSun::TBOOL& newFontCached){
		newFontCached = TSun::TFALSE;
		TSun::TU32 idx = FT_Get_Char_Index( face, c );
		if (idx && !m_Glyphs[idx - 1].cached)
		{
			m_Glyphs[idx - 1].cache(idx,mFontSize);
			catchAllFont();
			newFontCached = TSun::TTRUE;
		}
		return	idx;
	}

	TSun::TBOOL GUIFont::catchAllFont()
	{
		if(mPageSize==0)
			return TSun::TFALSE;
		// 摆放规则
		// 从大到小，先摆放大的，然后摆放小的，竖着摆，一列为一行
		// 第一遍存放到map中
		TSun::Map<TSun::TU32,TSun::List<TSun::TS32>> texHeightMap;
		TSun::TU32 maxHeight=0,minHeight=0,count=0;
		for (TSun::TS32 i = 0;i < face->num_glyphs;i++){
			if(m_Glyphs[i].cached)
			{
				texHeightMap[m_Glyphs[i].imgh].push_back(i);
				if(count==0)
				{
					minHeight = m_Glyphs[i].imgh;
				}
				else if(minHeight > m_Glyphs[i].imgh)
				{
					minHeight = m_Glyphs[i].imgh;
				}
				if(maxHeight < m_Glyphs[i].imgh)
				{
					maxHeight = m_Glyphs[i].imgh;
				}
				count++;
			}
		}
		if(maxHeight==0 || minHeight==0 || count==0)
			return TSun::TFALSE;
		// 计算buffer的大小
		TSun::TU32 totalHeight=0,nowHeight=maxHeight;
		TSun::TU32 fontCountPerLine=0;
		TSun::TU32 nextWidth = 0;
		while(nowHeight>=minHeight)
		{
			TSun::List<TSun::TS32> valueList;
			if(texHeightMap.get(nowHeight, valueList) == TSun::TTRUE)
			{
				if(valueList.size()>0)
				{
					if(nowHeight==maxHeight)
					{
						TSun::TU32 maxCount = mPageSize / nowHeight;
						totalHeight = nowHeight * ((TSun::TU32)valueList.size() / maxCount);
						nextWidth = valueList.size() % maxCount * nowHeight;
						fontCountPerLine = 1;
					}
					else
					{
						TSun::TU32 lineCount = (TSun::TU32)valueList.size() / fontCountPerLine;
						if((TSun::TU32)valueList.size() % fontCountPerLine > 0)
						{
							lineCount++;
						}
						nextWidth += lineCount * nowHeight;
						while(nextWidth >= mPageSize)
						{
							nextWidth -= mPageSize;
							totalHeight += maxHeight;
						}
					}
				}
			}
			nowHeight >>= 1;
			fontCountPerLine <<= 1;
		}
		if(nextWidth>0)
		{
			totalHeight += maxHeight;
		}
		TSun::TU32 pageCount = totalHeight / mPageSize;
		if(totalHeight % mPageSize > 0)
		{
			pageCount++;
		}
		// 分配空间，拷贝纹理
		for(TSun::TU32 iPage=0;iPage<pageCount-mPageCount;++iPage)
		{
			TSun::TU32* newBuffer = T_NEW_ARRAY(getEngineStructMemAllocator(), TSun::TU32, mPageSize*mPageSize);
			memset(newBuffer,0,sizeof(TSun::TU32)*mPageSize*mPageSize);
			mPageBufferList.push_back(newBuffer);
		}
		mPageCount = pageCount;
		
		TSun::TU32 *texp = TSun::TNULL;
		nowHeight = maxHeight;
		fontCountPerLine = 0;
		TSun::TU32 widthStep = 0;
		nextWidth = 0;
		TSun::TU32 copyPage = 0;
		TSun::TU32 rowCount = 0;
		TSun::TU32 maxRowPerPage = mPageSize / maxHeight;
		while(nowHeight>=minHeight)
		{
			TSun::List<TSun::TS32> valueList;
			if(texHeightMap.get(nowHeight, valueList) == TSun::TTRUE)
			{
				if(valueList.size()>0)
				{
					if(nowHeight==maxHeight)
					{
						fontCountPerLine = 1;
						for(TSun::TU32 index=0;index<(TSun::TU32)valueList.size();++index)
						{
							TSun::TS32 iFont = valueList.get(index);
							texp = mPageBufferList[copyPage] + widthStep + rowCount*maxHeight*mPageSize;
							TSun::TU32* pt = m_Glyphs[iFont].texd;
							for (TSun::TU32 i = 0;i < m_Glyphs[iFont].imgh;i++){
								TSun::TU32 *rowp = texp;
								for (TSun::TU32 j = 0;j < m_Glyphs[iFont].imgw;j++){
									if (*pt){
										*rowp = *pt;
									} else {
										*rowp = 0;
									}
									pt++;
									rowp++;
								}
								texp += mPageSize;
							}

							m_Glyphs[iFont].texStartU = (TSun::TF32)widthStep / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].texEndU = m_Glyphs[iFont].texStartU + (TSun::TF32)m_Glyphs[iFont].imgw / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].texStartV = (TSun::TF32)(rowCount*maxHeight) / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].texEndV = m_Glyphs[iFont].texStartV + (TSun::TF32)m_Glyphs[iFont].imgh / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].pageIndex = copyPage;
							widthStep += m_Glyphs[iFont].imgw;
							if(widthStep >= mPageSize)
							{
								widthStep = 0;
								rowCount++;
								if(rowCount>=maxRowPerPage)
								{
									copyPage++;
									rowCount=0;
								}
							}
						}
					}
					else
					{
						TSun::TU32 lineCount = (TSun::TU32)valueList.size() / fontCountPerLine;
						if((TSun::TU32)valueList.size() % fontCountPerLine > 0)
						{
							lineCount++;
						}
						TSun::TU32 countFontPerLine = 0;
						TSun::TU32 countLine = 0;
						for(TSun::TU32 index=0;index<(TSun::TU32)valueList.size();++index)
						{
							TSun::TS32 iFont = valueList.get(index);
							texp = mPageBufferList[copyPage] + widthStep + rowCount*maxHeight*mPageSize + countFontPerLine*nowHeight*mPageSize;
							TSun::TU32* pt = m_Glyphs[iFont].texd;
							for (TSun::TU32 i = 0;i < m_Glyphs[iFont].imgh;i++){
								TSun::TU32 *rowp = texp;
								for (TSun::TU32 j = 0;j < m_Glyphs[iFont].imgw;j++){
									if (*pt){
										*rowp = *pt;
									} else {
										*rowp = 0;
									}
									pt++;
									rowp++;
								}
								texp += mPageSize;
							}

							m_Glyphs[iFont].texStartU = (TSun::TF32)widthStep / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].texEndU = m_Glyphs[iFont].texStartU + (TSun::TF32)m_Glyphs[iFont].imgw / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].texStartV = (TSun::TF32)(rowCount*maxHeight + countFontPerLine*nowHeight) / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].texEndV = m_Glyphs[iFont].texStartV + (TSun::TF32)m_Glyphs[iFont].imgh / (TSun::TF32)mPageSize;
							m_Glyphs[iFont].pageIndex = copyPage;
							countFontPerLine++;
							if(countFontPerLine >= fontCountPerLine)
							{
								countFontPerLine = 0;
								countLine++;
								widthStep += m_Glyphs[iFont].imgw;
								if(widthStep >= mPageSize)
								{
									widthStep = 0;
									rowCount++;
									if(rowCount>=maxRowPerPage)
									{
										copyPage++;
										rowCount=0;
									}
								}
							}
						}
						if(countLine<lineCount)
						{
							widthStep += nowHeight;
							if(widthStep >= mPageSize)
							{
								widthStep = 0;
								rowCount++;
								if(rowCount>=maxRowPerPage)
								{
									copyPage++;
									rowCount=0;
								}
							}
						}
					}
				}
			}
			nowHeight >>= 1;
			fontCountPerLine <<= 1;
		}
		// 构建渲染组建
		TSun::List<RenderDeviceUsedSRV*>::Iterator itrSRV = mSRVList.begin();
		for(;itrSRV!=mSRVList.end();++itrSRV)
		{
			RenderDeviceUsedSRV* del = *itrSRV;
			if (del)
			{
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSRV, del);
				del = 0;
			}
		}
		mSRVList.clear();
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		for(TSun::TU32 iPage=0;iPage<mPageCount;++iPage)
		{
			TSun::TU32* theBuffer = mPageBufferList[iPage];
			// Create Shader Resource
			TLRenderDeviceTex2DDesc texDesc;
			texDesc.ArraySize = 1;
			texDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_SHADER_RESOURCE;
			texDesc.CPUAccessFlags = TSun::TFALSE;
			texDesc.Cube = TSun::TFALSE;
			texDesc.Format = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
			texDesc.GenerateMips = TSun::TFALSE;
			texDesc.Height = mPageSize;
			texDesc.MipLevels = 1;
			texDesc.SampleCount = 1;
			texDesc.SampleQuality = 0;
			texDesc.Width = mPageSize;
			TLRenderDeviceSubresourceData initData;
			initData.pSysMem = theBuffer;
			initData.SysMemPitch = mPageSize*sizeof(TSun::TU32);
			RenderDeviceUsedTex2D* pTex = pDevice->createTex2D(&texDesc,&initData);
			if(!pTex)
			{
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
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedTex2D, pTex);
				return TSun::TFALSE;
			}
			T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedTex2D, pTex);
			mSRVList.push_back(pSRV);
		}
		
		return TSun::TTRUE;
	}

	//! returns the dimension of a text
	TSun::TVOID GUIFont::GetDimension(const TSun::TWCHAR* text, TSun::TU32& left, TSun::TU32& right, TSun::TU32& top, TSun::TU32& bottom)
	{
		top = left = 0;
		right = 0;
		bottom = mFontSize;

		for(const TSun::TWCHAR* p = text; *p; ++p)
		{
			right += GetWidthFromCharacter(*p);
		}

		return ;
	}


	TSun::TS32 GUIFont::GetWidthFromCharacter(TSun::TWCHAR c)
	{
		TSun::TBOOL catched = TSun::TFALSE;
		TSun::TU32 n = GetGlyphByChar(c,catched);
		if ( n > 0){
			TSun::TS32 w = m_Glyphs[n - 1].texw;
			TSun::TS32 left = m_Glyphs[n - 1].left;
			if (w + left > 0) return w + left;
		}
		if (c >= 0x2000){
			return	mFontSize;
		} else {
			return	mFontSize / 2;
		}
	}

	TSun::TVOID GUIFont::PreDraw(TSun::TS32 n,TSun::TS32* imgw,TSun::TS32* imgh,TSun::TS32* texw,TSun::TS32* texh,TSun::TS32* offx,TSun::TS32* offy,
			TSun::TF32* texStartU,TSun::TF32* texEndU,TSun::TF32* texStartV,TSun::TF32* texEndV,TSun::TU32* pageIndex)
	{
		if (!imgw || !imgh || !texw || !texh || !offx || !offy || !texStartU || !texEndU || !texStartV || !texEndV || !pageIndex)
		{
			return ;
		}
		*imgw = m_Glyphs[n-1].imgw;
		*imgh = m_Glyphs[n-1].imgh;
		*texw = m_Glyphs[n-1].texw;
		*texh = m_Glyphs[n-1].texh;
		*offx = m_Glyphs[n-1].left;
		*offy = mFontSize - m_Glyphs[n-1].top;
		*texStartU = m_Glyphs[n-1].texStartU;
		*texEndU = m_Glyphs[n-1].texEndU;
		*texStartV = m_Glyphs[n-1].texStartV;
		*texEndV = m_Glyphs[n-1].texEndV;
		*pageIndex = m_Glyphs[n-1].pageIndex;
	}

	RenderDeviceUsedSRV* GUIFont::getSRV(TSun::TU32 pageIndex)
	{
		if(pageIndex>=mSRVList.size())
			return TSun::TNULL;
		return mSRVList[pageIndex];
	}

	//! Calculates the index of the character in the text which is on a specific position.
	TSun::TS32 GUIFont::GetCharacterFromPos(const TSun::TWCHAR* text, TSun::TS32 pixel_x)
	{
		TSun::TS32 x = 0;
		TSun::TS32 idx = 0;

		while (text[idx])
		{
			x += GetWidthFromCharacter(text[idx]);

			if (x >= pixel_x)
				return idx;

			++idx;
		}

		return -1;
	}

}