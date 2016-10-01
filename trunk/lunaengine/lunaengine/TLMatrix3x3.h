#ifndef _TLMATRIX3X3_H_
#define _TLMATRIX3X3_H_
#include "TLVector3.h"
#include "TLVector2.h"
#include <vector>
#include <assert.h>

namespace TLunaEngine
{
	template<typename T>
	class Matrix3x3
	{
	public:
		Matrix3x3()
		{
		}

		~Matrix3x3()
		{
		}

		Matrix3x3(const T* arr)
		{
			memcpy(m,arr,9*sizeof(T));
		}

		Matrix3x3(const Matrix3x3<T>& right)
		{
			memcpy(m,right.m,9*sizeof(T));
		}

		Matrix3x3( T m00, T m01, T m02, 
					T m10, T m11, T m12,
					T m20, T m21, T m22)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
		}

		// жиди[]
		inline T* operator[](TUByte iRow)
		{
			return m[iRow];
		}

		/// Assignment and comparison
        inline Matrix3x3<T>& operator= (const Matrix3x3<T>& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(T));
			return *this;
		}

		/** Matrix addition.
         */
        inline Matrix3x3<T> operator+ (const Matrix3x3<T>& rkMatrix) const
		{
			Matrix3x3<T> kSum;
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
				{
					kSum.m[iRow][iCol] = m[iRow][iCol] +
						rkMatrix.m[iRow][iCol];
				}
			}
			return kSum;
		}

		inline Matrix3x3<T>& operator +=(const Matrix3x3<T>& rkMatrix)
		{
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
				{
					m[iRow][iCol] = m[iRow][iCol] +
						rkMatrix.m[iRow][iCol];
				}
			}
			return *this;
		}

        /** Matrix subtraction.
         */
        inline Matrix3x3<T> operator- (const Matrix3x3<T>& rkMatrix) const
		{
			Matrix3x3<T> kDiff;
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
				{
					kDiff.m[iRow][iCol] = m[iRow][iCol] -
						rkMatrix.m[iRow][iCol];
				}
			}
			return kDiff;
		}

		inline Matrix3x3<T>& operator-= (const Matrix3x3<T>& rkMatrix)
		{
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
				{
					m[iRow][iCol] = m[iRow][iCol] -
						rkMatrix.m[iRow][iCol];
				}
			}
			return *this;
		}

        /** Matrix concatenation using '*'.
         */
        inline Matrix3x3<T> operator* (const Matrix3x3<T>& rkMatrix) const
		{
			Matrix3x3<T> kProd;
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
				{
					kProd.m[iRow][iCol] =
						m[iRow][0]*rkMatrix.m[0][iCol] +
						m[iRow][1]*rkMatrix.m[1][iCol] +
						m[iRow][2]*rkMatrix.m[2][iCol];
				}
			}
			return kProd;
		}

		inline Matrix3x3<T>& operator*= (const Matrix3x3<T>& rkMatrix)
		{
			(*this) = (*this) * rkMatrix;
			return *this;
		}

        inline Matrix3x3<T> operator- () const
		{
			return Matrix3x3<T>(
				-m[0][0], -m[0][1], -m[0][2], 
				-m[1][0], -m[1][1], -m[1][2], 
				-m[2][0], -m[2][1], -m[2][2]);
		}

        /// Matrix * vector [3x3 * 3x1 = 3x1]
        inline Vector3<T> operator* (const Vector3<T>& rkVector) const
		{
			Vector3<T> kProd;
			for (TUByte iCol = 0; iCol < 3; iCol++)
			{
				kProd[iCol] = rkVector.X*m[iCol][0] + rkVector.Y*m[iCol][1] + rkVector.Z*m[iCol][2];
			}
			return kProd;
		}

		inline TVOID multiplyVector3(const Vector3<T>& rkVector, Vector3<T>& out) const
		{
			for (TUByte iCol = 0; iCol < 3; iCol++)
			{
				out[iCol] = rkVector.X*m[iCol][0] + rkVector.Y*m[iCol][1] + rkVector.Z*m[iCol][2];
			}
		}

		/// Matrix * scalar
        inline Matrix3x3<T> operator* (T fScalar) const
		{
			Matrix3x3<T> kProd;
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
					kProd[iRow][iCol] = fScalar*m[iRow][iCol];
			}
			return kProd;
		}

		inline Matrix3x3<T>& operator*= (T fScalar)
		{
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
					m[iRow][iCol] = fScalar*m[iRow][iCol];
			}
			return *this;
		}

		// жУ0
		inline TVOID setZero()
		{
			for(TUByte i=0;i<3;i++)
				for(TUByte j=0;j<3;j++)
					m[i][j] = (T)0;
		}

		//! Sets the identity matrix.
		inline TVOID identity()
		{ 
			setZero(); 
			m[0][0] = m[1][1] = m[2][2] = 1.0f;	
		}
		//! Checks for identity
		inline TBOOL isIdentity()
		{
			if(IR(m[0][0])!=IEEE_1_0)	return TFALSE;
			if(IR(m[0][1])!=0)			return TFALSE;
			if(IR(m[0][2])!=0)			return TFALSE;

			if(IR(m[1][0])!=0)			return TFALSE;
			if(IR(m[1][1])!=IEEE_1_0)	return TFALSE;
			if(IR(m[1][2])!=0)			return TFALSE;

			if(IR(m[2][0])!=0)			return TFALSE;
			if(IR(m[2][1])!=0)			return TFALSE;
			if(IR(m[2][2])!=IEEE_1_0)	return TFALSE;
			return TTRUE;
		}

		inline Matrix3x3<T> getTranspose() const
		{
			Matrix3 kTranspose;
			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
					kTranspose[iRow][iCol] = m[iCol][iRow];
			}
			return kTranspose;
		}

		inline Matrix3x3<T>& transpose()
		{
			(*this) = getTranspose();
			return *this;
		}

		inline Matrix3x3<T> getInverse() const
		{
			if ( this->IsIdentity() )
			{
				return *this;
			}

			Matrix3x3<T> rkInverse;
			rkInverse[0][0] = m[1][1]*m[2][2] -
				m[1][2]*m[2][1];
			rkInverse[0][1] = m[0][2]*m[2][1] -
				m[0][1]*m[2][2];
			rkInverse[0][2] = m[0][1]*m[1][2] -
				m[0][2]*m[1][1];
			rkInverse[1][0] = m[1][2]*m[2][0] -
				m[1][0]*m[2][2];
			rkInverse[1][1] = m[0][0]*m[2][2] -
				m[0][2]*m[2][0];
			rkInverse[1][2] = m[0][2]*m[1][0] -
				m[0][0]*m[1][2];
			rkInverse[2][0] = m[1][0]*m[2][1] -
				m[1][1]*m[2][0];
			rkInverse[2][1] = m[0][1]*m[2][0] -
				m[0][0]*m[2][1];
			rkInverse[2][2] = m[0][0]*m[1][1] -
				m[0][1]*m[1][0];

			T fDet =
				m[0][0]*rkInverse[0][0] +
				m[0][1]*rkInverse[1][0]+
				m[0][2]*rkInverse[2][0];

			if(TLunaEngine::iszero(fDet))
				return Matrix3x3<T>();

			fDet = TLunaEngine::reciprocal(fDet);

			for (TUByte iRow = 0; iRow < 3; iRow++)
			{
				for (TUByte iCol = 0; iCol < 3; iCol++)
					rkInverse[iRow][iCol] *= fDet;
			}
			return rkInverse;
		}

		inline Matrix3x3<T>& inverse()
		{
			(*this) = getInverse();
			return *this;
		}

		/*
			construct 2D Texture transformations
			rotate about center, scale, and transform.
		*/
		//! Set to a texture transformation matrix with the given parameters.
		inline Matrix3x3<T>& BuildTextureTransform( TF32 rotateRad,
				const Vector2<T> &rotatecenter,
				const Vector2<T> &translate,
				const Vector2<T> &scale)
		{
			identity();
			const TF32 c = cosf(rotateRad);
			const TF32 s = sinf(rotateRad);

			m[0][0] = (T)(c * scale.X);
			m[0][1] = (T)(s * scale.Y);
			m[0][2] = 0;
			m[1][0] = (T)(-s * scale.X);
			m[1][1] = (T)(c * scale.Y);
			m[1][2] = 0;
			m[2][0] = (T)(c * scale.X * rotatecenter.X + -s * rotatecenter.Y + translate.X);
			m[2][1] = (T)(s * scale.Y * rotatecenter.X +  c * rotatecenter.Y + translate.Y);
			m[2][2] = 1;
			return *this;
		}

		//! Set texture transformation rotation
		/** Rotate about z axis, recenter at (0.5,0.5).
		Doesn't clear other elements than those affected
		\param radAngle Angle in radians
		\return Altered matrix */
		inline Matrix3x3<T>& SetTextureRotationCenter( TF32 radAngle )
		{
			identity();
			const TF32 c = cosf(radAngle);
			const TF32 s = sinf(radAngle);
			m[0][0] = (T)c;
			m[0][1] = (T)s;
			m[1][0] = (T)-s;
			m[1][1] = (T)c;
			m[2][0] = (T)(0.5f * ( s - c) + 0.5f);
			m[2][1] = (T)(-0.5f * ( s + c) + 0.5f);
			return *this;
		}

		//! Set texture transformation translation
		/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
		inline Matrix3x3<T>& SetTextureTranslate( T x, T y )
		{
			identity();
			m[2][0] = x;
			m[2][1] = y;
			return *this;
		}

		//! Set texture transformation scale
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		inline Matrix3x3<T>& SetTextureScale( T sx, T sy )
		{
			identity();
			m[0][0] = sx;
			m[1][1] = sy;
			return *this;
		}

		//! Set texture transformation scale, and recenter at (0.5,0.5)
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		inline Matrix3x3<T>& SetTextureScaleCenter( T sx, T sy )
		{
			identity();
			m[0][0] = sx;
			m[1][1] = sy;
			m[2][0] = (T)(0.5f - 0.5f * (TF32)sx);
			m[2][1] = (T)(0.5f - 0.5f * (TF32)sy);
			return *this
		}
	public:
		T m[3][3];
	};

	// define float matrix3x3
	typedef Matrix3x3<TF32> Matrix3x3Float;
	// define double matrix3x3
	typedef Matrix3x3<TF64> Matrix3x3Double;
	// define int matrix3x3
	typedef Matrix3x3<TS32> Matrix3x3Int;
	// define uint matrix3x3
	typedef Matrix3x3<TU32> Matrix3x3UInt;
}

#endif