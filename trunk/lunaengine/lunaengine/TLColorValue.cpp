#include "TLColorValue.h"
#include "TLFPU.h"

namespace TLunaEngine
{
	const ColorValue ColorValue::ZERO = ColorValue(0.0, 0.0, 0.0, 0.0);
	const ColorValue ColorValue::Black = ColorValue(0.0, 0.0, 0.0);
	const ColorValue ColorValue::White = ColorValue(1.0, 1.0, 1.0);
	const ColorValue ColorValue::Red = ColorValue(1.0, 0.0, 0.0);
	const ColorValue ColorValue::Green = ColorValue(0.0, 1.0, 0.0);
	const ColorValue ColorValue::Blue = ColorValue(0.0, 0.0, 1.0);

	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	ABGR ColorValue::getAsABGR(TVOID) const
//#else
	RGBA ColorValue::getAsRGBA(TVOID) const
//#endif
	{
		TUByte val8;
		TU32 val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		// Red
		val8 = static_cast<TUByte>(r * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<TUByte>(g * 255);
		val32 += val8 << 16;

		// Blue
		val8 = static_cast<TUByte>(b * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<TUByte>(a * 255);
		val32 += val8;

		return val32;
	}
	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	BGRA ColorValue::getAsBGRA(TVOID) const
//#else
	ARGB ColorValue::getAsARGB(TVOID) const
//#endif
	{
		TUByte val8;
		TU32 val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Alpha
		val8 = static_cast<TUByte>(a * 255);
		val32 = val8 << 24;

		// Red
		val8 = static_cast<TUByte>(r * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<TUByte>(g * 255);
		val32 += val8 << 8;

		// Blue
		val8 = static_cast<TUByte>(b * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	ARGB ColorValue::getAsARGB(TVOID) const
//#else
	BGRA ColorValue::getAsBGRA(TVOID) const
//#endif
	{
		TUByte val8;
		TU32 val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Blue
		val8 = static_cast<TUByte>(b * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<TUByte>(g * 255);
		val32 += val8 << 16;

		// Red
		val8 = static_cast<TUByte>(r * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<TUByte>(a * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	RGBA ColorValue::getAsRGBA(TVOID) const
//#else
	ABGR ColorValue::getAsABGR(TVOID) const
//#endif
	{
		TUByte val8;
		TU32 val32 = 0;

		// Convert to 32bit pattern
		// (ABRG = 8888)

		// Alpha
		val8 = static_cast<TUByte>(a * 255);
		val32 = val8 << 24;

		// Blue
		val8 = static_cast<TUByte>(b * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<TUByte>(g * 255);
		val32 += val8 << 8;

		// Red
		val8 = static_cast<TUByte>(r * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	TVOID ColorValue::setAsABGR(const ABGR val)
//#else
	TVOID ColorValue::setAsRGBA(const RGBA val)
//#endif
	{
		TU32 val32 = val;

		// Convert from 32bit pattern
		// (RGBA = 8888)

		// Red
		r = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	TVOID ColorValue::setAsBGRA(const BGRA val)
//#else
	TVOID ColorValue::setAsARGB(const ARGB val)
//#endif
	{
		TU32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		b = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	TVOID ColorValue::setAsARGB(const ARGB val)
//#else
	TVOID ColorValue::setAsBGRA(const BGRA val)
//#endif
	{
		TU32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Blue
		b = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
//	TVOID ColorValue::setAsRGBA(const RGBA val)
//#else
	TVOID ColorValue::setAsABGR(const ABGR val)
//#endif
	{
		TU32 val32 = val;

		// Convert from 32bit pattern
		// (ABGR = 8888)

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Red
		r = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
	TBOOL ColorValue::operator==(const ColorValue& rhs) const
	{
		return (r == rhs.r &&
			g == rhs.g &&
			b == rhs.b &&
			a == rhs.a);
	}
	//---------------------------------------------------------------------
	TBOOL ColorValue::operator!=(const ColorValue& rhs) const
	{
		return !(*this == rhs);
	}
	//---------------------------------------------------------------------
	TVOID ColorValue::setHSB(TF32 hue, TF32 saturation, TF32 brightness)
	{
		// wrap hue
		if (hue > 1.0f)
		{
			hue -= (TS32)hue;
		}
		else if (hue < 0.0f)
		{
			hue += (TS32)hue + 1;
		}
		// clamp saturation / brightness
		saturation = TLunaEngine::min_<TF32>(saturation, (TF32)1.0);
		saturation = TLunaEngine::max_<TF32>(saturation, (TF32)0.0);
		brightness = TLunaEngine::min_<TF32>(brightness, (TF32)1.0);
		brightness = TLunaEngine::max_<TF32>(brightness, (TF32)0.0);

		if (brightness == 0.0f)
		{
			// early exit, this has to be black
			r = g = b = 0.0f;
			return;
		}

		if (saturation == 0.0f)
		{
			// early exit, this has to be grey

			r = g = b = brightness;
			return;
		}


		TF32 hueDomain = hue * 6.0f;
		if (hueDomain >= 6.0f)
		{
			// wrap around, and allow mathematical errors
			hueDomain = 0.0f;
		}
		TU16 domain = (TU16)hueDomain;
		TF32 f1 = brightness * (1 - saturation);
		TF32 f2 = brightness * (1 - saturation * (hueDomain - domain));
		TF32 f3 = brightness * (1 - saturation * (1 - (hueDomain - domain)));

		switch (domain)
		{
		case 0:
			// red domain; green ascends
			r = brightness;
			g = f3;
			b = f1;
			break;
		case 1:
			// yellow domain; red descends
			r = f2;
			g = brightness;
			b = f1;
			break;
		case 2:
			// green domain; blue ascends
			r = f1;
			g = brightness;
			b = f3;
			break;
		case 3:
			// cyan domain; green descends
			r = f1;
			g = f2;
			b = brightness;
			break;
		case 4:
			// blue domain; red ascends
			r = f3;
			g = f1;
			b = brightness;
			break;
		case 5:
			// magenta domain; blue descends
			r = brightness;
			g = f1;
			b = f2;
			break;
		}


	}
	//---------------------------------------------------------------------
	TVOID ColorValue::getHSB(TF32* hue, TF32* saturation, TF32* brightness) const
	{

		TF32 vMin = TLunaEngine::min_<TF32>(r, TLunaEngine::min_<TF32>(g, b));
		TF32 vMax = TLunaEngine::max_<TF32>(r, TLunaEngine::max_<TF32>(g, b));
		TF32 delta = vMax - vMin;

		*brightness = vMax;

		if (TLunaEngine::equals(delta, 0.0f))
		{
			// grey
			*hue = 0;
			*saturation = 0;
		}
		else
		{
			// a colour
			*saturation = delta / vMax;

			TF32 deltaR = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
			TF32 deltaG = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
			TF32 deltaB = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;

			if (TLunaEngine::equals(r, vMax))
				*hue = deltaB - deltaG;
			else if (TLunaEngine::equals(g, vMax))
				*hue = 0.3333333f + deltaR - deltaB;
			else if (TLunaEngine::equals(b, vMax))
				*hue = 0.6666667f + deltaG - deltaR;

			if (*hue < 0.0f)
				*hue += 1.0f;
			if (*hue > 1.0f)
				*hue -= 1.0f;
		}


	}
}