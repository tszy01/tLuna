#ifndef _TLFPU_H_
#define _TLFPU_H_


#include "TLCommonTypeDef.h"
#include <math.h>

/**
*	使用irrlicht的Math方法
*/

namespace TLunaEngine{

	//! Enumeration for intersection relations of 3d objects
	enum EIntersectionRelation3D
	{
		ISREL3D_FRONT = 0,
		ISREL3D_BACK,
		ISREL3D_PLANAR,
		ISREL3D_SPANNING,
		ISREL3D_CLIPPED
	};

	//! Rounding error constant often used when comparing TF32 values.
	const TF32 ROUNDING_ERROR_32 = 0.00005f;
	const TF64 ROUNDING_ERROR_64 = 0.000005;

	const TF64 T_FLT_MIN = 1.175494351e-38F;
	const TF64 T_FLT_MAX = 3.402823466e+38F;

	#ifdef PI // make sure we don't collide with a define
	#undef PI
	#endif
		
	//! Constant for PI.
	const TF32 PI		= 3.14159265359f;

	//! Constant for reciprocal of PI.
	const TF32 RECIPROCAL_PI	= 1.0f/PI;

	//! Constant for half of PI.
	const TF32 HALF_PI	= PI/2.0f;

	#ifdef PI64 // make sure we don't collide with a define
	#undef PI64
	#endif
		
	//! Constant for 64bit PI.
	const TF64 PI64		= 3.1415926535897932384626433832795028841971693993751;

	//! Constant for 64bit reciprocal of PI.
	const TF64 RECIPROCAL_PI64 = 1.0/PI64;

	//! 32bit Constant for converting from degrees to radians
	const TF32 DEGTORAD = PI / 180.0f;

	//! 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
	const TF32 RADTODEG   = 180.0f / PI;

	//! 64bit constant for converting from degrees to radians (formally known as GRAD_PI2)
	const TF64 DEGTORAD64 = PI64 / 180.0;

	//! 64bit constant for converting from radians to degrees
	const TF64 RADTODEG64 = 180.0 / PI64;

	//! returns minimum of two values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline T min_(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	//! returns minimum of three values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline T min_(const T& a, const T& b, const T& c)
	{
		return a < b ? min_(a, c) : min_(b, c);
	}

	//! returns maximum of two values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline  T max_(const  T& a, const T& b)
	{
		return a < b ? b : a;
	}

	//! returns maximum of three values. Own implementation to get rid of the STL (VS6 problems)
	template<class T>
	inline  T max_(const T& a, const T& b, const T& c)
	{
		return a < b ? max_(b, c) : max_(a, c);
	}

	//! returns abs of two values. Own implementation to get rid of STL (VS6 problems)
	template<class T>
	inline T abs_(const T& a)
	{
		return a < (T)0 ? -a : a;
	}

	// sin(x)
	template<class T>
	inline T sin_(const T& a)
	{
		return std::sin(a);
	}

	// cos(x)
	template<class T>
	inline T cos_(const T& a)
	{
		return std::cos(a);
	}

	// tan(x)
	template<class T>
	inline T tan_(const T& a)
	{
		return std::tan(a);
	}

	// asin(x)
	template<class T>
	inline T asin_(const T& a)
	{
		return std::asin(a);
	}

	// acos(x)
	template<class T>
	inline T acos_(const T& a)
	{
		return std::acos(a);
	}

	// atan(x)
	template<class T>
	inline T atan_(const T& a)
	{
		return std::atan(a);
	}

	// atan(x,y)
	template<class T>
	inline T atan2_(const T& a, const T& b)
	{
		return std::atan2(a,b);
	}

	//! returns linear interpolation of a and b with ratio t
	//! \return: a if t==0, b if t==1, and the linear interpolation else
	template<class T>
	inline T lerp(const T& a, const T& b, const TF32 t)
	{
		return (T)(a*(1.f-t)) + (b*t);
	}

	//! clamps a value between low and high
	template <class T>
	inline  T clamp(const T& value, const T& low, const T& high)
	{
		return min_ (max_(value,low), high);
	}

