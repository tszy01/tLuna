#ifndef _TLGUIFONT_H_
#define _TLGUIFONT_H_

#include <ft2build.h>
#include <freetype/freetype.h>
#include "TSCommonTypeDef.h"
#include "TSList.h"

namespace TLunaEngine{
	class RenderDeviceUsedSRV;
	// 处理每一个文字，生成纹理
	class FontGlyph
	{
	public:
		TSun::TBOOL cached;
		FontGlyph();
		~FontGlyph();
		TSun::TVOID cache(TSun::TU32 idx,TSun::TU32 size);
		FT_Face *face;
		//TSun::TU32 size;
		TSun::TU32 top;
		TSun::TU32 left;
		TSun::TU32 texw;
		TSun::TU32 texh;
		TSun::TU32 imgw;
		TSun::TU32 imgh;
		TSun::TS32 offset;
		TSun::TU32* texd;
		TSun::TF32 texStartU;
		TSun::TF32 texEndU;
		TSun::TF32 texStartV;
		TSun::TF32 texEndV;
		TSun::TU32 pageIndex;
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
		TSun::TBOOL InitFont(const TSun::TCHAR* filename,TSun::TU32 size,TSun::TU32 texPageSize,TSun::TS32 id,FT_Library lib);

		//! draws an text and clips it to the specified rectangle if wanted
		TSun::TVOID PreDraw(TSun::TS32 n,TSun::TS32* imgw,TSun::TS32* imgh,TSun::TS32* texw,TSun::TS32* texh,TSun::TS32* offx,TSun::TS32* offy,
			TSun::TF32* texStartU,TSun::TF32* texEndU,TSun::TF32* texStartV,TSun::TF32* texEndV,TSun::TU32* pageIndex);

		//! returns the dimension of a text
		TSun::TVOID GetDimension(const TSun::TWCHAR* text, TSun::TU32& left, TSun::TU32& right, TSun::TU32& top, TSun::TU32& bottom);

		//! Calculates the index of the character in the text which is on a specific position.
		TSun::TS32 GetCharacterFromPos(const TSun::TWCHAR* text, TSun::TS32 pixel_x);

		TSun::TS32 GetWidthFromCharacter(TSun::TWCHAR c);
		TSun::TU32 GetGlyphByChar(TSun::TWCHAR c,TSun::TBOOL& newFontCached);

		// catch all font to one dsv
		TSun::TBOOL catchAllFont();

		inline TSun::TS32 GetId()
		{
			return m_Id;
		}

		RenderDeviceUsedSRV* getSRV(TSun::TU32 pageIndex);
	private:
		FontGlyph* m_Glyphs;
		FT_Library	library;
		FT_Face		face;
		TSun::TS32			m_Id;	// 全局唯一ID
		//RenderDeviceUsedSRV* mSRV;
		//TSun::TU32 mTotalWidth;
		//TSun::TU32 mTotalHeight;
		//TSun::TU32* mPixelBuffer;
		TSun::TU32 mFontSize;

		TSun::TU32 mPageCount;
		TSun::TU32 mPageSize;
		TSun::List<TSun::TU32*> mPageBufferList;
		TSun::List<RenderDeviceUsedSRV*> mSRVList;
	};

}

#endif