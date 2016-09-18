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
		bool cached;
		FontGlyph();
		~FontGlyph();
		void cache(TU32 idx,TU32 size);
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
		bool InitFont(const char* filename,TU32 size,TU32 texPageSize,int id,FT_Library lib);

		//! draws an text and clips it to the specified rectangle if wanted
		void PreDraw(int n,TS32* imgw,TS32* imgh,TS32* texw,TS32* texh,TS32* offx,TS32* offy,
			TF32* texStartU,TF32* texEndU,TF32* texStartV,TF32* texEndV,TU32* pageIndex);

		//! returns the dimension of a text
		void GetDimension(const wchar_t* text, TU32& left, TU32& right, TU32& top, TU32& bottom);

		//! Calculates the index of the character in the text which is on a specific position.
		TS32 GetCharacterFromPos(const wchar_t* text, TS32 pixel_x);

		TS32 GetWidthFromCharacter(wchar_t c);
		TU32 GetGlyphByChar(wchar_t c,bool& newFontCached);

		// catch all font to one dsv
		bool catchAllFont();

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