	// is nan
	template <class T>
	inline TBOOL isNaN(const T& f)
	{
		// std::isnan() is C99, not supported by all compilers
		// However NaN always fails this next test, no other number does.
		return f != f;
	}

	//! returns if a equals b, taking possible rounding errors into account
	inline TBOOL equals( TF32 a,  TF32 b,  TF32 tolerance = ROUNDING_ERROR_32)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! returns if a equals b, taking possible rounding errors into account
	inline TBOOL equals( TF64 a,  TF64 b,  TF64 tolerance = ROUNDING_ERROR_64)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! returns if a equals b, taking possible rounding errors into account
	inline TBOOL equals( TS32 a,  TS32 b,  TS32 tolerance = 0)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! returns if a equals b, taking possible rounding errors into account
	inline TBOOL equals( TU32 a,  TU32 b,  TU32 tolerance = 0)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	//! returns if a equals zero, taking rounding errors into account
	inline TBOOL iszero( TF32 a,  TF32 tolerance = ROUNDING_ERROR_32)
	{
		return fabsf ( a ) <= tolerance;
	}

	//! returns if a equals zero, taking rounding errors into account
	inline TBOOL iszero( TF64 a,  TF64 tolerance = ROUNDING_ERROR_64)
	{
		return fabs ( a ) <= tolerance;
	}

	//! returns if a equals zero, taking rounding errors into account
	inline TBOOL iszero( TS32 a,  TS32 tolerance = 0)
	{
		return ( a & 0x7ffffff ) <= tolerance;
	}

	//! returns if a equals zero, taking rounding errors into account
	inline TBOOL iszero( TU32 a,  TU32 tolerance = 0)
	{
		return a <= tolerance;
	}

	inline TS32 s32_min ( TS32 a, TS32 b)
	{
		TS32 mask = (a - b) >> 31;
		return (a & mask) | (b & ~mask);
	}

	inline TS32 s32_max ( TS32 a, TS32 b)
	{
		TS32 mask = (a - b) >> 31;
		return (b & mask) | (a & ~mask);
	}

	inline TS32 s32_clamp (TS32 value, TS32 low, TS32 high)
	{
		return s32_min (s32_max(value,low), high);
	}

	// ------------------------ 以下都是将符点转换成整数来运算 ------------------------

	/*
		TF32 IEEE-754 bit represenation

		0      0x00000000
		1.0    0x3f800000
		0.5    0x3f000000
		3      0x40400000
		+inf   0x7f800000
		-inf   0xff800000
		+NaN   0x7fc00000 or 0x7ff00000
		in general: number = (sign ? -1:1) * 2^(exponent) * 1.(mantissa bits)
	*/

	//#define F32_2_INT32(f)		(*((TS32 *) &(f)))
	template<typename T>
	TS32 F32_2_INT32(T f)
	{
		return (*((TS32 *) &(f)));
	}

	//#define F32_2_UINT32(f)		(*((TU32 *) &(f)))
	template<typename T>
	TU32 F32_2_UINT32(T f)
	{
		return (*((TU32 *) &(f)));
	}

	//#define F32_2_UINT32_POINTER(f)	( ((TU32 *) &(f)))
	template<typename T>
	TU32* F32_2_UINT32_POINTER(T f)
	{
		return ( ((TU32 *) &(f)));
	}

	// 以下定义不要改成const
	#define F32_VALUE_0		0x00000000
	#define F32_VALUE_1		0x3f800000
	#define F32_SIGN_BIT		0x80000000U
	#define F32_EXPON_MANTISSA	0x7FFFFFFFU
	//! integer representation of 1.0
	#define IEEE_1_0			0x3f800000
	//! integer representation of 255.0
	#define IEEE_255_0			0x437f0000

	//! code is taken from IceFPU
	//! Integer representation of a floating-point value.
	template<typename T>
	TU32 IR(T x)
	{
		return ((TU32&)(x));
	}

	//! Absolute integer representation of a floating-point value
	template<typename T>
	TS32 AIR(T x)
	{
		return (IR(x)&0x7fffffff);
	}

