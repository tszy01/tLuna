#ifndef _TLQUATERNION_H_
#define _TLQUATERNION_H_
//#include "Matrix4x4.h"
#include "TLFPU.h"

namespace TLunaEngine{
	template<typename T>
	class Quaternion
	{
	public:
		//! Default Constructor
		Quaternion() : X(0), Y(0), Z(0), W(1) {}

		// Destructor
		~Quaternion(){}

		//! Constructor
		Quaternion(T x, T y, T z, T w) : X(x), Y(y), Z(z), W(w) { }

		//! Constructor which converts euler angles (radians) to a Quaternion
		Quaternion(T x, T y, T z){set(x,y,z);}

		//! Constructor which converts euler angles (radians) to a Quaternion
		Quaternion(const Vector3<T>& vec){set(vec.X,vec.Y,vec.Z);}

		////! Constructor which converts a matrix to a Quaternion
		//Quaternion(const Matrix4x4<T>& m)
		//{
		//	T diag = m[0][0] + m[1][1] + m[2][2] + 1;
		//	TF32 scale = 0.0f;

		//	if( diag > (T)0.0f )
		//	{
		//		scale = sqrtf(diag) * 2.0f; // get scale from diagonal

		//		// TODO: speed this up
		//		X = (T)((TF32)( m[2][1] - m[1][2]) / scale);
		//		Y = (T)((TF32)( m[0][2] - m[2][0]) / scale);
		//		Z = (T)((TF32)( m[1][0] - m[0][1]) / scale);
		//		W = (T)(0.25f * scale);
		//	}
		//	else
		//	{
		//		if ( m[0][0] > m[1][1] && m[0][0] > m[2][2])
		//		{
		//			// 1st element of diag is greatest value
		//			// find scale according to 1st element, and double it
		//			scale = sqrtf( 1.0f + (TF32)m[0][0] - (TF32)m[1][1] - (TF32)m[2][2]) * 2.0f;

		//			// TODO: speed this up
		//			X = (T)(0.25f * scale);
		//			Y = (T)((TF32)(m[0][1] + m[1][0]) / scale);
		//			Z = (T)((TF32)(m[2][0] + m[0][2]) / scale);
		//			W = (T)((TF32)(m[2][1] - m[1][2]) / scale);
		//		}
		//		else if ( m[1][1] > m[2][2])
		//		{
		//			// 2nd element of diag is greatest value
		//			// find scale according to 2nd element, and double it
		//			scale = sqrtf( 1.0f + (TF32)m[1][1] - (TF32)m[0][0] - (TF32)m[2][2]) * 2.0f;

		//			// TODO: speed this up
		//			X = (T)((TF32)(m[0][1] + m[1][0] ) / scale);
		//			Y = (T)(0.25f * scale);
		//			Z = (T)((TF32)(m[1][2] + m[2][1] ) / scale);
		//			W = (T)((TF32)(m[0][2] - m[2][0] ) / scale);
		//		}
		//		else
		//		{
		//			// 3rd element of diag is greatest value
		//			// find scale according to 3rd element, and double it
		//			scale = sqrtf( 1.0f + (TF32)m[2][2] - (TF32)m[0][0] - (TF32)m[1][1]) * 2.0f;

		//			// TODO: speed this up
		//			X = (T)((TF32)(m[0][2] + m[2][0]) / scale);
		//			Y = (T)((TF32)(m[1][2] + m[2][1]) / scale);
		//			Z = (T)(0.25f * scale);
		//			W = (T)((TF32)(m[1][0] - m[0][1]) / scale);
		//		}
		//	}

		//	normalize();
		//}

		//! Equalilty operator
		inline TBOOL operator==(const Quaternion<T>& other) const
		{
			if(X != other.X)
				return TFALSE;
			if(Y != other.Y)
				return TFALSE;
			if(Z != other.Z)
				return TFALSE;
			if(W != other.W)
				return TFALSE;

			return TTRUE;
		}

		//! Assignment operator
		inline Quaternion<T>& operator=(const Quaternion<T>& other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
			return *this;
		}

		////! Matrix assignment operator
		//inline Quaternion<T>& operator=(const Matrix4x4<T>& m)
		//{
		//	T diag = m[0][0] + m[1][1] + m[2][2] + 1;
		//	TF32 scale = 0.0f;

