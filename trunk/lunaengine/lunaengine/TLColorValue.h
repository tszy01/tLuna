#ifndef _TLCOLORVALUE_H_
#define _TLCOLORVALUE_H_

#include "TLCommonTypeDef.h"
#include <assert.h>

namespace TLunaEngine
{
	// color value class from ogre

	typedef TU32 RGBA;
	typedef TU32 ARGB;
	typedef TU32 ABGR;
	typedef TU32 BGRA;

	/** Class representing colour.
	@remarks
	Colour is represented as 4 components, each of which is a
	floating-point value from 0.0 to 1.0.
	@par
	The 3 'normal' colour components are red, green and blue, a higher
	number indicating greater amounts of that component in the colour.
	The forth component is the 'alpha' value, which represents
	transparency. In this case, 0.0 is completely transparent and 1.0 is
	fully opaque.
	*/
	class ColorValue
	{
	public:
		static const ColorValue ZERO;
		static const ColorValue Black;
		static const ColorValue White;
		static const ColorValue Red;
		static const ColorValue Green;
		static const ColorValue Blue;

		explicit ColorValue(TF32 red = 1.0f,
			TF32 green = 1.0f,
			TF32 blue = 1.0f,
			TF32 alpha = 1.0f) : r(red), g(green), b(blue), a(alpha)
		{ }

		TBOOL operator==(const ColorValue& rhs) const;
		TBOOL operator!=(const ColorValue& rhs) const;

		TF32 r, g, b, a;

		/** Retrieves colour as RGBA.
		*/
		RGBA getAsRGBA(TVOID) const;

		/** Retrieves colour as ARGB.
		*/
		ARGB getAsARGB(TVOID) const;

		/** Retrieves colour as BGRA.
		*/
		BGRA getAsBGRA(TVOID) const;

		/** Retrieves colours as ABGR */
		ABGR getAsABGR(TVOID) const;

		/** Sets colour as RGBA.
		*/
		TVOID setAsRGBA(const RGBA val);

		/** Sets colour as ARGB.
		*/
		TVOID setAsARGB(const ARGB val);

		/** Sets colour as BGRA.
		*/
		TVOID setAsBGRA(const BGRA val);

		/** Sets colour as ABGR.
		*/
		TVOID setAsABGR(const ABGR val);

		/** Clamps colour value to the range [0, 1].
		*/
		TVOID saturate(TVOID)
		{
			if (r < 0)
				r = 0;
			else if (r > 1)
				r = 1;

			if (g < 0)
				g = 0;
			else if (g > 1)
				g = 1;

			if (b < 0)
				b = 0;
			else if (b > 1)
				b = 1;

			if (a < 0)
				a = 0;
			else if (a > 1)
				a = 1;
		}

		/** As saturate, except that this colour value is unaffected and
		the saturated colour value is returned as a copy. */
		ColorValue saturateCopy(TVOID) const
		{
			ColorValue ret = *this;
			ret.saturate();
			return ret;
		}

		/// Array accessor operator
		inline TF32 operator [] (const TSIZE i) const
		{
			assert(i < 4);

			return *(&r + i);
		}

		/// Array accessor operator
		inline TF32& operator [] (const TSIZE i)
		{
			assert(i < 4);

			return *(&r + i);
		}

		/// Pointer accessor for direct copying
		inline TF32* ptr()
		{
			return &r;
		}
		/// Pointer accessor for direct copying
		inline const TF32* ptr() const
		{
			return &r;
		}


		// arithmetic operations
		inline ColorValue operator + (const ColorValue& rkVector) const
		{
			ColorValue kSum;

			kSum.r = r + rkVector.r;
			kSum.g = g + rkVector.g;
			kSum.b = b + rkVector.b;
			kSum.a = a + rkVector.a;

			return kSum;
		}

		inline ColorValue operator - (const ColorValue& rkVector) const
		{
			ColorValue kDiff;

			kDiff.r = r - rkVector.r;
			kDiff.g = g - rkVector.g;
			kDiff.b = b - rkVector.b;
			kDiff.a = a - rkVector.a;

			return kDiff;
		}

		inline ColorValue operator * (const TF32 fScalar) const
		{
			ColorValue kProd;

			kProd.r = fScalar*r;
			kProd.g = fScalar*g;
			kProd.b = fScalar*b;
			kProd.a = fScalar*a;

			return kProd;
		}

		inline ColorValue operator * (const ColorValue& rhs) const
		{
			ColorValue kProd;

			kProd.r = rhs.r * r;
			kProd.g = rhs.g * g;
			kProd.b = rhs.b * b;
			kProd.a = rhs.a * a;

			return kProd;
		}

		inline ColorValue operator / (const ColorValue& rhs) const
		{
			ColorValue kProd;

			kProd.r = r / rhs.r;
			kProd.g = g / rhs.g;
			kProd.b = b / rhs.b;
			kProd.a = a / rhs.a;

			return kProd;
		}

		inline ColorValue operator / (const TF32 fScalar) const
		{
			assert(fScalar != 0.0);

			ColorValue kDiv;

			TF32 fInv = 1.0f / fScalar;
			kDiv.r = r * fInv;
			kDiv.g = g * fInv;
			kDiv.b = b * fInv;
			kDiv.a = a * fInv;

			return kDiv;
		}

		inline friend ColorValue operator * (const TF32 fScalar, const ColorValue& rkVector)
		{
			ColorValue kProd;

			kProd.r = fScalar * rkVector.r;
			kProd.g = fScalar * rkVector.g;
			kProd.b = fScalar * rkVector.b;
			kProd.a = fScalar * rkVector.a;

			return kProd;
		}

		// arithmetic updates
		inline ColorValue& operator += (const ColorValue& rkVector)
		{
			r += rkVector.r;
			g += rkVector.g;
			b += rkVector.b;
			a += rkVector.a;

			return *this;
		}

		inline ColorValue& operator -= (const ColorValue& rkVector)
		{
			r -= rkVector.r;
			g -= rkVector.g;
			b -= rkVector.b;
			a -= rkVector.a;

			return *this;
		}

		inline ColorValue& operator *= (const TF32 fScalar)
		{
			r *= fScalar;
			g *= fScalar;
			b *= fScalar;
			a *= fScalar;
			return *this;
		}

		inline ColorValue& operator /= (const TF32 fScalar)
		{
			assert(fScalar != 0.0);

			TF32 fInv = 1.0f / fScalar;

			r *= fInv;
			g *= fInv;
			b *= fInv;
			a *= fInv;

			return *this;
		}

		/** Set a colour value from Hue, Saturation and Brightness.
		@param hue Hue value, scaled to the [0,1] range as opposed to the 0-360
		@param saturation Saturation level, [0,1]
		@param brightness Brightness level, [0,1]
		*/
		TVOID setHSB(TF32 hue, TF32 saturation, TF32 brightness);

		/** Convert the current colour to Hue, Saturation and Brightness values.
		@param hue Output hue value, scaled to the [0,1] range as opposed to the 0-360
		@param saturation Output saturation level, [0,1]
		@param brightness Output brightness level, [0,1]
		*/
		TVOID getHSB(TF32* hue, TF32* saturation, TF32* brightness) const;

	};
}

#endif