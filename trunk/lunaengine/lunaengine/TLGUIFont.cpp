#include "StdAfx.h"
#include "TLGUIFont.h"
#include "TLRenderMgr.h"
#include "TLIRenderDevice.h"
#include "TLIRenderDeviceUsedTex2D.h"
#include "TLIRenderDeviceUsedSRV.h"
#include <map>
#include <vector>

namespace TLunaEngine{

	// ------------------ TLFontGlyph ----------------------
	TLFontGlyph::TLFontGlyph() : texd(NULL),pageIndex(-1)
	{
	}

	TLFontGlyph::~TLFontGlyph()
	{
		if(texd)
		{
			delete [] texd;
			texd = NULL;
		}
	}

	void TLFontGlyph::cache(UINT idx,UINT size)
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
					unsigned char *pt = bits.buffer;
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
					texd = new UINT[imgw*imgh];
					memset(texd,0,imgw*imgh*sizeof(UINT));
					UINT *texp = texd;
					offset = size - bits.rows;
					bool cflag = false;
					for (int i = 0;i < bits.rows;i++){
						UINT *rowp = texp;
						for (int j = 0;j < bits.width;j++){
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
					cached = true;
				}
			}
		}
	}

	// ------------------ TLGUIFont ------------------------
	TLGUIFont::TLGUIFont(void) : 
	m_Glyphs(NULL),
	face(NULL),
	library(NULL),
	m_Id(-1),
	mFontSize(0),
	mPageCount(0),
	mPageSize(512)
	{
	}

	TLGUIFont::~TLGUIFont(void)
	{
		std::vector<UINT*>::iterator itrBuffer = mPageBufferList.begin();
		for(;itrBuffer!=mPageBufferList.end();++itrBuffer)
		{
			UINT* del = *itrBuffer;
			SAFE_DELETE_ARRAY(del);
		}
		mPageBufferList.clear();
		std::vector<TLIRenderDeviceUsedSRV*>::iterator itrSRV = mSRVList.begin();
		for(;itrSRV!=mSRVList.end();++itrSRV)
		{
			TLIRenderDeviceUsedSRV* del = *itrSRV;
			SAFE_DELETE(del);
		}
		mSRVList.clear();
		mPageCount=0;
		if (m_Glyphs)
		{
			delete [] m_Glyphs;
			m_Glyphs = NULL;
		}
		FT_Done_Face(face);
		face = NULL;
		library = NULL;
	}

	bool TLGUIFont::InitFont(const char *filename, UINT size,UINT texPageSize,int id,FT_Library lib)
	{
		if(filename==NULL || lib==NULL || texPageSize<=0 || size<=0)
		{
			return false;
		}
		// Load Freetype
		library = lib;
		if (FT_New_Face( library,filename,0,&face )){
			return	false;
		}
		// 创建文字个体
		m_Glyphs = new TLFontGlyph[face->num_glyphs];
		for (int i = 0;i < face->num_glyphs;i++){
			m_Glyphs[i].face = &face;
			m_Glyphs[i].cached = false;
		}
		m_Id = id;
		mFontSize = size;
		mPageSize = texPageSize;
		return true;
	}

	UINT TLGUIFont::GetGlyphByChar(wchar_t c,bool& newFontCached){
		newFontCached = false;
		UINT idx = FT_Get_Char_Index( face, c );
		if (idx && !m_Glyphs[idx - 1].cached)
		{
			m_Glyphs[idx - 1].cache(idx,mFontSize);
			catchAllFont();
			newFontCached = true;
		}
		return	idx;
	}

	bool TLGUIFont::catchAllFont()
	{
		if(mPageSize==0)
			return false;
		// 摆放规则
		// 从大到小，先摆放大的，然后摆放小的，竖着摆，一列为一行
		// 第一遍存放到map中
		std::map<UINT,std::vector<int>> texHeightMap;
		UINT maxHeight=0,minHeight=0,count=0;
		for (int i = 0;i < face->num_glyphs;i++){
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
			return false;
		// 计算buffer的大小
		UINT totalHeight=0,nowHeight=maxHeight;
		UINT fontCountPerLine=0;
		UINT nextWidth = 0;
		while(nowHeight>=minHeight)
		{
			std::map<UINT,std::vector<int>>::iterator itrFind = texHeightMap.find(nowHeight);
			if(itrFind!=texHeightMap.end())
			{
				if(itrFind->second.size()>0)
				{
					if(nowHeight==maxHeight)
					{
						UINT maxCount = mPageSize / nowHeight;
						totalHeight = nowHeight * (itrFind->second.size() / maxCount);
						nextWidth = itrFind->second.size() % maxCount * nowHeight;
						fontCountPerLine = 1;
					}
					else
					{
						UINT lineCount = itrFind->second.size() / fontCountPerLine;
						if(itrFind->second.size() % fontCountPerLine > 0)
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
		UINT pageCount = totalHeight / mPageSize;
		if(totalHeight % mPageSize > 0)
		{
			pageCount++;
		}
		// 分配空间，拷贝纹理
		for(UINT iPage=0;iPage<pageCount-mPageCount;++iPage)
		{
			UINT* newBuffer = new UINT[mPageSize*mPageSize];
			memset(newBuffer,0,sizeof(UINT)*mPageSize*mPageSize);
			mPageBufferList.push_back(newBuffer);
		}
		mPageCount = pageCount;
		
		UINT *texp = NULL;
		nowHeight = maxHeight;
		fontCountPerLine = 0;
		UINT widthStep = 0;
		nextWidth = 0;
		UINT copyPage = 0;
		UINT rowCount = 0;
		UINT maxRowPerPage = mPageSize / maxHeight;
		while(nowHeight>=minHeight)
		{
			std::map<UINT,std::vector<int>>::iterator itrFind = texHeightMap.find(nowHeight);
			if(itrFind!=texHeightMap.end())
			{
				if(itrFind->second.size()>0)
				{
					if(nowHeight==maxHeight)
					{
						fontCountPerLine = 1;
						for(size_t index=0;index<itrFind->second.size();++index)
						{
							int& iFont = itrFind->second.at(index);
							texp = mPageBufferList[copyPage] + widthStep + rowCount*maxHeight*mPageSize;
							UINT* pt = m_Glyphs[iFont].texd;
							for (UINT i = 0;i < m_Glyphs[iFont].imgh;i++){
								UINT *rowp = texp;
								for (UINT j = 0;j < m_Glyphs[iFont].imgw;j++){
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

							m_Glyphs[iFont].texStartU = (FLOAT)widthStep / (FLOAT)mPageSize;
							m_Glyphs[iFont].texEndU = m_Glyphs[iFont].texStartU + (FLOAT)m_Glyphs[iFont].imgw / (FLOAT)mPageSize;
							m_Glyphs[iFont].texStartV = (FLOAT)(rowCount*maxHeight) / (FLOAT)mPageSize;
							m_Glyphs[iFont].texEndV = m_Glyphs[iFont].texStartV + (FLOAT)m_Glyphs[iFont].imgh / (FLOAT)mPageSize;
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
						UINT lineCount = itrFind->second.size() / fontCountPerLine;
						if(itrFind->second.size() % fontCountPerLine > 0)
						{
							lineCount++;
						}
						UINT countFontPerLine = 0;
						UINT countLine = 0;
						for(size_t index=0;index<itrFind->second.size();++index)
						{
							int& iFont = itrFind->second.at(index);
							texp = mPageBufferList[copyPage] + widthStep + rowCount*maxHeight*mPageSize + countFontPerLine*nowHeight*mPageSize;
							UINT* pt = m_Glyphs[iFont].texd;
							for (UINT i = 0;i < m_Glyphs[iFont].imgh;i++){
								UINT *rowp = texp;
								for (UINT j = 0;j < m_Glyphs[iFont].imgw;j++){
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

							m_Glyphs[iFont].texStartU = (FLOAT)widthStep / (FLOAT)mPageSize;
							m_Glyphs[iFont].texEndU = m_Glyphs[iFont].texStartU + (FLOAT)m_Glyphs[iFont].imgw / (FLOAT)mPageSize;
							m_Glyphs[iFont].texStartV = (FLOAT)(rowCount*maxHeight + countFontPerLine*nowHeight) / (FLOAT)mPageSize;
							m_Glyphs[iFont].texEndV = m_Glyphs[iFont].texStartV + (FLOAT)m_Glyphs[iFont].imgh / (FLOAT)mPageSize;
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
		std::vector<TLIRenderDeviceUsedSRV*>::iterator itrSRV = mSRVList.begin();
		for(;itrSRV!=mSRVList.end();++itrSRV)
		{
			TLIRenderDeviceUsedSRV* del = *itrSRV;
			SAFE_DELETE(del);
		}
		mSRVList.clear();
		TLIRenderDevice* pDevice = TLRenderMgr::getSingletonPtr()->getDevice();
		for(UINT iPage=0;iPage<mPageCount;++iPage)
		{
			UINT* theBuffer = mPageBufferList[iPage];
			// Create Shader Resource
			TLRenderDeviceTex2DDesc texDesc;
			texDesc.ArraySize = 1;
			texDesc.BindFlags = RENDER_DEVICE_BIND_FLAG_SHADER_RESOURCE;
			texDesc.CPUAccessFlags = FALSE;
			texDesc.Cube = FALSE;
			texDesc.Format = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
			texDesc.GenerateMips = FALSE;
			texDesc.Height = mPageSize;
			texDesc.MipLevels = 1;
			texDesc.SampleCount = 1;
			texDesc.SampleQuality = 0;
			texDesc.Width = mPageSize;
			TLRenderDeviceSubresourceData initData;
			initData.pSysMem = theBuffer;
			initData.SysMemPitch = mPageSize*sizeof(UINT);
			TLIRenderDeviceUsedTex2D* pTex = pDevice->createTex2D(&texDesc,&initData);
			if(!pTex)
			{
				return false;
			}
			TLRenderDeviceSRVDesc srvDesc;
			srvDesc.Format = RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = RENDER_DEVICE_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Params[0] = 0;
			srvDesc.Params[1] = 1;
			TLIRenderDeviceUsedSRV* pSRV = pDevice->createShaderResourceView(pTex,&srvDesc);
			if(!pSRV)
			{
				delete pTex;
				return false;
			}
			delete pTex;
			mSRVList.push_back(pSRV);
		}
		
		return true;
	}

	//! returns the dimension of a text
	void TLGUIFont::GetDimension(const wchar_t* text,RECT* pRc)
	{
		pRc->top = pRc->left = 0;
		pRc->right = 0;
		pRc->bottom = mFontSize;

		for(const wchar_t* p = text; *p; ++p)
		{
			pRc->right += GetWidthFromCharacter(*p);
		}

		return ;
	}


	INT TLGUIFont::GetWidthFromCharacter(wchar_t c)
	{
		bool catched = false;
		UINT n = GetGlyphByChar(c,catched);
		if ( n > 0){
			int w = m_Glyphs[n - 1].texw;
			INT left = m_Glyphs[n - 1].left;
			if (w + left > 0) return w + left;
		}
		if (c >= 0x2000){
			return	mFontSize;
		} else {
			return	mFontSize / 2;
		}
	}

	void TLGUIFont::PreDraw(int n,INT* imgw,INT* imgh,INT* texw,INT* texh,INT* offx,INT* offy,
			FLOAT* texStartU,FLOAT* texEndU,FLOAT* texStartV,FLOAT* texEndV,UINT* pageIndex)
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

	TLIRenderDeviceUsedSRV* TLGUIFont::getSRV(UINT pageIndex)
	{
		if(pageIndex>=mSRVList.size())
			return NULL;
		return mSRVList[pageIndex];
	}

	//! Calculates the index of the character in the text which is on a specific position.
	INT TLGUIFont::GetCharacterFromPos(const wchar_t* text, INT pixel_x)
	{
		INT x = 0;
		INT idx = 0;

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