	//! Floating-point representation of an integer value.
	template<typename T>
	TF32 FR(T x)
	{
		return ((TF32&)(x));
	}

	template<typename T>
	TBOOL F32_LOWER_0(T f)
	{
		return (F32_2_UINT32(f) >  F32_SIGN_BIT);
	}

	template<typename T>
	TBOOL F32_LOWER_EQUAL_0(T f)
	{
		return (F32_2_INT32(f) <= F32_VALUE_0);
	}

	template<typename T>
	TBOOL F32_GREATER_0(T f)
	{
		return (F32_2_INT32(f) >  F32_VALUE_0);
	}

	template<typename T>
	TBOOL F32_GREATER_EQUAL_0(T f)
	{
		return (F32_2_UINT32(f) <= F32_SIGN_BIT);
	}

	template<typename T>
	TBOOL F32_EQUAL_1(T f)
	{
		return (F32_2_UINT32(f) == F32_VALUE_1);
	}

	template<typename T>
	TBOOL F32_EQUAL_0(T f)
	{
		return ( (F32_2_UINT32(f) & F32_EXPON_MANTISSA ) == F32_VALUE_0);
	}

	// only same sign
	template<typename T>
	TBOOL F32_A_GREATER_B(T a,T b)
	{
		return (F32_2_INT32((a)) > F32_2_INT32((b)));
	}

	//! Integer-based comparison of a floating point value.
	//! Don't use it blindly, it can be faster or slower than the FPU comparison, depends on the context.
	template<typename T>
	TBOOL IS_NEGATIVE_FLOAT(T x)
	{
		return (IR(x)&0x80000000);
	}

	// ----------------------------------------------------------------------------


	//! conditional set based on mask and arithmetic shift
	inline  TU32 if_c_a_else_b (  TS32 condition,  TU32 a,  TU32 b )
	{
		return ( ( -condition >> 31 ) & ( a ^ b ) ) ^ b;
	}

	//! conditional set based on mask and arithmetic shift
	inline  TU32 if_c_a_else_0 (  TS32 condition,  TU32 a )
	{
		return ( -condition >> 31 ) & a;
	}

	/*
		if (condition) state |= m; else state &= ~m;
	*/
	inline  TVOID setbit_cond ( TU32 &state, TS32 condition, TU32 mask )
	{
		// 0, or any postive to mask
		//TS32 conmask = -condition >> 31;
		state ^= ( ( -condition >> 31 ) ^ state ) & mask;
	}

	// 比x+0.5小的最大整数
	inline  TF32 round_( TF32 x )
	{
		return floorf( x + 0.5f );
	}

	inline  TVOID clearFPUException ()
	{
	#if defined(_MSC_VER) && !defined(_WIN64)
		__asm fnclex;
	#endif
	}

	// 开方的倒数
	inline TF32 reciprocal_squareroot( TF32 x)
	{
		// comes from NVIDIA
	#if defined(_MSC_VER) && !defined(_WIN64)
		// an sse2 version
		__asm
		{
			movss	xmm0, x
			rsqrtss	xmm0, xmm0
			movss	x, xmm0
		}
		return x;
	#else // no fast math
		return 1.f / sqrtf ( x );
	#endif
	}


	// 倒数
	inline TF32 reciprocal (  TF32 f )
	{
		return 1.f / f;
	}

	// 倒数快速算法
	inline TF32 reciprocal_approxim (  TF32 p )
	{
		register TU32 x = 0x7F000000 - IR ( p );
		TF32 r = FR ( x );
		return r * (2.0f - p * r);
	}

	// 比x小的最大整数
	inline TS32 floor32(TF32 x)
	{
	#if defined(_MSC_VER) && !defined(_WIN64)
		TF32 h = 0.5f;
		TS32 t;
		__asm
		{
			fld	x
			fsub	h
			fistp	t
		}
		return t;
	#else // no fast math
		return (TS32) floorf ( x );
	#endif
	}

