#ifndef _TLVECTOR3_H_
#define _TLVECTOR3_H_

#include "TLFPU.h"

/**
*	使用irrlicht的vector3d方法
*/

namespace TLunaEngine{
	//! 3d vector template class with lots of operators and methods.
	template <typename T>
	class Vector3
	{
	public:
		// 构造函数
		Vector3() : X(0), Y(0), Z(0) {}
		Vector3(T nx, T ny, T nz) : X(nx), Y(ny), Z(nz) {}
		Vector3(const Vector3<T>& other) : X(other.X), Y(other.Y), Z(other.Z) {}
		~Vector3()
		{
		}

		// ------------------------ 重载运算法 ----------------------------------

		// 注意:预算符乘以,除以另外一个向量既不是点乘,也不是叉乘,使用时要注意
		inline Vector3<T> operator-() const { return Vector3<T>(-X, -Y, -Z); }
		inline Vector3<T>& operator=(const Vector3<T>& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }
		inline Vector3<T> operator+(const Vector3<T>& other) const { return Vector3<T>(X + other.X, Y + other.Y, Z + other.Z); }
		inline Vector3<T>& operator+=(const Vector3<T>& other) { X+=other.X; Y+=other.Y; Z+=other.Z; return *this; }
		inline Vector3<T> operator-(const Vector3<T>& other) const { return Vector3<T>(X - other.X, Y - other.Y, Z - other.Z); }
		inline Vector3<T>& operator-=(const Vector3<T>& other) { X-=other.X; Y-=other.Y; Z-=other.Z; return *this; }
		inline Vector3<T> operator*(const Vector3<T>& other) const { return Vector3<T>(X * other.X, Y * other.Y, Z * other.Z); }
		inline Vector3<T>& operator*=(const Vector3<T>& other) { X*=other.X; Y*=other.Y; Z*=other.Z; return *this; }
		inline Vector3<T> operator*(const T v) const { return Vector3<T>(X * v, Y * v, Z * v); }
		inline Vector3<T>& operator*=(const T v) { X*=v; Y*=v; Z*=v; return *this; }
		inline Vector3<T> operator/(const Vector3<T>& other) const { return Vector3<T>(X / other.X, Y / other.Y, Z / other.Z); }
		inline Vector3<T>& operator/=(const Vector3<T>& other) { X/=other.X; Y/=other.Y; Z/=other.Z; return *this; }
		inline Vector3<T> operator/(const T v) const { T i=(T)1.0/v; return Vector3<T>(X * i, Y * i, Z * i); }
		inline Vector3<T>& operator/=(const T v) { T i=(T)1.0/v; X*=i; Y*=i; Z*=i; return *this; }
		// 大小比较,必须三个同时满足
		inline TBOOL operator<=(const Vector3<T>&other) const { return X<=other.X && Y<=other.Y && Z<=other.Z;}
		inline TBOOL operator>=(const Vector3<T>&other) const { return X>=other.X && Y>=other.Y && Z>=other.Z;}
		inline TBOOL operator<(const Vector3<T>&other) const { return X<other.X && Y<other.Y && Z<other.Z;}
		inline TBOOL operator>(const Vector3<T>&other) const { return X>other.X && Y>other.Y && Z>other.Z;}
		// 比较相等
		inline TBOOL operator==(const Vector3<T>& other) const
		{
			return this->equals(other);
		}
		// 比较不等
		inline TBOOL operator!=(const Vector3<T>& other) const
		{
			return !this->equals(other);
		}
		// ----------------------------------------------------------------------
		// ------------------------ 成员函数 ------------------------------------

		// 调用FPU的比较函数
		inline TBOOL equals(const Vector3<T>& other, const T tolerance = (T)ROUNDING_ERROR_32 ) const
		{
			return TLunaEngin::equals(X, other.X, tolerance) &&
				TLunaEngin::equals(Y, other.Y, tolerance) &&
				TLunaEngin::equals(Z, other.Z, tolerance);
		}

		/** Returns whether this vector is within a directional tolerance
			of another vector.
		@param rhs The vector to compare with
		@param tolerance The maximum angle by which the vectors may vary and
			still be considered equal
		@note Both vectors should be normalised.
		*/
		inline TBOOL directionEquals(const Vector3<T>& rhs,
			const T& tolerance) const
		{
			T dot = dotProduct(rhs);
			T angle = (T)acos(dot);

			return abs_(angle) <= tolerance;

		}
		// 赋值函数
		inline TVOID set(const T nx, const T ny, const T nz) {X=nx; Y=ny; Z=nz; }
		inline TVOID set(const Vector3<T>& p) { X=p.X; Y=p.Y; Z=p.Z;}

		// 长度
		inline T getLength() const { return (T) sqrt((TF64)(X*X + Y*Y + Z*Z)); }

		// 长度的平方
		inline T getLengthSQ() const { return X*X + Y*Y + Z*Z; }

