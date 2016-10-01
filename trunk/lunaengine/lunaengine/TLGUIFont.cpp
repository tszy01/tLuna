#include "TLGUIFont.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice.h"
#include "TLRenderDeviceUsedTex2D.h"
#include "TLRenderDeviceUsedSRV.h"
#include "TLMap.h"

namespace TLunaEngine{

	// ------------------ FontGlyph ----------------------
	FontGlyph::FontGlyph() : texd(TNULL),pageIndex(-1)
	{
	}

	FontGlyph::~FontGlyph()
	{
		if(texd)
		{
			delete [] texd;
			texd = TNULL;
		}
	}

	TVOID FontGlyph::cache(TU32 idx,TU32 size)
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
					TUByte *pt = bits.buffer;
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
					texd = new TU32[imgw*imgh];
					memset(texd,0,imgw*imgh*sizeof(TU32));
					TU32 *texp = texd;
					offset = size - bits.rows;
					TBOOL cflag = TFALSE;
					for (TS32 i = 0;i < bits.rows;i++){
						TU32 *rowp = texp;
						for (TS32 j = 0;j < bits.width;j++){
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
					cached = TTRUE;
				}
			}
		}
	}

	// ------------------ GUIFont ------------------------
	GUIFont::GUIFont(TVOID) : 
	m_Glyphs(TNULL),
	face(TNULL),
	library(TNULL),
	m_Id(-1),
	mFontSize(0),
	mPageCount(0),
	mPageSize(512)
	{
	}

	GUIFont::~GUIFont(TVOID)
	{
		List<TU32*>::Iterator itrBuffer = mPageBufferList.begin();
		for(;itrBuffer!=mPageBufferList.end();++itrBuffer)
		{
			TU32* del = *itrBuffer;
			if (del)
			{
				delete [] del;
				(*itrBuffer) = 0;
			}
		}
		mPageBufferList.clear();
		List<RenderDeviceUsedSRV*>::Iterator itrSRV = mSRVList.begin();
		for(;itrSRV!=mSRVList.end();++itrSRV)
		{
			RenderDeviceUsedSRV* del = *itrSRV;
			if (del)
			{
				delete del;
				del = 0;
			}
		}
		mSRVList.clear();
		mPageCount=0;
		if (m_Glyphs)
		{
			delete [] m_Glyphs;
			m_Glyphs = TNULL;
		}
		FT_Done_Face(face);
		face = TNULL;
		library = TNULL;
	}

	TBOOL GUIFont::InitFont(const TCHAR* filename, TU32 size,TU32 texPageSize,TS32 id,FT_Library lib)
	{
		if(filename==TNULL || lib==TNULL || texPageSize<=0 || size<=0)
		{
			return TFALSE;
		}
		// Load Freetype
		library = lib;
		if (FT_New_Face( library,filename,0,&face )){
			return	TFALSE;
		}
		// 创建文字个体
		m_Glyphs = new FontGlyph[face->num_glyphs];
		for (TS32 i = 0;i < face->num_glyphs;i++){
			m_Glyphs[i].face = &face;
			m_Glyphs[i].cached = TFALSE;
		}
		m_Id = id;
		mFontSize = size;
		mPageSize = texPageSize;
		return TTRUE;
	}

	TU32 GUIFont::GetGlyphByChar(TWCHAR c,TBOOL& newFontCached){
		newFontCached = TFALSE;
		TU32 idx = FT_Get_Char_Index( face, c );
		if (idx && !m_Glyphs[idx - 1].cached)
		{
			m_Glyphs[idx - 1].cache(idx,mFontSize);
			catchAllFont();
			newFontCached = TTRUE;
		}
		return	idx;
	}

	TBOOL GUIFont::catchAllFont()
	{
		if(mPageSize==0)
			return TFALSE;
		// 摆放规则
		// 从大到小，先摆放大的，然后摆放小的，竖着摆，一列为一行
		// 第一遍存放到map中
		Map<TU32,List<TS32>> texHeightMap;
		TU32 maxHeight=0,minHeight=0,count=0;
		for (TS32 i = 0;i < face->num_glyphs;i++){
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
			return TFALSE;
		// 计算buffer的大小
		TU32 totalHeight=0,nowHeight=maxHeight;
		TU32 fontCountPerLine=0;
		TU32 nextWidth = 0;
		while(nowHeight>=minHeight)
		{
			List<TS32> valueList;
			if(texHeightMap.get(nowHeight, valueList) == TTRUE)
			{
				if(valueList.size()>0)
				{
					if(nowHeight==maxHeight)
					{
						TU32 maxCount = mPageSize / nowHeight;
						totalHeight = nowHeight * ((TU32)valueList.size() / maxCount);
						nextWidth = valueList.size() % maxCount * nowHeight;
						fontCountPerLine = 1;
					}
					else
					{
						TU32 lineCount = (TU32)valueList.size() / fontCountPerLine;
						if((TU32)valueList.size() % fontCountPerLine > 0)
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
		TU32 pageCount = totalHeight / mPageSize;
		if(totalHeight % mPageSize > 0)
		{
			pageCount++;
		}
		// 分配空间，拷贝纹理
		for(TU32 iPage=0;iPage<pageCount-mPageCount;++iPage)
		{
			TU32* newBuffer = new TU32[mPageSize*mPageSize];
			memset(newBuffer,0,sizeof(TU32)*mPageSize*mPageSize);
			mPageBufferList.push_back(newBuffer);
		}
		mPageCount = pageCount;
		
		TU32 *texp = TNULL;
		nowHeight = maxHeight;
		fontCountPerLine = 0;
		TU32 widthStep = 0;
		nextWidth = 0;
		TU32 copyPage = 0;
		TU32 rowCount = 0;
		TU32 maxRowPerPage = mPageSize / maxHeight;
		while(nowHeight>=minHeight)
		{
			List<TS32> valueList;
			if(texHeightMap.get(nowHeight, valueList) == TTRUE)
			{
				if(valueList.size()>0)
				{
					if(nowHeight==maxHeight)
					{
						fontCountPerLine = 1;
						for(TU32 index=0;index<(TU32)valueList.size();++index)
						{
							TS32 iFont = valueList.get(index);
							texp = mPageBufferList[copyPage] + widthStep + rowCount*maxHeight*mPageSize;
							TU32* pt = m_Glyphs[iFont].texd;
							for (TU32 i = 0;i < m_Glyphs[iFont].imgh;i++){
								TU32 *rowp = texp;
								for (TU32 j = 0;j < m_Glyphs[iFont].imgw;j++){
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

							m_Glyphs[iFont].texStartU = (TF32)widthStep / (TF32)mPageSize;
							m_Glyphs[iFont].texEndU = m_Glyphs[iFont].texStartU + (TF32)m_Glyphs[iFont].imgw / (TF32)mPageSize;
							m_Glyphs[iFont].texStartV = (TF32)(rowCount*maxHeight) / (TF32)mPageSize;
							m_Glyphs[iFont].texEndV = m_Glyphs[iFont].texStartV + (TF32)m_Glyphs[iFont].imgh / (TF32)mPageSize;
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
						TU32 lineCount = (TU32)valueList.size() / fontCountPerLine;
						if((TU32)valueList.size() % fontCountPerLine > 0)
						{
							lineCount++;
						}
						TU32 countFontPerLine = 0;
						TU32 countLine = 0;
						for(TU32 index=0;index<(TU32)valueList.size();++index)
						{
							TS32 iFont = valueList.get(index);
							texp = mPageBufferList[copyPage] + widthStep + rowCount*maxHeight*mPageSize + countFontPerLine*nowHeight*mPageSize;
							TU32* pt = m_Glyphs[iFont].texd;
							for (TU32 i = 0;i < m_Glyphs[iFont].imgh;i++){
								TU32 *rowp = texp;
								for (TU32 j = 0;j < m_Glyphs[iFont].imgw;j++){
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

							m_Glyphs[iFont].texStartU = (TF32)widthStep / (TF32)mPageSize;
							m_Glyphs[iFont].texEndU = m_Glyphs[iFont].texStartU + (TF32)m_Glyphs[iFont].imgw / (TF32)mPageSize;
							m_Glyphs[iFont].texStartV = (TF32)(rowCount*maxHeight + countFontPerLine*nowHeight) / (TF32)mPageSize;
							m_Glyphs[iFont].texEndV = m_Glyphs[iFont].texStartV + (TF32)m_Glyphs[iFont].imgh / (TF32)mPageSize;
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
		List<RenderDeviceUsedSRV*>::Iterator itrSRV = mSRVList.begin();
		for(;itrSRV!=mSRVList.end();++itrSRV)
		{
			RenderDeviceUsedSRV* del = *itrSRV;
			if (del)
			{
				delete del;
				del = 0;
			}
		}
		mSRVList.clear();
		RenderDevice* pDevice = RenderMgr::getSingletonPtr()->getDevice();
		for(TU32 iPage=0;iPage<mPageCount;++iPage)
		{
			TU32* theBuffer = mPageBufferList[iPage];
			// Create Shader Resource
			TLRenderDeviceTex2DDesc texDesc;
			texDesc.ArraySize = 1;
			texDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_SHADER_RESOURCE;
			texDesc.CPUAccessFlags = TFALSE;
			texDesc.Cube = TFALSE;
			texDesc.Format = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
			texDesc.GenerateMips = TFALSE;
			texDesc.Height = mPageSize;
			texDesc.MipLevels = 1;
			texDesc.SampleCount = 1;
			texDesc.SampleQuality = 0;
			texDesc.Width = mPageSize;
			TLRenderDeviceSubresourceData initData;
			initData.pSysMem = theBuffer;
			initData.SysMemPitch = mPageSize*sizeof(TU32);
			RenderDeviceUsedTex2D* pTex = pDevice->createTex2D(&texDesc,&initData);
			if(!pTex)
			{
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
				delete pTex;
				return TFALSE;
			}
			delete pTex;
			mSRVList.push_back(pSRV);
		}
		
		return TTRUE;
	}

	//! returns the dimension of a text
	TVOID GUIFont::GetDimension(const TWCHAR* text, TU32& left, TU32& right, TU32& top, TU32& bottom)
	{
		top = left = 0;
		right = 0;
		bottom = mFontSize;

		for(const TWCHAR* p = text; *p; ++p)
		{
			right += GetWidthFromCharacter(*p);
		}

		return ;
	}


	TS32 GUIFont::GetWidthFromCharacter(TWCHAR c)
	{
		TBOOL catched = TFALSE;
		TU32 n = GetGlyphByChar(c,catched);
		if ( n > 0){
			TS32 w = m_Glyphs[n - 1].texw;
			TS32 left = m_Glyphs[n - 1].left;
			if (w + left > 0) return w + left;
		}
		if (c >= 0x2000){
			return	mFontSize;
		} else {
			return	mFontSize / 2;
		}
	}

	TVOID GUIFont::PreDraw(TS32 n,TS32* imgw,TS32* imgh,TS32* texw,TS32* texh,TS32* offx,TS32* offy,
			TF32* texStartU,TF32* texEndU,TF32* texStartV,TF32* texEndV,TU32* pageIndex)
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

	RenderDeviceUsedSRV* GUIFont::getSRV(TU32 pageIndex)
	{
		if(pageIndex>=mSRVList.size())
			return TNULL;
		return mSRVList[pageIndex];
	}

	//! Calculates the index of the character in the text which is on a specific position.
	TS32 GUIFont::GetCharacterFromPos(const TWCHAR* text, TS32 pixel_x)
	{
		TS32 x = 0;
		TS32 idx = 0;

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