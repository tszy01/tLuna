#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>
#include <vector>

namespace TLunaEngine{
	class TLIRenderDeviceUsedSRV;
	// 处理每一个文字，生成纹理
	class TLFontGlyph
	{
	public:
		bool cached;
		TLFontGlyph();
		~TLFontGlyph();
		void cache(UINT idx,UINT size);
		FT_Face *face;
		//UINT size;
		UINT top;
		UINT left;
		UINT texw;
		UINT texh;
		UINT imgw;
		UINT imgh;
		INT offset;
		UINT* texd;
		FLOAT texStartU;
		FLOAT texEndU;
		FLOAT texStartV;
		FLOAT texEndV;
		UINT pageIndex;
	};

	/*
	 *	自制字体类，使用FreeType库
	 */
	class TLGUIFont
	{
	public:

		//! constructor
		TLGUIFont();

		//! destructor
		~TLGUIFont();

		//! loads a truetype font file
		bool InitFont(const char* filename,UINT size,UINT texPageSize,int id,FT_Library lib);

		//! draws an text and clips it to the specified rectangle if wanted
		void PreDraw(int n,INT* imgw,INT* imgh,INT* texw,INT* texh,INT* offx,INT* offy,
			FLOAT* texStartU,FLOAT* texEndU,FLOAT* texStartV,FLOAT* texEndV,UINT* pageIndex);

		//! returns the dimension of a text
		void GetDimension(const wchar_t* text,RECT* pRc);

		//! Calculates the index of the character in the text which is on a specific position.
		INT GetCharacterFromPos(const wchar_t* text, INT pixel_x);

		INT GetWidthFromCharacter(wchar_t c);
		UINT GetGlyphByChar(wchar_t c,bool& newFontCached);

		// catch all font to one dsv
		bool catchAllFont();

		inline int GetId()
		{
			return m_Id;
		}

		TLIRenderDeviceUsedSRV* getSRV(UINT pageIndex);
	private:
		TLFontGlyph* m_Glyphs;
		FT_Library	library;
		FT_Face		face;
		int			m_Id;	// 全局唯一ID
		//TLIRenderDeviceUsedSRV* mSRV;
		//UINT mTotalWidth;
		//UINT mTotalHeight;
		//UINT* mPixelBuffer;
		UINT mFontSize;

		UINT mPageCount;
		UINT mPageSize;
		std::vector<UINT*> mPageBufferList;
		std::vector<TLIRenderDeviceUsedSRV*> mSRVList;
	};

}