		//	if( diag > (T)0.0f )
		//	{
		//		scale = sqrtf(diag) * 2.0f; // get scale from diagonal

		//		// TODO: speed this up
		//		X = (T)((TF32)( m[2][1] - m[1][2]) / scale);
		//		Y = (T)((TF32)( m[0][2] - m[2][0]) / scale);
		//		Z = (T)((TF32)( m[1][0] - m[0][1]) / scale);
		//		W = (T)(0.25f * scale);
		//	}
		//	else
		//	{
		//		if ( m[0][0] > m[1][1] && m[0][0] > m[2][2])
		//		{
		//			// 1st element of diag is greatest value
		//			// find scale according to 1st element, and double it
		//			scale = sqrtf( 1.0f + (TF32)m[0][0] - (TF32)m[1][1] - (TF32)m[2][2]) * 2.0f;

		//			// TODO: speed this up
		//			X = (T)(0.25f * scale);
		//			Y = (T)((TF32)(m[0][1] + m[1][0]) / scale);
		//			Z = (T)((TF32)(m[2][0] + m[0][2]) / scale);
		//			W = (T)((TF32)(m[2][1] - m[1][2]) / scale);
		//		}
		//		else if ( m[1][1] > m[2][2])
		//		{
		//			// 2nd element of diag is greatest value
		//			// find scale according to 2nd element, and double it
		//			scale = sqrtf( 1.0f + (TF32)m[1][1] - (TF32)m[0][0] - (TF32)m[2][2]) * 2.0f;

		//			// TODO: speed this up
		//			X = (T)((TF32)(m[0][1] + m[1][0] ) / scale);
		//			Y = (T)(0.25f * scale);
		//			Z = (T)((TF32)(m[1][2] + m[2][1] ) / scale);
		//			W = (T)((TF32)(m[0][2] - m[2][0] ) / scale);
		//		}
		//		else
		//		{
		//			// 3rd element of diag is greatest value
		//			// find scale according to 3rd element, and double it
		//			scale = sqrtf( 1.0f + (TF32)m[2][2] - (TF32)m[0][0] - (TF32)m[1][1]) * 2.0f;

		//			// TODO: speed this up
		//			X = (T)((TF32)(m[0][2] + m[2][0]) / scale);
		//			Y = (T)((TF32)(m[1][2] + m[2][1]) / scale);
		//			Z = (T)(0.25f * scale);
		//			W = (T)((TF32)(m[1][0] - m[0][1]) / scale);
		//		}
		//	}

		//	normalize();
		//	return *this;
		//}

		//! Add operator
		inline Quaternion<T> operator+(const Quaternion<T>& other) const
		{
			return Quaternion<T>(X+other.X, Y+other.Y, Z+other.Z, W+other.W);
		}

		//! Multiplication operator
		inline Quaternion<T> operator*(const Quaternion<T>& other) const
		{
			Quaternion<T> tmp;
			tmp.W = (other.W * W) - (other.X * X) - (other.Y * Y) - (other.Z * Z);
			tmp.X = (other.W * X) + (other.X * W) + (other.Y * Z) - (other.Z * Y);
			tmp.Y = (other.W * Y) + (other.Y * W) + (other.Z * X) - (other.X * Z);
			tmp.Z = (other.W * Z) + (other.Z * W) + (other.X * Y) - (other.Y * X);

			return tmp;
		}

		//! Multiplication operator
		inline Quaternion<T> operator*(T s) const
		{
			return Quaternion<T>(s*X, s*Y, s*Z, s*W);
		}

		//! Multiplication operator
		inline Quaternion<T>& operator*=(T s)
		{
			X *= s; Y*=s; Z*=s; W*=s;
			return *this;
		}

		//! Multiplication operator
		inline Vector3<T> operator*(const Vector3<T>& v) const
		{
			// nVidia SDK implementation
			Vector3<T> uv, uuv;
			Vector3<T> qvec(X, Y, Z);
			uv = qvec.crossProduct(v);
			uuv = qvec.crossProduct(uv);
			uv *= ((T)2.0f * W);
			uuv *= (T)2.0f;

			return v + uv + uuv;
		}

		//! Multiplication operator
		inline Quaternion<T>& operator*=(const Quaternion<T>& other)
		{
			*this = other * (*this);
			return *this;
		}