	// 比x大的最小整数
	inline TS32 ceil32 ( TF32 x )
	{
	#if defined(_MSC_VER) && !defined(_WIN64)
		TF32 h = 0.5f;
		TS32 t;
		__asm
		{
			fld	x
			fadd	h
			fistp	t
		}
		return t;
	#else // not fast math
		return (TS32) ceilf ( x );
	#endif
	}

	// 比x+0.5小的最大整数
	inline TS32 round32(TF32 x)
	{
	#if defined(_MSC_VER) && !defined(_WIN64)
		TS32 t;
		__asm
		{
			fld   x
			fistp t
		}
		return t;
	#else // no fast math
		return (TS32) round_(x);
	#endif
	}

	inline TF32 f32_max3( TF32 a,  TF32 b,  TF32 c)
	{
		return a > b ? (a > c ? a : c) : (b > c ? b : c);
	}

	inline TF32 f32_min3( TF32 a,  TF32 b,  TF32 c)
	{
		return a < b ? (a < c ? a : c) : (b < c ? b : c);
	}

	// x减去比x小的最大整数
	inline TF32 fract ( TF32 x )
	{
		return x - floorf ( x );
	}

	//! Fast fabs for floating-point values. It just clears the sign bit.
	//! Don't use it blindy, it can be faster or slower than the FPU comparison, depends on the context.
	inline TF32 FastFabs(TF32 x)
	{
		TU32 FloatBits = IR(x)&0x7fffffff;
		return FR(FloatBits);
	}

	//! Fast square root for floating-point values.
	inline TF32 FastSqrt(TF32 square)
	{
		return sqrtf(square);
	}

	//! Saturates positive to zero.
	inline TF32 Fastsat(TF32 f)
	{
		TU32 y = (TU32&)f & ~((TS32&)f >>31);
		return (TF32&)y;
	}

	//! Computes 1.0f / sqrtf(x).
	inline TF32 Fastrsqrt(TF32 f)
	{
		TF32 x = f * 0.5f;
		TU32 y = 0x5f3759df - ((TU32&)f >> 1);
		// Iteration...
		(TF32&)y  = (TF32&)y * ( 1.5f - ( x * (TF32&)y * (TF32&)y ) );
		// Result
		return (TF32&)y;
	}

	//! Computes 1.0f / sqrtf(x). Comes from NVIDIA.
	inline TF32 FastInvSqrt(const TF32& x)
	{
		TU32 tmp = (TU32(IEEE_1_0 << 1) + IEEE_1_0 - *(TU32*)&x) >> 1;   
		TF32 y = *(TF32*)&tmp;                                             
		return y * (1.47f - 0.47f * x * y * y);
	}

	//! Computes 1.0f / sqrtf(x). Comes from Quake3. Looks like the first one I had above.
	//! See http://www.magic-software.com/3DGEDInvSqrt.html
	inline TF32 FastRSqrt(TF32 number)
	{
		TS32 i;
		TF32 x2, y;
		const TF32 threehalfs = 1.5f;

		x2 = number * 0.5f;
		y  = number;
		i  = * (TS32 *) &y;
		i  = 0x5f3759df - (i >> 1);
		y  = * (TF32 *) &i;
		y  = y * (threehalfs - (x2 * y * y));

		return y;
	}

	//! TO BE DOCUMENTED
	inline TF32 Fastsqrt(TF32 f)
	{
		TU32 y = ( ( (TS32&)f - 0x3f800000 ) >> 1 ) + 0x3f800000;
		// Iteration...?
		// (TF32&)y = (3.0f - ((TF32&)y * (TF32&)y) / f) * (TF32&)y * 0.5f;
		// Result
		return (TF32&)y;
	}

	//! Returns the TF32 ranged espilon value.
	inline TF32 Fastepsilon(TF32 f)
	{
		TU32 b = (TU32&)f & 0xff800000;
		TU32 a = b | 0x00000001;
		(TF32&)a -= (TF32&)b;
		// Result
		return (TF32&)a;
	}

	//---------------------------- bitwise from ogre ------------------------------------

