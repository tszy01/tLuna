#ifndef _TLVECTOR4_H_
#define _TLVECTOR4_H_

#include "TLFPU.h"
#include "TLVector3.h"

namespace TLunaEngine{

	/*
	 *	使用Ogre的Vector4
	 */
	template <typename T>
	class Vector4
	{
	public:

		Vector4(TVOID) : X(0),Y(0),Z(0),W(0)
		{
		}

		~Vector4(TVOID)
		{
		}
	public:
		T X,Y,Z,W;

	public:
		inline Vector4( T x,T y,T z,T w )
			: X( x ), Y( y ), Z( z ), W( w)
		{
		}

		inline Vector4( T afCoordinate[4] )
			: X( afCoordinate[0] ),
			Y( afCoordinate[1] ),
			Z( afCoordinate[2] ),
			W( afCoordinate[3] )
		{
		}

		inline Vector4(const Vector4<T>& right) : X(right.X),Y(right.Y),Z(right.Z),W(right.W)
		{
		}

		inline Vector4(const Vector3<T>& rhs)
			: X(rhs.X), Y(rhs.Y), Z(rhs.Z), W(1)
		{
		}

		inline T operator [] ( const TSIZE i ) const
		{
			return *(&X+i);
		}

		inline T& operator [] ( const TSIZE i )
		{
			return *(&X+i);
		}

		/// Pointer accessor for direct copying
		inline T* ptr()
		{
			return &X;
		}
		/// Pointer accessor for direct copying
		inline const T* ptr() const
		{
			return &X;
		}

		/** Assigns the value of the other vector.
		@param
		rkVector The other vector
		*/
		inline Vector4<T>& operator = ( const Vector4<T>& rkVector )
		{
			X = rkVector.X;
			Y = rkVector.Y;
			Z = rkVector.Z;
			W = rkVector.W;

			return *this;
		}

		inline Vector4<T>& operator = ( const T fScalar)
		{
			X = fScalar;
			Y = fScalar;
			Z = fScalar;
			W = fScalar;
			return *this;
		}

		inline TBOOL operator == ( const Vector4<T>& rkVector ) const
		{
			return ( X == rkVector.X &&
				Y == rkVector.Y &&
				Z == rkVector.Z &&
				W == rkVector.W );
		}

		inline TBOOL operator != ( const Vector4<T>& rkVector ) const
		{
			return ( X != rkVector.X ||
				Y != rkVector.Y ||
				Z != rkVector.Z ||
				W != rkVector.W );
		}

		inline Vector4<T>& operator = (const Vector3<T>& rhs)
		{
			X = rhs.X;
			Y = rhs.Y;
			Z = rhs.Z;
			W = 1;
			return *this;
		}

		// arithmetic operations
		inline Vector4<T> operator + ( const Vector4<T>& rkVector ) const
		{
			return Vector4<T>(
				X + rkVector.X,
				Y + rkVector.Y,
				Z + rkVector.Z,
				W + rkVector.W);
		}

		inline Vector4<T> operator - ( const Vector4<T>& rkVector ) const
		{
			return Vector4<T>(
				X - rkVector.X,
				Y - rkVector.Y,
				Z - rkVector.Z,
				W - rkVector.W);
		}

		inline Vector4<T> operator * ( const T fScalar ) const
		{
			return Vector4<T>(
				X * fScalar,
				Y * fScalar,
				Z * fScalar,
				W * fScalar);
		}

		inline Vector4<T> operator * ( const Vector4<T>& rhs) const
		{
			return Vector4<T>(
				rhs.X * X,
				rhs.Y * Y,
				rhs.Z * Z,
				rhs.W * W);
		}

		inline Vector4<T> operator / ( const T fScalar ) const
		{
			T fInv = (T)1.0f / fScalar;

			return Vector4<T>(
				X * fInv,
				Y * fInv,
				Z * fInv,
				W * fInv);
		}

		inline Vector4<T> operator / ( const Vector4<T>& rhs) const
		{
			return Vector4<T>(
				X / rhs.X,
				Y / rhs.Y,
				Z / rhs.Z,
				W / rhs.W);
		}

		inline const Vector4<T>& operator + () const
		{
			return *this;
		}

		inline Vector4<T> operator - () const
		{
			return Vector4<T>(-X, -Y, -Z, -W);
		}

		// arithmetic updates
		inline Vector4<T>& operator += ( const Vector4<T>& rkVector )
		{
			X += rkVector.X;
			Y += rkVector.Y;
			Z += rkVector.Z;
			W += rkVector.W;

			return *this;
		}

		inline Vector4<T>& operator -= ( const Vector4<T>& rkVector )
		{
			X -= rkVector.X;
			Y -= rkVector.Y;
			Z -= rkVector.Z;
			W -= rkVector.W;

			return *this;
		}

		inline Vector4<T>& operator *= ( const T fScalar )
		{
			X *= fScalar;
			Y *= fScalar;
			Z *= fScalar;
			W *= fScalar;
			return *this;
		}

		inline Vector4<T>& operator += ( const T fScalar )
		{
			X += fScalar;
			Y += fScalar;
			Z += fScalar;
			W += fScalar;
			return *this;
		}

		inline Vector4<T>& operator -= ( const T fScalar )
		{
			X -= fScalar;
			Y -= fScalar;
			Z -= fScalar;
			W -= fScalar;
			return *this;
		}

		inline Vector4<T>& operator *= ( const Vector4<T>& rkVector )
		{
			X *= rkVector.X;
			Y *= rkVector.Y;
			Z *= rkVector.Z;
			W *= rkVector.W;

			return *this;
		}

		inline Vector4<T>& operator /= ( const T fScalar )
		{
			T fInv = (T)1.0f / fScalar;

			X *= fInv;
			Y *= fInv;
			Z *= fInv;
			W *= fInv;

			return *this;
		}

		inline Vector4<T>& operator /= ( const Vector4<T>& rkVector )
		{
			X /= rkVector.X;
			Y /= rkVector.Y;
			Z /= rkVector.Z;
			W /= rkVector.W;

			return *this;
		}

		// 得到数组
		inline TVOID GetArray(T* value)
		{
			if (!value)
			{
				return ;
			}
			value[0] = X;
			value[1] = Y;
			value[2] = Z;
			value[3] = W;
		}

		// 全部置0
		inline Vector4<T>& SetZero()
		{
			X = (T)0;
			Y = (T)0;
			Z = (T)0;
			W = (T)0;
			return *this;
		}

		// 单位化this
		inline Vector4<T>& normalize()
		{
			T l = X*X + Y*Y + Z*Z + W*W;
			if (l == (T)0)
				return *this;
			l = (T) reciprocal_squareroot ( (TF32)l );
			X *= l;
			Y *= l;
			Z *= l;
			W *= l;
			return *this;
		}

		// 长度
		inline T getLength() const { return (T) sqrt((TF64)(X*X + Y*Y + Z*Z + W*W)); }

		/** Calculates the dot (scalar) product of this vector with another.
		@param
		vec Vector with which to calculate the dot product (together
		with this one).
		@returns
		A T representing the dot product value.
		*/
		inline T dotProduct(const Vector4<T>& vec) const
		{
			return X * vec.X + Y * vec.Y + Z * vec.Z + W * vec.W;
		}
	};

	// define float vector4
	typedef Vector4<TF32> Vector4Float;
	// define double vector4
	typedef Vector4<TF64> Vector4Double;
	// define int vector4
	typedef Vector4<TS32> Vector4Int;
	// define uint vector4
	typedef Vector4<TU32> Vector4UInt;

}

#endif