		//! Calculates the dot product
		inline T getDotProduct(const Quaternion<T>& other) const
		{
			return (X * other.X) + (Y * other.Y) + (Z * other.Z) + (W * other.W);
		}

		//! Sets new Quaternion
		inline TVOID set(T x, T y, T z, T w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		//! Sets new Quaternion based on euler angles (radians)
		inline TVOID set(T x, T y, T z)
		{
			TF64 angle;
			angle = (TF64)x * 0.5;
			TF64 sr = sin(angle);
			TF64 cr = cos(angle);

			angle = (TF64)y * 0.5;
			TF64 sp = sin(angle);
			TF64 cp = cos(angle);

			angle = (TF64)z * 0.5;
			TF64 sy = sin(angle);
			TF64 cy = cos(angle);

			TF64 cpcy = cp * cy;
			TF64 spcy = sp * cy;
			TF64 cpsy = cp * sy;
			TF64 spsy = sp * sy;

			X = (T)(sr * cpcy - cr * spsy);
			Y = (T)(cr * spcy + sr * cpsy);
			Z = (T)(cr * cpsy - sr * spcy);
			W = (T)(cr * cpcy + sr * spsy);

			normalize();
		}

		//! Sets new Quaternion based on euler angles (radians)
		inline TVOID set(const Vector3<T>& vec)
		{
			set(vec.X, vec.Y, vec.Z);
		}

		//! Normalizes the Quaternion
		inline Quaternion<T>& normalize()
		{
			T n = X*X + Y*Y + Z*Z + W*W;
			if (n == (T)1)
				return *this;

			//n = 1.0f / sqrtf(n);
			n = (T)TLunaEngine::reciprocal_squareroot ( (TF32)n );
			X *= n;
			Y *= n;
			Z *= n;
			W *= n;

			return *this;
		}

		////! Creates a matrix from this Quaternion
		//inline Matrix4x4<T> getMatrix() const
		//{
		//	Matrix4x4<T> m;

		//	m[0][0] = (T)(1.0f - 2.0f*Y*Y - 2.0f*Z*Z);
		//	m[1][0] = (T)(2.0f*X*Y + 2.0f*Z*W);
		//	m[2][0] = (T)(2.0f*X*Z - 2.0f*Y*W);
		//	m[3][0] = (T)(0.0f);

		//	m[0][1] = (T)(2.0f*X*Y - 2.0f*Z*W);
		//	m[1][1] = (T)(1.0f - 2.0f*X*X - 2.0f*Z*Z);
		//	m[2][1] = (T)(2.0f*Z*Y + 2.0f*X*W);
		//	m[3][1] = (T)(0.0f);

		//	m[0][2] = (T)(2.0f*X*Z + 2.0f*Y*W);
		//	m[1][2] = (T)(2.0f*Z*Y - 2.0f*X*W);
		//	m[2][2] = (T)(1.0f - 2.0f*X*X - 2.0f*Y*Y);
		//	m[3][2] = (T)(0.0f);

		//	m[0][3] = (T)0.0f;
		//	m[1][3] = (T)0.0f;
		//	m[2][3] = (T)0.0f;
		//	m[3][3] = (T)1.0f;

		//	return m;
		//}

		////! Creates a matrix from this Quaternion
		//inline TVOID getMatrix( Matrix4x4<T> &dest ) const
		//{
		//	dest[0][0] = (T)(1.0f - 2.0f*Y*Y - 2.0f*Z*Z);
		//	dest[0][1] = (T)(2.0f*X*Y + 2.0f*Z*W);
		//	dest[0][2] = (T)(2.0f*X*Z - 2.0f*Y*W);
		//	dest[0][3] = (T)(0.0f);

		//	dest[1][0] = (T)(2.0f*X*Y - 2.0f*Z*W);
		//	dest[1][1] = (T)(1.0f - 2.0f*X*X - 2.0f*Z*Z);
		//	dest[1][2] = (T)(2.0f*Z*Y + 2.0f*X*W);
		//	dest[1][3] = (T)(0.0f);

		//	dest[2][0] = (T)(2.0f*X*Z + 2.0f*Y*W);
		//	dest[2][1] = (T)(2.0f*Z*Y - 2.0f*X*W);
		//	dest[2][2] = (T)(1.0f - 2.0f*X*X - 2.0f*Y*Y);
		//	dest[2][3] = (T)(0.0f);

		//	dest[3][0] = (T)0.0f;
		//	dest[3][1] = (T)0.0f;
		//	dest[3][2] = (T)0.0f;
		//	dest[3][3] = (T)1.0f;
		//}

		////! Creates a matrix from this Quaternion
		//inline TVOID getMatrix_transposed( Matrix4x4<T> &dest ) const
		//{
		//	dest[0][0] = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
		//	dest[1][0] = 2.0f*X*Y + 2.0f*Z*W;
		//	dest[2][0] = 2.0f*X*Z - 2.0f*Y*W;
		//	dest[3][0] = 0.0f;

		//	dest[0][1] = 2.0f*X*Y - 2.0f*Z*W;
		//	dest[1][1] = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
		//	dest[2][1] = 2.0f*Z*Y + 2.0f*X*W;
		//	dest[3][1] = 0.0f;

		//	dest[0][2] = 2.0f*X*Z + 2.0f*Y*W;
		//	dest[1][2] = 2.0f*Z*Y - 2.0f*X*W;
		//	dest[2][2] = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
		//	dest[3][2] = 0.0f;

		//	dest[0][3] = 0.0f;
		//	dest[1][3] = 0.0f;
		//	dest[2][3] = 0.0f;
		//	dest[3][3] = 1.0f;
		//}

		//! Inverts this Quaternion
		inline Quaternion<T>& makeInverse()
		{
			X = -X; Y = -Y; Z = -Z;
			return *this;
		}

		//! Set this Quaternion to the result of the interpolation between two quaternions
		inline TVOID slerp(const Quaternion<T>& q1,const Quaternion<T>& q2, TF32 interpolate )
		{
			T angle = q1.getDotProduct(q2);

			if (angle < (T)0.0f)
			{
				q1 *= (T)(-1.0f);
				angle *= (T)(-1.0f);
			}

			TF32 scale;
			TF32 invscale;

			if (((TF32)angle + 1.0f) > 0.05f)
			{
				if ((1.0f - (TF32)angle) >= 0.05f) // spherical interpolation
				{
					TF32 theta = acosf((TF32)angle);
					TF32 invsintheta = 1.0f / sinf(theta);
					scale = sinf(theta * (1.0f-interpolate)) * invsintheta;
					invscale = sin(theta * interpolate) * invsintheta;
				}
				else // linear interploation
				{
					scale = 1.0f - interpolate;
					invscale = interpolate;
				}
			}
			else
			{
				q2.set(-q1.Y, q1.X, -q1.W, q1.Z);
				scale = sinf(PI * (0.5f - interpolate));
				invscale = sinf(PI * interpolate);
			}

			*this = (q1*scale) + (q2*invscale);
		}

		/// Spherical quadratic interpolation
        inline TVOID squad (TF32 fT, const Quaternion<T>& rkP,
            const Quaternion<T>& rkA, const Quaternion<T>& rkB,
            const Quaternion<T>& rkQ/*, TBOOL shortestPath = TFALSE*/)
		{
			TF32 fSlerpT = 2.0f*fT*(1.0f-fT);
			Quaternion<T> kSlerpP;
			kSlerpP.slerp(rkP,rkQ,fT/*,shortestPath*/);
			Quaternion<T> kSlerpQ;
			kSlerpQ.slerp(rkA,rkb,fT);
			slerp(kSlerpP,kSlerpQ,fSlerpT);
		}

		//! Create Quaternion from rotation angle and rotation axis.
		/** Axis must be unit length.
		The Quaternion representing the rotation is
		q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
		inline TVOID fromAngleAxis (T angle, const Vector3<T>& axis)
		{
			TF32 fHalfAngle = 0.5f*(TF32)angle;
			TF32 fSin = sinf(fHalfAngle);
			W = (T)cosf(fHalfAngle);
			X = (T)(fSin*(TF32)axis.X);
			Y = (T)(fSin*(TF32)axis.Y);
			Z = (T)(fSin*(TF32)axis.Z);
		}

		//! Fills an angle (radians) around an axis (unit vector)
		inline TVOID toAngleAxis (T &angle, Vector3<T>& axis) const
		{
			TF32 scale = sqrtf(X*X + Y*Y + Z*Z);
			if (TLunaEngine::iszero(scale) || W > (T)1.0f || W < (T)-1.0f)
			{
				angle = (T)0.0f;
				axis.X = (T)0.0f;
				axis.Y = (T)1.0f;
				axis.Z = (T)0.0f;
			}
			else
			{
				angle = (T)(2.0f * acosf((TF32)W));
				axis.X = (T)((TF32)X / scale);
				axis.Y = (T)((TF32)Y / scale);
				axis.Z = (T)((TF32)Z / scale);
			}
		}

		//! Output this Quaternion to an euler angle (radians)
		inline TVOID toEuler(Vector3<T>& euler) const
		{
			T sqw = W*W;
			T sqx = X*X;
			T sqy = Y*Y;
			T sqz = Z*Z;

			// heading = rotation about z-axis
			euler.Z = (T) (atan2(2.0 * (TF64)(X*Y +Z*W),(TF64)(sqx - sqy - sqz + sqw)));

			// bank = rotation about x-axis
			euler.X = (T) (atan2(2.0 * (TF64)(Y*Z +X*W),(TF64)(-sqx - sqy + sqz + sqw)));

			// attitude = rotation about y-axis
			euler.Y = (T) (asin( (TF64)clamp(-2.0 * (TF64)(X*Z - Y*W), -1.0, 1.0) ));
		}

		//! Set Quaternion to identity
		inline TVOID makeIdentity()
		{
			W = (T)1.f;
			X = (T)0.f;
			Y = (T)0.f;
			Z = (T)0.f;
		}

		//! Set Quaternion to represent a rotation from one vector to another.
		inline TVOID rotationFromTo(const Vector3<T>& from, const Vector3<T>& to)
		{
			// Based on Stan Melax's article in Game Programming Gems
			// Copy, since cannot modify local
			Vector3<T> v0 = from;
			Vector3<T> v1 = to;
			v0.normalize();
			v1.normalize();

			Vector3<T> c = v0.crossProduct(v1);

			T d = v0.dotProduct(v1);
			if (d >= (T)1.0f) // If dot == 1, vectors are the same
			{
				*this=Quaternion<T>(0,0,0,1); //IDENTITY;
			}
			TF32 s = sqrtf( (TF32)((1+d)*2) ); // optimize inv_sqrt
			TF32 invs = 1.0f / s;

			X = (T)((TF32)c.X * invs);
			Y = (T)((TF32)c.Y * invs);
			Z = (T)((TF32)c.Z * invs);
			W = (T)((TF32)s * 0.5f);
		}

		inline Quaternion<T> exp () const
		{
			// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
			// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

			TF32 fAngle = TLunaEngine::FastSqrt((TF32)(X*X+Y*Y+Z*Z));
			TF32 fSin = sinf(fAngle);

			Quaternion<T> kResult;
			kResult.W = cosf(fAngle);

			if ( TLunaEngine::abs_(fSin) >= (T)0.001f )
			{
				TF32 fCoeff = fSin/fAngle;
				kResult.X = fCoeff*X;
				kResult.Y = fCoeff*Y;
				kResult.Z = fCoeff*Z;
			}
			else
			{
				kResult.X = X;
				kResult.Y = Y;
				kResult.Z = Z;
			}

			return kResult;
		}

        inline Quaternion<T> log () const
		{
			// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
			// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
			// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

			Quaternion<T> kResult;
			kResult.W = 0.0;

			if ( TLunaEngine::abs_(W) < 1.0 )
			{
				TF32 fAngle = acosf((TF32)w);
				TF32 fSin = sinf(fAngle);
				if ( TLunaEngine::abs_(fSin) >= (T)0.001f )
				{
					TF32 fCoeff = fAngle/fSin;
					kResult.X = fCoeff*X;
					kResult.Y = fCoeff*Y;
					kResult.Z = fCoeff*Z;
					return kResult;
				}
			}

			kResult.X = X;
			kResult.Y = Y;
			kResult.Z = Z;

			return kResult;
		}

		//! Quaternion elements.
		T X, Y, Z, W;
	};

	// define float Quaternion
	typedef Quaternion<TF32> QuaternionFloat;
	// define double Quaternion
	typedef Quaternion<TF64> QuaternionDouble;
	// define int Quaternion
	typedef Quaternion<TS32> QuaternionInt;
	// define uint Quaternion
	typedef Quaternion<TU32> QuaternionUInt;
}

#endif