	/** Returns the most significant bit set in a value.
    */
    inline TU32 mostSignificantBitSet(TU32 value)
    {
        TU32 result = 0;
        while (value != 0) {
            ++result;
            value >>= 1;
        }
        return result-1;
    }
    /** Returns the closest power-of-two number greater or equal to value.
        @note 0 and 1 are powers of two, so 
            firstPO2From(0)==0 and firstPO2From(1)==1.
    */
    inline TU32 firstPO2From(TU32 n)
    {
        --n;            
        n |= n >> 16;
        n |= n >> 8;
        n |= n >> 4;
        n |= n >> 2;
        n |= n >> 1;
        ++n;
        return n;
    }
    /** Determines whether the number is power-of-two or not.
        @note 0 and 1 are tread as power of two.
    */
    template<typename T>
    inline TBOOL isPO2(T n)
    {
        return (n & (n-1)) == 0;
    }
    /** Returns the number of bits a pattern must be shifted right by to
        remove right-hand zeros.
    */
	template<typename T>
    inline TU32 getBitShift(T mask)
	{
		if (mask == 0)
			return 0;

		TU32 result = 0;
		while ((mask & 1) == 0) {
			++result;
			mask >>= 1;
		}
		return result;
	}

    /** Takes a value with a given src bit mask, and produces another
        value with a desired bit mask.
        @remarks
            This routine is useful for colour conversion.
    */
	template<typename SrcT, typename DestT>
    inline DestT convertBitPattern(SrcT srcValue, SrcT srcBitMask, DestT destBitMask)
	{
		// Mask off irrelevant source value bits (if any)
		srcValue = srcValue & srcBitMask;

		// Shift source down to bottom of TU32
		const TU32 srcBitShift = getBitShift(srcBitMask);
		srcValue >>= srcBitShift;

		// Get max value possible in source from srcMask
		const SrcT srcMax = srcBitMask >> srcBitShift;

		// Get max available in dest
		const TU32 destBitShift = getBitShift(destBitMask);
		const DestT destMax = destBitMask >> destBitShift;

		// Scale source value into destination, and shift back
		DestT destValue = (srcValue * destMax) / srcMax;
		return (destValue << destBitShift);
	}

    /**
        * Convert N bit colour channel value to P bits. It fills P bits with the
        * bit pattern repeated. (this is /((1<<n)-1) in fixed point)
        */
    inline TU32 fixedToFixed(TU32 value, TU32 n, TU32 p) 
    {
        if(n > p) 
        {
            // Less bits required than available; this is easy
            value >>= n-p;
        } 
        else if(n < p)
        {
            // More bits required than are there, do the fill
            // Use old fashioned division, probably better than a loop
            if(value == 0)
                    value = 0;
            else if(value == (static_cast<TU32>(1)<<n)-1)
                    value = (1<<p)-1;
            else    value = value*(1<<p)/((1<<n)-1);
        }
        return value;    
    }

    /**
        * Convert floating point colour channel value between 0.0 and 1.0 (otherwise clamped) 
        * to integer of a certain number of bits. Works for any value of bits between 0 and 31.
        */
    inline TU32 floatToFixed(const TF32 value, const TU32 bits)
    {
        if(value <= 0.0f) return 0;
        else if (value >= 1.0f) return (1<<bits)-1;
        else return (TU32)(value * (1<<bits));     
    }

    /**
        * Fixed point to TF32
        */
    inline TF32 fixedToFloat(unsigned value, TU32 bits)
    {
        return (TF32)value/(TF32)((1<<bits)-1);
    }