		// X,Z平面的长度
		inline T getLength2D() const { return (T) sqrt((TF64)(X*X + Z*Z)); }

		// 点乘结果
		inline T dotProduct(const Vector3<T>& other) const
		{
			return X*other.X + Y*other.Y + Z*other.Z;
		}

		inline T absDotProduct(const Vector3<T>& vec) const
        {
            return TLunaEngine::abs_(X * vec.X) + TLunaEngine::abs_(Y * vec.Y) + TLunaEngine::abs_(Z * vec.Z);
        }

		// this-other的长度
		inline T getDistanceFrom(const Vector3<T>& other) const
		{
			return Vector3<T>(X - other.X, Y - other.Y, Z - other.Z).getLength();
		}

		// this-other长度平方
		inline T getDistanceFromSQ(const Vector3<T>& other) const
		{
			return Vector3<T>(X - other.X, Y - other.Y, Z - other.Z).getLengthSQ();
		}

		// 叉乘结果
		inline Vector3<T> crossProduct(const Vector3<T>& p) const
		{
			return Vector3<T>(Y * p.Z - Z * p.Y, Z * p.X - X * p.Z, X * p.Y - Y * p.X);
		}

		// this是否在通过begin和end的中间的线上
		inline TBOOL isBetweenPoints(const Vector3<T>& begin, const Vector3<T>& end) const
		{
			T f = (end - begin).getLengthSQ();
			return getDistanceFromSQ(begin) < f &&
				getDistanceFromSQ(end) < f;
		}

		/** Gets the angle between 2 vectors.
		@remarks
		Vectors do not have to be unit-length but must represent directions.
		*/
		inline T angleBetween(const Vector3<T>& dest) const
		{
			T lenProduct = getLength() * dest.getLength();

			// Divide by zero check
			if (lenProduct < (T)1e-6f)
				lenProduct = (T)1e-6f;

			T f = dotProduct(dest) / lenProduct;

			f = clamp(f, -1.0, 1.0);
			return acos_(f);

		}

		// 单位化this
		inline Vector3<T>& normalize()
		{
			T l = X*X + Y*Y + Z*Z;
			if (l == (T)0)
				return *this;
			l = (T) reciprocal_squareroot ( (TF32)l );
			X *= l;
			Y *= l;
			Z *= l;
			return *this;
		}

		// this乘以一个长度
		// 这个函数会先将this单位化
		inline TVOID setLength(T newlength)
		{
			normalize();
			*this *= newlength;
		}

		// this的每个分量乘以-1
		inline TVOID invert()
		{
			X *= -1.0f;
			Y *= -1.0f;
			Z *= -1.0f;
		}

		// this在center的坐标系绕Y轴旋转degree角度
		// 如果center的X,Z都是0,那就是this绕本坐标系Y旋转
		inline TVOID rotateXZBy(TF32 degrees, const Vector3<T>& center)
		{
			degrees *= DEGTORAD;
			T cs = (T)cosf(degrees);
			T sn = (T)sinf(degrees);
			X -= center.X;
			Z -= center.Z;
			set(X*cs - Z*sn, Y, X*sn + Z*cs);
			X += center.X;
			Z += center.Z;
		}

		// this在center的坐标系绕Z轴旋转degree角度
		// 如果center的X,Y都是0,那就是this绕本坐标系Z旋转
		inline TVOID rotateXYBy(TF32 degrees, const Vector3<T>& center)
		{
			degrees *= DEGTORAD;
			T cs = (T)cosf(degrees);
			T sn = (T)sinf(degrees);
			X -= center.X;
			Y -= center.Y;
			set(X*cs - Y*sn, X*sn + Y*cs, Z);
			X += center.X;
			Y += center.Y;
		}

		// this在center的坐标系绕X轴旋转degree角度
		// 如果center的Z,Y都是0,那就是this绕本坐标系X旋转
		inline TVOID rotateYZBy(TF32 degrees, const Vector3<T>& center)
		{
			degrees *= DEGTORAD;
			T cs = (T)cosf(degrees);
			T sn = (T)sinf(degrees);
			Z -= center.Z;
			Y -= center.Y;
			set(X, Y*cs - Z*sn, Y*sn + Z*cs);
			Z += center.Z;
			Y += center.Y;
		}

		// this->X * d + other.X * (1-d)
		// Y,Z类似X的算法
		// 权重加和
		// 0 < d < 1
		inline Vector3<T> getInterpolated(const Vector3<T>& other, const TF32 d) const
		{
			const TF32 inv = 1.0f - d;
			return Vector3<T>(other.X*inv + X*d, other.Y*inv + Y*d, other.Z*inv + Z*d);
		}

