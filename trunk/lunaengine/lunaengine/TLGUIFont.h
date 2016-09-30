#ifndef _TLGUIFONT_H_
#define _TLGUIFONT_H_

#include <ft2build.h>
#include <freetype/freetype.h>
#include <vector>
#include "TLCommonTypeDef.h"

namespace TLunaEngine{
	class RenderDeviceUsedSRV;
	// 处理每一个文字，生成纹理
	class FontGlyph
	{
	public:
		TBOOL cached;
		FontGlyph();
		~FontGlyph();
		TVOID cache(TU32 idx,TU32 size);
		FT_Face *face;
		//TU32 size;
		TU32 top;
		TU32 left;
		TU32 texw;
		TU32 texh;
		TU32 imgw;
		TU32 imgh;
		TS32 offset;
		TU32* texd;
		TF32 texStartU;
		TF32 texEndU;
		TF32 texStartV;
		TF32 texEndV;
		TU32 pageIndex;
	};

	/*
	 *	自制字体类，使用FreeType库
	 */
	class GUIFont
	{
	public:

		//! constructor
		GUIFont();

		//! destructor
		~GUIFont();

		//! loads a truetype font file
		TBOOL InitFont(const TCHAR* filename,TU32 size,TU32 texPageSize,int id,FT_Library lib);

		//! draws an text and clips it to the specified rectangle if wanted
		TVOID PreDraw(int n,TS32* imgw,TS32* imgh,TS32* texw,TS32* texh,TS32* offx,TS32* offy,
			TF32* texStartU,TF32* texEndU,TF32* texStartV,TF32* texEndV,TU32* pageIndex);

		//! returns the dimension of a text
		TVOID GetDimension(const TWCHAR* text, TU32& left, TU32& right, TU32& top, TU32& bottom);

		//! Calculates the index of the character in the text which is on a specific position.
		TS32 GetCharacterFromPos(const TWCHAR* text, TS32 pixel_x);

		TS32 GetWidthFromCharacter(TWCHAR c);
		TU32 GetGlyphByChar(TWCHAR c,TBOOL& newFontCached);

		// catch all font to one dsv
		TBOOL catchAllFont();

		inline int GetId()
		{
			return m_Id;
		}

		RenderDeviceUsedSRV* getSRV(TU32 pageIndex);
	private:
		FontGlyph* m_Glyphs;
		FT_Library	library;
		FT_Face		face;
		int			m_Id;	// 全局唯一ID
		//RenderDeviceUsedSRV* mSRV;
		//TU32 mTotalWidth;
		//TU32 mTotalHeight;
		//TU32* mPixelBuffer;
		TU32 mFontSize;

		TU32 mPageCount;
		TU32 mPageSize;
		std::vector<TU32*> mPageBufferList;
		std::vector<RenderDeviceUsedSRV*> mSRVList;
	};

}

#endif