    /**
        * Write a n*8 bits integer value to memory in native endian.
        */
    inline TVOID intWrite(TVOID *dest, const TS32 n, const TU32 value)
    {
        switch(n) {
            case 1:
                ((TUByte*)dest)[0] = (TUByte)value;
                break;
            case 2:
                ((TU16*)dest)[0] = (TU16)value;
                break;
            case 3:
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG      
                ((TUByte*)dest)[0] = (TUByte)((value >> 16) & 0xFF);
                ((TUByte*)dest)[1] = (TUByte)((value >> 8) & 0xFF);
                ((TUByte*)dest)[2] = (TUByte)(value & 0xFF);
//#else
//                ((TUByte*)dest)[2] = (TUByte)((value >> 16) & 0xFF);
//                ((TUByte*)dest)[1] = (TUByte)((value >> 8) & 0xFF);
//                ((TUByte*)dest)[0] = (TUByte)(value & 0xFF);
//#endif
                break;
            case 4:
                ((TU32*)dest)[0] = (TU32)value;                
                break;                
        }        
    }
    /**
        * Read a n*8 bits integer value to memory in native endian.
        */
    inline TU32 intRead(const TVOID *src, TS32 n) {
        switch(n) {
            case 1:
                return ((const TUByte*)src)[0];
            case 2:
                return ((const TU16*)src)[0];
            case 3:
//#if OGRE_ENDIAN == OGRE_ENDIAN_BIG      
                return ((TU32)((const TUByte*)src)[0]<<16)|
                        ((TU32)((const TUByte*)src)[1]<<8)|
                        ((TU32)((const TUByte*)src)[2]);
//#else
//                return ((TU32)((const TUByte*)src)[0])|
//                        ((TU32)((const TUByte*)src)[1]<<8)|
//                        ((TU32)((const TUByte*)src)[2]<<16);
//#endif
            case 4:
                return ((const TU32*)src)[0];
        } 
        return 0; // ?
    }

	/** Converts TF32 in TU32 format to a a half in TU16 format
	*/
    inline TU16 floatToHalfI(TU32 i)
    {
        register TS32 s =  (i >> 16) & 0x00008000;
        register TS32 e = ((i >> 23) & 0x000000ff) - (127 - 15);
        register TS32 m =   i        & 0x007fffff;
        
        if (e <= 0)
        {
            if (e < -10)
            {
                return 0;
            }
            m = (m | 0x00800000) >> (1 - e);
        
            return static_cast<TU16>(s | (m >> 13));
        }
        else if (e == 0xff - (127 - 15))
        {
            if (m == 0) // Inf
            {
                return static_cast<TU16>(s | 0x7c00);
            } 
            else    // NAN
            {
                m >>= 13;
                return static_cast<TU16>(s | 0x7c00 | m | (m == 0));
            }
        }
        else
        {
            if (e > 30) // Overflow
            {
                return static_cast<TU16>(s | 0x7c00);
            }
        
            return static_cast<TU16>(s | (e << 10) | (m >> 13));
        }
    }

	/** Convert a float32 to a float16 (NV_half_float)
        Courtesy of OpenEXR
    */
    inline TU16 floatToHalf(TF32 i)
    {
        union { TF32 f; TU32 i; } v;
        v.f = i;
        return floatToHalfI(v.i);
    }
        
	/** Converts a half in TU16 format to a TF32
		in TU32 format
		*/
    inline TU32 halfToFloatI(TU16 y)
    {
        register TS32 s = (y >> 15) & 0x00000001;
        register TS32 e = (y >> 10) & 0x0000001f;
        register TS32 m =  y        & 0x000003ff;
        
        if (e == 0)
        {
            if (m == 0) // Plus or minus zero
            {
                return s << 31;
            }
            else // Denormalized number -- renormalize it
            {
                while (!(m & 0x00000400))
                {
                    m <<= 1;
                    e -=  1;
                }
        
                e += 1;
                m &= ~0x00000400;
            }
        }
        else if (e == 31)
        {
            if (m == 0) // Inf
            {
                return (s << 31) | 0x7f800000;
            }
            else // NaN
            {
                return (s << 31) | 0x7f800000 | (m << 13);
            }
        }
        
        e = e + (127 - 15);
        m = m << 13;
        
        return (s << 31) | (e << 23) | m;
    }

	/**
        * Convert a float16 (NV_half_float) to a float32
        * Courtesy of OpenEXR
        */
    inline TF32 halfToFloat(TU16 y)
    {
        union { TF32 f; TU32 i; } v;
        v.i = halfToFloatI(y);
        return v.f;
    }

}

#endif