		// 二次的权重加和
		// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
		// 0 < d < 1
		inline Vector3<T> getInterpolated_quadratic(const Vector3<T>& v2, const Vector3<T>& v3, const TF32 d) const
		{
			const TF32 inv = 1.0f - d;
			const TF32 mul0 = inv * inv;
			const TF32 mul1 = 2.0f * d * inv;
			const TF32 mul2 = d * d;

			return Vector3<T> ( X * mul0 + v2.X * mul1 + v3.X * mul2,
					Y * mul0 + v2.Y * mul1 + v3.Y * mul2,
					Z * mul0 + v2.Z * mul1 + v3.Z * mul2);
		}

		// 得到X,Z平面的角度
		// 范围0-360
		inline TF32 getHorizonAngle()
		{
			TF32 angle = (TF32)atan2(Z,X);
			angle *= (TF32)RADTODEG64;
			if (angle < 0.0f) angle += 360.0f;
			if (angle >= 360.0f) angle -= 360.0f;
			return angle;
		}

		// 得到水平角度另一种算法，返回弧度
		inline T getYaw()
		{
			if ((TF32)Z>-0.001f && (TF32)Z<0.001f)
			{
				if (X >= (T)0)
				{
					return (T)(PI * 0.5f);
				}
				else
				{
					return (T)(-PI * 0.5f);
				}
			}
			else
			{
				if (Z >= (T)0)
				{
					return atan_(X / Z);
				}
				else
				{
					return atan_(X / Z) + PI;
				}
			}
			return (T)0;
		}

		// 全部置0
		inline Vector3<T>& SetZero()
		{
			X = (T)0;
			Y = (T)0;
			Z = (T)0;
			return *this;
		}

		//! Tests for exact zero vector
		inline TBOOL IsZero()
		{
			if(IR(X) || IR(Y) || IR(Z))	return TFALSE;
			return TTRUE;
		}

		/** Returns TTRUE if this vector is zero length. */
		inline TBOOL isZeroLength(TVOID) const
		{
			TF32 sqlen = (X * X) + (Y * Y) + (Z * Z);
			return (sqlen < (1e-06 * 1e-06));

		}

		// 线性插值
		// this = a + t * (b - a)
		inline Vector3<T>& Lerp(const Vector3<T>& a, const Vector3<T>& b, T t)
		{
			X = a.X + t * (b.X - a.X);
			Y = a.Y + t * (b.Y - a.Y);
			Z = a.Z + t * (b.Z - a.Z);
			return *this;
		}

		// 得到数组w=1
		inline TVOID AsPoint(T* value)
		{
			if(!value)
				return;
			value[0] = X;
			value[1] = Y;
			value[2] = Z;
			value[3] = 1;
		}

		// 得到数组w=0
		inline TVOID AsDirection(T* value)
		{
			if(!value)
				return;
			value[0] = X;
			value[1] = Y;
			value[2] = Z;
			value[3] = 0;
		}

		/** Sets this vector's components to the minimum of its own and the
            ones of the passed in vector.
            @remarks
                'Minimum' in this case means the combination of the lowest
                value of x, y and z from both vectors. Lowest is taken just
                numerically, not magnitude, so -1 < 0.
        */
        inline TVOID makeFloor( const Vector3<T>& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
            if( cmp.z < z ) z = cmp.z;
        }

        /** Sets this vector's components to the maximum of its own and the
            ones of the passed in vector.
            @remarks
                'Maximum' in this case means the combination of the highest
                value of x, y and z from both vectors. Highest is taken just
                numerically, not magnitude, so 1 > -3.
        */
        inline TVOID makeCeil( const Vector3<T>& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
            if( cmp.z > z ) z = cmp.z;
        }

        /** Generates a vector perpendicular to this vector (eg an 'up' vector).
            @remarks
                This method will return a vector which is perpendicular to this
                vector. There are an infinite number of possibilities but this
                method will guarantee to generate one of them. If you need more
                control you should use the Quaternion class.
        */
        inline Vector3<T> perpendicular(TVOID) const
        {
            static const TF32 fSquareZero = (TF32)(1e-06 * 1e-06);

            Vector3<T> perp = this->crossProduct( Vector3<T>(1,0,0) );

            // Check length
            if( perp.getLengthSQ() < fSquareZero )
            {
                /* This vector is the Y axis multiplied by a scalar, so we have
                   to use another axis.
                */
                perp = this->crossProduct( Vector3<T>(0,1,0) );
            }
			perp.normalize();

            return perp;
        }

		inline T operator [] (const TSIZE i) const
		{
			return *(&X + i);
		}

		inline T& operator [] (const TSIZE i)
		{
			return *(&X + i);
		}


		//! X coordinate of the vector
		T X;
		//! Y coordinate of the vector
		T Y;
		//! Z coordinate of the vector
		T Z;
	};

	// define float vector3
	typedef Vector3<TF32> Vector3Float;
	// define double vector3
	typedef Vector3<TF64> Vector3Double;
	// define int vector3
	typedef Vector3<TS32> Vector3Int;
	// define uint vector3
	typedef Vector3<TU32> Vector3UInt;

}

#endif