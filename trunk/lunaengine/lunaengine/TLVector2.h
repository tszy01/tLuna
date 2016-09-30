#ifndef _TLVECTOR2_H_
#define _TLVECTOR2_H_

#include "TLFPU.h"

/**
*	使用irrlicht的vector2d方法
*/

namespace TLunaEngine{

	template<typename T>
	class Vector2
	{
	public:
		Vector2() : X(0), Y(0) {}
		Vector2(T nx, T ny) : X(nx), Y(ny) {}
		Vector2(const Vector2<T>& other) : X(other.X), Y(other.Y) {}
		~Vector2()
		{
		}

		// ----------------- 运算符重载 ----------------------------------------------------------

		// 把符号改变
		inline Vector2<T> operator-() const { return Vector2<T>(-X, -Y); }

		// 赋值
		inline Vector2<T>& operator=(const Vector2<T>& other) { X = other.X; Y = other.Y; return *this; }

		// 加上另外一个向量
		inline Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(X + other.X, Y + other.Y); }
		inline Vector2<T>& operator+=(const Vector2<T>& other) { X+=other.X; Y+=other.Y; return *this; }
		// 加上另外一个标量
		inline Vector2<T> operator+(const T v) const { return Vector2<T>(X + v, Y + v); }
		inline Vector2<T>& operator+=(const T v) { X+=v; Y+=v; return *this; }

		// 减去另外一个向量
		inline Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(X - other.X, Y - other.Y); }
		inline Vector2<T>& operator-=(const Vector2<T>& other) { X-=other.X; Y-=other.Y; return *this; }
		// 减去另外一个标量
		inline Vector2<T> operator-(const T v) const { return Vector2<T>(X - v, Y - v); }
		inline Vector2<T>& operator-=(const T v) { X-=v; Y-=v; return *this; }

		// 乘以另外一个向量
		inline Vector2<T> operator*(const Vector2<T>& other) const { return Vector2<T>(X * other.X, Y * other.Y); }
		inline Vector2<T>& operator*=(const Vector2<T>& other) { X*=other.X; Y*=other.Y; return *this; }
		// 乘以另外一个标量
		inline Vector2<T> operator*(const T v) const { return Vector2<T>(X * v, Y * v); }
		inline Vector2<T>& operator*=(const T v) { X*=v; Y*=v; return *this; }

		// 除以另外一个向量
		inline Vector2<T> operator/(const Vector2<T>& other) const { return Vector2<T>(X / other.X, Y / other.Y); }
		inline Vector2<T>& operator/=(const Vector2<T>& other) { X/=other.X; Y/=other.Y; return *this; }
		// 除以另外一个标量
		inline Vector2<T> operator/(const T v) const { return Vector2<T>(X / v, Y / v); }
		inline Vector2<T>& operator/=(const T v) { X/=v; Y/=v; return *this; }

		// 比较大小
		// 必须X，Y同时满足条件
		inline TBOOL operator<=(const Vector2<T>&other) const { return X<=other.X && Y<=other.Y; }
		inline TBOOL operator>=(const Vector2<T>&other) const { return X>=other.X && Y>=other.Y; }
		inline TBOOL operator<(const Vector2<T>&other) const { return X<other.X && Y<other.Y; }
		inline TBOOL operator>(const Vector2<T>&other) const { return X>other.X && Y>other.Y; }
		inline TBOOL operator==(const Vector2<T>& other) const { return other.X==X && other.Y==Y; }
		// 不等于，有一个不等就返回真
		inline TBOOL operator!=(const Vector2<T>& other) const { return other.X!=X || other.Y!=Y; }

		// ---------------------------------------------------------------------------------------

		// 方法

		//! Checks if this vector equals the other one.
		/** Takes floating point rounding errors into account.
		\param other Vector to compare with.
		\return True if the two vector are (almost) equal, else TFALSE. */
		inline TBOOL equals(const Vector2<T>& other) const
		{
			return TLunaEngine::equals(X, other.X) && TLunaEngine::equals(Y, other.Y);
		}

		inline TVOID set(T nx, T ny) {X=nx; Y=ny; }
		inline TVOID set(const Vector2<T>& p) { X=p.X; Y=p.Y;}

		//! Gets the length of the vector.
		/** \return The length of the vector. */
		inline T getLength() const { return (T)sqrt((TF64)(X*X + Y*Y)); }

		//! Get the squared length of this vector
		/** This is useful because it is much faster than getLength().
		\return The squared length of the vector. */
		inline T getLengthSQ() const { return X*X + Y*Y; }

		// 点乘可以求投影
		//! Get the dot product of this vector with another.
		/** \param other Other vector to take dot product with.
		\return The dot product of the two vectors. */
		inline T dotProduct(const Vector2<T>& other) const
		{
			return X*other.X + Y*other.Y;
		}

		//! Gets distance from another point.
		/** Here, the vector is interpreted as a point in 2-dimensional space.
		\param other Other vector to measure from.
		\return Distance from other point. */
		inline T getDistanceFrom(const Vector2<T>& other) const
		{
			return Vector2<T>(X - other.X, Y - other.Y).getLength();
		}

		//! Returns squared distance from another point.
		/** Here, the vector is interpreted as a point in 2-dimensional space.
		\param other Other vector to measure from.
		\return Squared distance from other point. */
		inline T getDistanceFromSQ(const Vector2<T>& other) const
		{
			return Vector2<T>(X - other.X, Y - other.Y).getLengthSQ();
		}

		//! rotates the point around a center by an amount of degrees.
		/** \param degrees Amount of degrees to rotate by.
		\param center Rotation center. */
		inline TVOID rotateBy(TF64 degrees, const Vector2<T>& center)
		{
			degrees *= DEGTORAD64;
			T cs = (T)cos(degrees);
			T sn = (T)sin(degrees);

			X -= center.X;
			Y -= center.Y;

			set(X*cs - Y*sn, X*sn + Y*cs);

			X += center.X;
			Y += center.Y;
		}

		//! Normalize the vector.
		/** The null vector is left untouched.
		\return Reference to this vector, after normalization. */
		inline Vector2<T>& normalize()
		{
			T l = X*X + Y*Y;
			if (l == (T)0.0f)
				return *this;
			l = (T)TLunaEngine::reciprocal_squareroot ( (TF32)l );
			X *= l;
			Y *= l;
			return *this;
		}

		//! Calculates the angle of this vector in degrees in the trigonometric sense.
		/** 0 is to the left (9 o'clock), values increase clockwise.
		This method has been suggested by Pr3t3nd3r.
		\return Returns a value between 0 and 360. */
		inline TF64 getAngleTrig() const
		{
			if (X == 0)
				return Y < 0 ? 270 : 90;
			else
			if (Y == 0)
				return X < 0 ? 180 : 0;

			if ( Y > 0)
				if (X > 0)
					return atan(Y/X) * RADTODEG64;
				else
					return 180.0-atan(Y/-X) * RADTODEG64;
			else
				if (X > 0)
					return 360.0-atan(-Y/X) * RADTODEG64;
				else
					return 180.0+atan(-Y/-X) * RADTODEG64;
		}

		//! Calculates the angle of this vector in degrees in the counter trigonometric sense.
		/** 0 is to the right (3 o'clock), values increase counter-clockwise.
		\return Returns a value between 0 and 360. */
		inline TF64 getAngle() const
		{
			if (Y == 0) // corrected thanks to a suggestion by Jox
				return X < 0 ? 180 : 0;
			else if (X == 0)
				return Y < 0 ? 90 : 270;

			TF64 tmp = (TF64)Y / (TF64)getLength();
			tmp = atan(sqrt(1 - tmp*tmp) / tmp) * RADTODEG64;

			if (X>0 && Y>0)
				return tmp + 270;
			else
			if (X>0 && Y<0)
				return tmp + 90;
			else
			if (X<0 && Y<0)
				return 90 - tmp;
			else
			if (X<0 && Y>0)
				return 270 - tmp;

			return tmp;
		}

		//! Calculates the angle between this vector and another one in degree.
		/** \param b Other vector to test with.
		\return Returns a value between 0 and 90. */
		inline TF64 getAngleWith(const Vector2<T>& b) const
		{
			TF64 tmp = (TF64)(X*b.X + Y*b.Y);

			if (tmp == 0.0)
				return 90.0;

			tmp = tmp / sqrt((TF64)((X*X + Y*Y) * (b.X*b.X + b.Y*b.Y)));
			if (tmp < 0.0)
				tmp = -tmp;

			return atan(sqrt(1 - tmp*tmp) / tmp) * RADTODEG64;
		}

		//! Returns if this vector interpreted as a point is on a line between two other points.
		/** It is assumed that the point is on the line.
		\param begin Beginning vector to compare between.
		\param end Ending vector to compare between.
		\return True if this vector is between begin and end, TFALSE if not. */
		inline TBOOL isBetweenPoints(const Vector2<T>& begin, const Vector2<T>& end) const
		{
			T f = (end - begin).getLengthSQ();
			return getDistanceFromSQ(begin) < f &&
				getDistanceFromSQ(end) < f;
		}

		//! Get the interpolated vector
		/** \param other Other vector to interpolate with.
		\param d Value between 0.0f and 1.0f.
		\return Interpolated vector. */
		inline Vector2<T> getInterpolated(const Vector2<T>& other, TF32 d) const
		{
			TF32 inv = 1.0f - d;
			return Vector2<T>(other.X*inv + X*d, other.Y*inv + Y*d);
		}

		//! Returns (quadratically) interpolated vector between this and the two given ones.
		/** \param v2 Second vector to interpolate with
		\param v3 Third vector to interpolate with
		\param d Value between 0.0f and 1.0f.
		\return Interpolated vector. */
		inline Vector2<T> getInterpolated_quadratic(const Vector2<T>& v2, const Vector2<T>& v3, const TF32 d) const
		{
			// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
			const TF32 inv = 1.0f - d;
			const TF32 mul0 = inv * inv;
			const TF32 mul1 = 2.0f * d * inv;
			const TF32 mul2 = d * d;

			return Vector2<T> ( X * mul0 + v2.X * mul1 + v3.X * mul2,
						Y * mul0 + v2.Y * mul1 + v3.Y * mul2);
		}

		// 从b出发向a走t步
		//! Sets this vector to the linearly interpolated vector between a and b.
		/** \param a first vector to interpolate with
		\param b second vector to interpolate with
		\param t value between 0.0f and 1.0f. */
		inline TVOID interpolate(const Vector2<T>& a, const Vector2<T>& b, const TF32 t)
		{
			X = b.X + ( ( a.X - b.X ) * t );
			Y = b.Y + ( ( a.Y - b.Y ) * t );
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
		}

		//! X coordinate of vector.
		T X;
		//! Y coordinate of vector.
		T Y;
	};

	// define float vector2
	typedef Vector2<TF32> Vector2Float;
	// define double vector2
	typedef Vector2<TF64> Vector2Double;
	// define int vector2
	typedef Vector2<TS32> Vector2Int;
	// define uint vector2
	typedef Vector2<TU32> Vector2UInt;

}

#endif