#ifndef _TLMATRIX4X4_H_
#define _TLMATRIX4X4_H_
#include "TLVector3.h"
#include "TLVector2.h"
#include "TLVector4.h"
#include "TLQuaternion.h"
#include "TLMatrix3x3.h"
namespace TLunaEngine{

	template<typename T>
	class Matrix4x4
	{
	public:
		Matrix4x4(TVOID)
		{
		}
		~Matrix4x4(TVOID)
		{
		}
		Matrix4x4(	T m00, T m01, T m02, T m03,
					T m10, T m11, T m12, T m13,
					T m20, T m21, T m22, T m23,
					T m30, T m31, T m32, T m33)
		{
			M[0][0] = m00;	M[0][1] = m01;	M[0][2] = m02;	M[0][3] = m03;
			M[1][0] = m10;	M[1][1] = m11;	M[1][2] = m12;	M[1][3] = m13;
			M[2][0] = m20;	M[2][1] = m21;	M[2][2] = m22;	M[2][3] = m23;
			M[3][0] = m30;	M[3][1] = m31;	M[3][2] = m32;	M[3][3] = m33;
		}
		Matrix4x4(const Matrix4x4<T>& mat)
		{
			for(TUByte i=0;i<4;i++)
				for(TUByte j=0;j<4;j++)
					M[i][j] = mat.M[i][j];
		}

		Matrix4x4(const T* arr)
		{
			memcpy(M,arr,16*sizeof(T));
		}
	protected:
		T M[4][4];
	public:
		// 置0
		inline TVOID SetZero()
		{
			for(TUByte i=0;i<4;i++)
				for(TUByte j=0;j<4;j++)
					M[i][j] = (T)0;
		}
		//! Sets the identity matrix.
		inline TVOID Identity()
		{ 
			SetZero(); 
			M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1.0f;	
		}
		//! Checks for identity
		inline TBOOL IsIdentity()
		{
			if(IR(M[0][0])!=IEEE_1_0)	return TFALSE;
			if(IR(M[0][1])!=0)			return TFALSE;
			if(IR(M[0][2])!=0)			return TFALSE;
			if(IR(M[0][3])!=0)			return TFALSE;

			if(IR(M[1][0])!=0)			return TFALSE;
			if(IR(M[1][1])!=IEEE_1_0)	return TFALSE;
			if(IR(M[1][2])!=0)			return TFALSE;
			if(IR(M[1][3])!=0)			return TFALSE;

			if(IR(M[2][0])!=0)			return TFALSE;
			if(IR(M[2][1])!=0)			return TFALSE;
			if(IR(M[2][2])!=IEEE_1_0)	return TFALSE;
			if(IR(M[2][3])!=0)			return TFALSE;

			if(IR(M[3][0])!=0)			return TFALSE;
			if(IR(M[3][1])!=0)			return TFALSE;
			if(IR(M[3][2])!=0)			return TFALSE;
			if(IR(M[3][3])!=IEEE_1_0)	return TFALSE;
			return TTRUE;
		}
		// 重载=
		inline TVOID operator=(const Matrix4x4<T>& mat)
		{
			for(TUByte i=0;i<4;i++)
				for(TUByte j=0;j<4;j++)
					M[i][j] = mat.M[i][j];
		}
		// 直接赋值
		inline Matrix4x4<T>& Set(	T m00, T m01, T m02, T m03,
											T m10, T m11, T m12, T m13,
											T m20, T m21, T m22, T m23,
											T m30, T m31, T m32, T m33)
		{
			M[0][0] = m00;	M[0][1] = m01;	M[0][2] = m02;	M[0][3] = m03;
			M[1][0] = m10;	M[1][1] = m11;	M[1][2] = m12;	M[1][3] = m13;
			M[2][0] = m20;	M[2][1] = m21;	M[2][2] = m22;	M[2][3] = m23;
			M[3][0] = m30;	M[3][1] = m31;	M[3][2] = m32;	M[3][3] = m33;
			return *this;
		}
		// 重载[]
		inline T* operator[](TUByte iRow)
		{
			return M[iRow];
		}
		// 得到T[16]
		inline TVOID ToTArray(T* value)
		{
			//T* temp = value;
			memcpy(&value[0],M[0],sizeof(T)*4);
			//temp += sizeof(T)*4;
			memcpy(&value[4],M[1],sizeof(T)*4);
			//temp += sizeof(T)*4;
			memcpy(&value[8],M[2],sizeof(T)*4);
			//temp += sizeof(T)*4;
			memcpy(&value[12],M[3],sizeof(T)*4);
			//temp += sizeof(T)*4;
		}
		// 得到一行
		inline TVOID GetRow(TUByte iRow,T* value)
		{
			if(!value)
				return;
			for(TUByte i=0;i<4;i++)
				value[i] = M[iRow][i];
		}
		// 赋值一行
		inline TVOID SetRow(TUByte iRow,T* value)
		{
			if(!value)
				return;
			for(TUByte i=0;i<4;i++)
				M[iRow][i] = value[i];
		}
		// 得到一列
		inline TVOID GetCol(TUByte iCol,T* value)
		{
			if(!value)
				return;
			for(TUByte i=0;i<4;i++)
				value[i] = M[i][iCol];
		}
		// 赋值一列
		inline TVOID SetCol(TUByte iCol,T* value)
		{
			if(!value)
				return;
			for(TUByte i=0;i<4;i++)
				M[i][iCol] = value[i];
		}
		// 得到偏移分量
		inline Vector3<T> GetTrans()
		{
			Vector3<T> trans(M[3][0],M[3][1],M[3][2]);
			return trans;
		}
		// 设置偏移分量
		inline TVOID SetTrans(const Vector3<T>& value)
		{
			Identity();
			M[3][0] = value.X;
			M[3][1] = value.Y;
			M[3][2] = value.Z;
		}
		// 去掉偏移分量
		inline Matrix4x4<T>& NoTrans()
		{
			M[3][0] = M[3][1] = M[3][2] = (T)0;
			return *this;
		}
		// 得到去掉偏移分量的矩阵
		inline Matrix4x4<T> GetNoTrans()
		{
			Matrix4x4<T> m(*this);
			m[3][0] = m[3][1] = m[3][2] = (T)0;
			return m;
		}
		// 得到缩放分量
		inline Vector3<T> GetScale()
		{
			// See http://www.robertblum.com/articles/2005/02/14/decomposing-matrices

			// Deal with the 0 rotation case first
			// Prior to Irrlicht 1.6, we always returned this value.
			if(TLunaEngine::iszero(M[0][1]) && TLunaEngine::iszero(M[0][2]) &&
				TLunaEngine::iszero(M[1][0]) && TLunaEngine::iszero(M[1][2]) &&
				TLunaEngine::iszero(M[2][0]) && TLunaEngine::iszero(M[2][1]))
				return Vector3<T>(M[0][0], M[1][1], M[2][2]);

			// We have to do the full calculation.
			return Vector3<T>((T)TLunaEngine::Fastsqrt(M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2]),
								(T)TLunaEngine::Fastsqrt(M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2]),
								(T)TLunaEngine::Fastsqrt(M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2]));
		}
		// get rotation data
		inline Quaternion<T> GetRotation()
		{
			T X,Y,Z,W;
			T diag = M[0][0] + M[1][1] + M[2][2] + 1;
			TF32 scale = 0.0f;

			if( diag > (T)0.0f )
			{
				scale = sqrtf(diag) * 2.0f; // get scale from diagonal

				// TODO: speed this up
				X = (T)((TF32)( m[2][1] - m[1][2]) / scale);
				Y = (T)((TF32)( m[0][2] - m[2][0]) / scale);
				Z = (T)((TF32)( m[1][0] - m[0][1]) / scale);
				W = (T)(0.25f * scale);
			}
			else
			{
				if ( m[0][0] > m[1][1] && m[0][0] > m[2][2])
				{
					// 1st element of diag is greatest value
					// find scale according to 1st element, and double it
					scale = sqrtf( 1.0f + (TF32)m[0][0] - (TF32)m[1][1] - (TF32)m[2][2]) * 2.0f;

					// TODO: speed this up
					X = (T)(0.25f * scale);
					Y = (T)((TF32)(m[0][1] + m[1][0]) / scale);
					Z = (T)((TF32)(m[2][0] + m[0][2]) / scale);
					W = (T)((TF32)(m[2][1] - m[1][2]) / scale);
				}
				else if ( m[1][1] > m[2][2])
				{
					// 2nd element of diag is greatest value
					// find scale according to 2nd element, and double it
					scale = sqrtf( 1.0f + (TF32)m[1][1] - (TF32)m[0][0] - (TF32)m[2][2]) * 2.0f;

					// TODO: speed this up
					X = (T)((TF32)(m[0][1] + m[1][0] ) / scale);
					Y = (T)(0.25f * scale);
					Z = (T)((TF32)(m[1][2] + m[2][1] ) / scale);
					W = (T)((TF32)(m[0][2] - m[2][0] ) / scale);
				}
				else
				{
					// 3rd element of diag is greatest value
					// find scale according to 3rd element, and double it
					scale = sqrtf( 1.0f + (TF32)m[2][2] - (TF32)m[0][0] - (TF32)m[1][1]) * 2.0f;

					// TODO: speed this up
					X = (T)((TF32)(m[0][2] + m[2][0]) / scale);
					Y = (T)((TF32)(m[1][2] + m[2][1]) / scale);
					Z = (T)(0.25f * scale);
					W = (T)((TF32)(m[1][0] - m[0][1]) / scale);
				}
			}

			Quaternion<T> rot(X,Y,Z,W);
			rot.normalize();
			return rot;
		}
		// 设置缩放分量
		inline TVOID SetScale(const Vector3<T>& value)
		{
			Identity();
			M[0][0] = value.X;
			M[1][1] = value.Y;
			M[2][2] = value.Z;
		}
		// 设置绕X轴旋转
		inline TVOID SetRotX(TF32 degrees)
		{
			degrees *= DEGTORAD;
			TF32 Cos = cosf(degrees), Sin = sinf(degrees); 
			Identity(); 
			M[1][1] = M[2][2] = (T)Cos; 
			M[2][1] = (T)(-Sin);	
			M[1][2] = (T)Sin;	
		}
		// 设置绕Y轴旋转
		inline TVOID SetRotY(TF32 degrees)	
		{ 
			degrees *= DEGTORAD;
			TF32 Cos = cosf(degrees), Sin = sinf(degrees); 
			Identity(); 
			M[0][0] = M[2][2] = (T)Cos; 
			M[2][0] = (T)Sin;	
			M[0][2] = (T)(-Sin);	
		}
		// 设置绕Z轴旋转
		inline TVOID SetRotZ(TF32 degrees)	
		{ 
			degrees *= DEGTORAD;
			TF32 Cos = cosf(degrees), Sin = sinf(degrees); 
			Identity(); 
			M[0][0] = M[1][1] = (T)Cos; 
			M[1][0] = (T)(-Sin);	
			M[0][1] = (T)Sin;	
		}
		// 设置旋转使用欧拉角
		inline TVOID SetRotEuler(const Vector3<T>& rotation)
		{
			Identity(); 
			const TF32 cr = cosf( (TF32)rotation.X );
			const TF32 sr = sinf( (TF32)rotation.X );
			const TF32 cp = cosf( (TF32)rotation.Y );
			const TF32 sp = sinf( (TF32)rotation.Y );
			const TF32 cy = cosf( (TF32)rotation.Z );
			const TF32 sy = sinf( (TF32)rotation.Z );

			M[0][0] = (T)( cp*cy );
			M[0][1] = (T)( cp*sy );
			M[0][2] = (T)( -sp );

			const TF32 srsp = sr*sp;
			const TF32 crsp = cr*sp;

			M[1][0] = (T)( srsp*cy-cr*sy );
			M[1][1] = (T)( srsp*sy+cr*cy );
			M[1][2] = (T)( sr*cp );

			M[2][0] = (T)( crsp*cy+sr*sy );
			M[2][1] = (T)( crsp*sy-sr*cy );
			M[2][2] = (T)( cr*cp );
		}
		// from quaternion rotation
		inline Matrix4x4<T>& fromQuaternion(const Quaternion<T>& rot)
		{
			M[0][0] = (T)(1.0f - 2.0f*rot.Y*rot.Y - 2.0f*rot.Z*rot.Z);
			M[1][0] = (T)(2.0f*rot.X*rot.Y + 2.0f*rot.Z*rot.W);
			M[2][0] = (T)(2.0f*rot.X*rot.Z - 2.0f*rot.Y*rot.W);
			M[3][0] = (T)(0.0f);

			M[0][1] = (T)(2.0f*rot.X*rot.Y - 2.0f*rot.Z*rot.W);
			M[1][1] = (T)(1.0f - 2.0f*rot.X*rot.X - 2.0f*rot.Z*rot.Z);
			M[2][1] = (T)(2.0f*rot.Z*rot.Y + 2.0f*rot.X*rot.W);
			M[3][1] = (T)(0.0f);

			M[0][2] = (T)(2.0f*rot.X*rot.Z + 2.0f*rot.Y*rot.W);
			M[1][2] = (T)(2.0f*rot.Z*rot.Y - 2.0f*rot.X*rot.W);
			M[2][2] = (T)(1.0f - 2.0f*rot.X*rot.X - 2.0f*rot.Y*rot.Y);
			M[3][2] = (T)(0.0f);

			M[0][3] = (T)0.0f;
			M[1][3] = (T)0.0f;
			M[2][3] = (T)0.0f;
			M[3][3] = (T)1.0f;

			return *this;
		}
		// 得到当前矩阵的转置矩阵
		inline Matrix4x4<T> GetTranspose()
		{
			Matrix4x4<T> out;
			for(TUByte i=0;i<4;i++)
				for(TUByte j=0;j<4;j++)
					out[i][j] = M[j][i];
			return out;
		}
		// 使自己转置
		inline Matrix4x4<T>& Transpose()
		{
			(*this) = GetTranspose();
			return *this;
		}
		// 得到当前矩阵的逆矩阵
		inline Matrix4x4<T> GetInverse()
		{
			/// Calculates the inverse of this Matrix
			/// The inverse is calculated using Cramers rule.
			/// If no inverse exists then 'TFALSE' is returned.
			if ( this->IsIdentity() )
			{
				return *this;
			}

			Matrix4x4<T> out;

			TF32 d = (M[0][0] * M[1][1] - M[0][1] * M[1][0]) * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				(M[0][0] * M[1][2] - M[0][2] * M[1][0]) * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) +
				(M[0][0] * M[1][3] - M[0][3] * M[1][0]) * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) +
				(M[0][1] * M[1][2] - M[0][2] * M[1][1]) * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) -
				(M[0][1] * M[1][3] - M[0][3] * M[1][1]) * (M[2][0] * M[3][2] - M[2][2] * M[3][0]) +
				(M[0][2] * M[1][3] - M[0][3] * M[1][2]) * (M[2][0] * M[3][1] - M[2][1] * M[3][0]);

			if(TLunaEngine::iszero(d))
				return out;

			d = TLunaEngine::reciprocal(d);

			out[0][0] = (T)( d * (M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) +
					M[1][2] * (M[2][3] * M[3][1] - M[2][1] * M[3][3]) +
					M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])));
			out[0][1] = (T)( d * (M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
					M[2][2] * (M[0][3] * M[3][1] - M[0][1] * M[3][3]) +
					M[2][3] * (M[0][1] * M[3][2] - M[0][2] * M[3][1])));
			out[0][2] = (T)( d * (M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2]) +
					M[3][2] * (M[0][3] * M[1][1] - M[0][1] * M[1][3]) +
					M[3][3] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])));
			out[0][3] = (T)( d * (M[0][1] * (M[1][3] * M[2][2] - M[1][2] * M[2][3]) +
					M[0][2] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) +
					M[0][3] * (M[1][2] * M[2][1] - M[1][1] * M[2][2])));
			out[1][0] = (T)( d * (M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
					M[1][3] * (M[2][2] * M[3][0] - M[2][0] * M[3][2]) +
					M[1][0] * (M[2][3] * M[3][2] - M[2][2] * M[3][3])));
			out[1][1] = (T)( d * (M[2][2] * (M[0][0] * M[3][3] - M[0][3] * M[3][0]) +
					M[2][3] * (M[0][2] * M[3][0] - M[0][0] * M[3][2]) +
					M[2][0] * (M[0][3] * M[3][2] - M[0][2] * M[3][3])));
			out[1][2] = (T)( d * (M[3][2] * (M[0][0] * M[1][3] - M[0][3] * M[1][0]) +
					M[3][3] * (M[0][2] * M[1][0] - M[0][0] * M[1][2]) +
					M[3][0] * (M[0][3] * M[1][2] - M[0][2] * M[1][3])));
			out[1][3] = (T)( d * (M[0][2] * (M[1][3] * M[2][0] - M[1][0] * M[2][3]) +
					M[0][3] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]) +
					M[0][0] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])));
			out[2][0] = (T)( d * (M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]) +
					M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) +
					M[1][1] * (M[2][3] * M[3][0] - M[2][0] * M[3][3])));
			out[2][1] = (T)( d * (M[2][3] * (M[0][0] * M[3][1] - M[0][1] * M[3][0]) +
					M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
					M[2][1] * (M[0][3] * M[3][0] - M[0][0] * M[3][3])));
			out[2][2] = (T)( d * (M[3][3] * (M[0][0] * M[1][1] - M[0][1] * M[1][0]) +
					M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1]) +
					M[3][1] * (M[0][3] * M[1][0] - M[0][0] * M[1][3])));
			out[2][3] = (T)( d * (M[0][3] * (M[1][1] * M[2][0] - M[1][0] * M[2][1]) +
					M[0][0] * (M[1][3] * M[2][1] - M[1][1] * M[2][3]) +
					M[0][1] * (M[1][0] * M[2][3] - M[1][3] * M[2][0])));
			out[3][0] = (T)( d * (M[1][0] * (M[2][2] * M[3][1] - M[2][1] * M[3][2]) +
					M[1][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]) +
					M[1][2] * (M[2][1] * M[3][0] - M[2][0] * M[3][1])));
			out[3][1] = (T)( d * (M[2][0] * (M[0][2] * M[3][1] - M[0][1] * M[3][2]) +
					M[2][1] * (M[0][0] * M[3][2] - M[0][2] * M[3][0]) +
					M[2][2] * (M[0][1] * M[3][0] - M[0][0] * M[3][1])));
			out[3][2] = (T)( d * (M[3][0] * (M[0][2] * M[1][1] - M[0][1] * M[1][2]) +
					M[3][1] * (M[0][0] * M[1][2] - M[0][2] * M[1][0]) +
					M[3][2] * (M[0][1] * M[1][0] - M[0][0] * M[1][1])));
			out[3][3] = (T)( d * (M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) +
					M[0][1] * (M[1][2] * M[2][0] - M[1][0] * M[2][2]) +
					M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0])));
			return out;
		}
		// 将自己逆矩阵
		inline Matrix4x4<T>& Inverse()
		{
			if(IsIdentity())
				return *this;
			(*this) = GetInverse();
			return *this;
		}
		// this + mat
		inline Matrix4x4<T>	operator+(const Matrix4x4<T>& mat)
		{
			return Matrix4x4<T>(
			M[0][0]+mat.M[0][0], M[0][1]+mat.M[0][1], M[0][2]+mat.M[0][2], M[0][3]+mat.M[0][3], 
			M[1][0]+mat.M[1][0], M[1][1]+mat.M[1][1], M[1][2]+mat.M[1][2], M[1][3]+mat.M[1][3], 
			M[2][0]+mat.M[2][0], M[2][1]+mat.M[2][1], M[2][2]+mat.M[2][2], M[2][3]+mat.M[2][3], 
			M[3][0]+mat.M[3][0], M[3][1]+mat.M[3][1], M[3][2]+mat.M[3][2], M[3][3]+mat.M[3][3]);
		}
		// this + mat
		inline Matrix4x4<T>& operator+=(const Matrix4x4<T>& mat)
		{
			M[0][0]+=mat.M[0][0];	M[0][1]+=mat.M[0][1];	M[0][2]+=mat.M[0][2];	M[0][3]+=mat.M[0][3];
			M[1][0]+=mat.M[1][0];	M[1][1]+=mat.M[1][1];	M[1][2]+=mat.M[1][2];	M[1][3]+=mat.M[1][3];
			M[2][0]+=mat.M[2][0];	M[2][1]+=mat.M[2][1];	M[2][2]+=mat.M[2][2];	M[2][3]+=mat.M[2][3];
			M[3][0]+=mat.M[3][0];	M[3][1]+=mat.M[3][1];	M[3][2]+=mat.M[3][2];	M[3][3]+=mat.M[3][3];
			return	*this;
		}

		// this - mat
		inline Matrix4x4<T> operator-(const Matrix4x4<T>& mat)
		{
			return Matrix4x4<T>(
			M[0][0]-mat.M[0][0], M[0][1]-mat.M[0][1], M[0][2]-mat.M[0][2], M[0][3]-mat.M[0][3], 
			M[1][0]-mat.M[1][0], M[1][1]-mat.M[1][1], M[1][2]-mat.M[1][2], M[1][3]-mat.M[1][3], 
			M[2][0]-mat.M[2][0], M[2][1]-mat.M[2][1], M[2][2]-mat.M[2][2], M[2][3]-mat.M[2][3], 
			M[3][0]-mat.M[3][0], M[3][1]-mat.M[3][1], M[3][2]-mat.M[3][2], M[3][3]-mat.M[3][3]);
		}
		// this - mat
		inline Matrix4x4<T>& operator-=(const Matrix4x4<T>& mat)
		{
			M[0][0]-=mat.M[0][0];	M[0][1]-=mat.M[0][1];	M[0][2]-=mat.M[0][2];	M[0][3]-=mat.M[0][3];
			M[1][0]-=mat.M[1][0];	M[1][1]-=mat.M[1][1];	M[1][2]-=mat.M[1][2];	M[1][3]-=mat.M[1][3];
			M[2][0]-=mat.M[2][0];	M[2][1]-=mat.M[2][1];	M[2][2]-=mat.M[2][2];	M[2][3]-=mat.M[2][3];
			M[3][0]-=mat.M[3][0];	M[3][1]-=mat.M[3][1];	M[3][2]-=mat.M[3][2];	M[3][3]-=mat.M[3][3];
			return	*this;
		}
		// -mat
		inline Matrix4x4<T> operator-()
		{
			return Matrix4x4<T>(
			-M[0][0], -M[0][1], -M[0][2], -M[0][3], 
			-M[1][0], -M[1][1], -M[1][2], -M[1][3], 
			-M[2][0], -M[2][1], -M[2][2], -M[2][3], 
			-M[3][0], -M[3][1], -M[3][2], -M[3][3]);
		}
		// this * mat
		inline Matrix4x4<T> operator*(const Matrix4x4<T>& mat)
		{
			return Matrix4x4<T>(
			M[0][0]*mat.M[0][0] + M[0][1]*mat.M[1][0] + M[0][2]*mat.M[2][0] + M[0][3]*mat.M[3][0],
			M[0][0]*mat.M[0][1] + M[0][1]*mat.M[1][1] + M[0][2]*mat.M[2][1] + M[0][3]*mat.M[3][1],
			M[0][0]*mat.M[0][2] + M[0][1]*mat.M[1][2] + M[0][2]*mat.M[2][2] + M[0][3]*mat.M[3][2],
			M[0][0]*mat.M[0][3] + M[0][1]*mat.M[1][3] + M[0][2]*mat.M[2][3] + M[0][3]*mat.M[3][3],

			M[1][0]*mat.M[0][0] + M[1][1]*mat.M[1][0] + M[1][2]*mat.M[2][0] + M[1][3]*mat.M[3][0],
			M[1][0]*mat.M[0][1] + M[1][1]*mat.M[1][1] + M[1][2]*mat.M[2][1] + M[1][3]*mat.M[3][1],
			M[1][0]*mat.M[0][2] + M[1][1]*mat.M[1][2] + M[1][2]*mat.M[2][2] + M[1][3]*mat.M[3][2],
			M[1][0]*mat.M[0][3] + M[1][1]*mat.M[1][3] + M[1][2]*mat.M[2][3] + M[1][3]*mat.M[3][3],

			M[2][0]*mat.M[0][0] + M[2][1]*mat.M[1][0] + M[2][2]*mat.M[2][0] + M[2][3]*mat.M[3][0],
			M[2][0]*mat.M[0][1] + M[2][1]*mat.M[1][1] + M[2][2]*mat.M[2][1] + M[2][3]*mat.M[3][1],
			M[2][0]*mat.M[0][2] + M[2][1]*mat.M[1][2] + M[2][2]*mat.M[2][2] + M[2][3]*mat.M[3][2],
			M[2][0]*mat.M[0][3] + M[2][1]*mat.M[1][3] + M[2][2]*mat.M[2][3] + M[2][3]*mat.M[3][3],

			M[3][0]*mat.M[0][0] + M[3][1]*mat.M[1][0] + M[3][2]*mat.M[2][0] + M[3][3]*mat.M[3][0],
			M[3][0]*mat.M[0][1] + M[3][1]*mat.M[1][1] + M[3][2]*mat.M[2][1] + M[3][3]*mat.M[3][1],
			M[3][0]*mat.M[0][2] + M[3][1]*mat.M[1][2] + M[3][2]*mat.M[2][2] + M[3][3]*mat.M[3][2],
			M[3][0]*mat.M[0][3] + M[3][1]*mat.M[1][3] + M[3][2]*mat.M[2][3] + M[3][3]*mat.M[3][3]);
		}
		// this * mat
		inline Matrix4x4<T>& operator*=(const Matrix4x4<T>& mat)
		{
			(*this) = (*this) * mat;
			return *this;
		}
		// multiply vector4
		inline TVOID multiplyVector4(const Vector4<T>& in, Vector4<T>& out)
		{
			out.X = in.X*M[0][0] + in.Y*M[1][0] + in.Z*M[2][0] + in.W * M[3][0];
			out.Y = in.X*M[0][1] + in.Y*M[1][1] + in.Z*M[2][1] + in.W * M[3][1];
			out.Z = in.X*M[0][2] + in.Y*M[1][2] + in.Z*M[2][2] + in.W * M[3][2];
			out.W = in.X*M[0][3] + in.Y*M[1][3] + in.Z*M[2][3] + in.W * M[3][3];
		}
		// multiply vector4
		inline Vector4<T> operator *(const Vector4<T>& in)
		{
			Vector4<T> out;
			out.X = in.X*M[0][0] + in.Y*M[1][0] + in.Z*M[2][0] + in.W * M[3][0];
			out.Y = in.X*M[0][1] + in.Y*M[1][1] + in.Z*M[2][1] + in.W * M[3][1];
			out.Z = in.X*M[0][2] + in.Y*M[1][2] + in.Z*M[2][2] + in.W * M[3][2];
			out.W = in.X*M[0][3] + in.Y*M[1][3] + in.Z*M[2][3] + in.W * M[3][3];
			return out;
		}
		// this * s
		inline Matrix4x4<T>	operator*(T s)
		{
			return Matrix4x4<T>(
			M[0][0]*s,	M[0][1]*s,	M[0][2]*s,	M[0][3]*s,
			M[1][0]*s,	M[1][1]*s,	M[1][2]*s,	M[1][3]*s,
			M[2][0]*s,	M[2][1]*s,	M[2][2]*s,	M[2][3]*s,
			M[3][0]*s,	M[3][1]*s,	M[3][2]*s,	M[3][3]*s);
		}
		// this * s
		inline Matrix4x4<T>& operator*=(T s)
		{
			M[0][0]*=s;	M[0][1]*=s;	M[0][2]*=s;	M[0][3]*=s;
			M[1][0]*=s;	M[1][1]*=s;	M[1][2]*=s;	M[1][3]*=s;
			M[2][0]*=s;	M[2][1]*=s;	M[2][2]*=s;	M[2][3]*=s;
			M[3][0]*=s;	M[3][1]*=s;	M[3][2]*=s;	M[3][3]*=s;
			return	*this;
		}
		// 变换点
		inline TVOID TransformPoint(const Vector3<T>& in,Vector3<T>& out)
		{
			out.X = in.X*M[0][0] + in.Y*M[1][0] + in.Z*M[2][0] + /*(T)1 **/ M[3][0];
			out.Y = in.X*M[0][1] + in.Y*M[1][1] + in.Z*M[2][1] + /*(T)1 **/ M[3][1];
			out.Z = in.X*M[0][2] + in.Y*M[1][2] + in.Z*M[2][2] + /*(T)1 **/ M[3][2];
		}
		// 变换点
		inline Vector3<T> TransformPoint(const Vector3<T>& in)
		{
			Vector3<T> out;
			out.X = in.X*M[0][0] + in.Y*M[1][0] + in.Z*M[2][0] + /*(T)1 **/ M[3][0];
			out.Y = in.X*M[0][1] + in.Y*M[1][1] + in.Z*M[2][1] + /*(T)1 **/ M[3][1];
			out.Z = in.X*M[0][2] + in.Y*M[1][2] + in.Z*M[2][2] + /*(T)1 **/ M[3][2];
			return out;
		}
		// 变换方向
		inline TVOID TransformDir(const Vector3<T>& in,Vector3<T>& out)
		{
			out.X = in.X*M[0][0] + in.Y*M[1][0] + in.Z*M[2][0] + /*(T)0 * M[3][0]*/0;
			out.Y = in.X*M[0][1] + in.Y*M[1][1] + in.Z*M[2][1] + /*(T)0 * M[3][1]*/0;
			out.Z = in.X*M[0][2] + in.Y*M[1][2] + in.Z*M[2][2] + /*(T)0 * M[3][2]*/0;
		}
		// 变换方向
		inline Vector3<T> TransformDir(const Vector3<T>& in)
		{
			Vector3<T> out;
			out.X = in.X*M[0][0] + in.Y*M[1][0] + in.Z*M[2][0];
			out.Y = in.X*M[0][1] + in.Y*M[1][1] + in.Z*M[2][1];
			out.Z = in.X*M[0][2] + in.Y*M[1][2] + in.Z*M[2][2];
			return out;
		}

		/** Building a Matrix4 from orientation / scale / position.
        @remarks
            Transform is performed in the order scale, rotate, translation, i.e. translation is independent
            of orientation axes, scale does not affect size of translation, rotation and scaling are always
            centered on the origin.
        */
        inline TVOID makeTransform(const Vector3<T>& position, const Vector3<T>& scale, const Quaternion<T>& orientation)
		{
			// Ordering:
			//    1. Scale
			//    2. Rotate
			//    3. Translate

			//Matrix3x3<T> rot3x3;
			//orientation.ToRotationMatrix(rot3x3);
			Vector3<T> rotEuler;
			orientation.toEuler(rotEuler);

			const TF32 cr = cosf( (TF32)rotEuler.X );
			const TF32 sr = sinf( (TF32)rotEuler.X );
			const TF32 cp = cosf( (TF32)rotEuler.Y );
			const TF32 sp = sinf( (TF32)rotEuler.Y );
			const TF32 cy = cosf( (TF32)rotEuler.Z );
			const TF32 sy = sinf( (TF32)rotEuler.Z );

			M[0][0] = (T)( cp*cy ) * scale.X;
			M[0][1] = (T)( cp*sy );
			M[0][2] = (T)( -sp );
			M[0][3] = 0;

			const TF32 srsp = sr*sp;
			const TF32 crsp = cr*sp;

			M[1][0] = (T)( srsp*cy-cr*sy );
			M[1][1] = (T)( srsp*sy+cr*cy ) * scale.Y;
			M[1][2] = (T)( sr*cp );
			M[1][3] = 0;

			M[2][0] = (T)( crsp*cy+sr*sy );
			M[2][1] = (T)( crsp*sy-sr*cy );
			M[2][2] = (T)( cr*cp ) * scale.Z;
			M[2][3] = 0;

			M[3][0] = position.X;
			M[3][1] = position.Y;
			M[3][2] = position.Z;
			M[3][3] = 1;
		}

        /** Building an inverse Matrix4 from orientation / scale / position.
        @remarks
            As makeTransform except it build the inverse given the same data as makeTransform, so
            performing -translation, -rotate, 1/scale in that order.
        */
        inline TVOID makeInverseTransform(const Vector3<T>& position, const Vector3<T>& scale, const Quaternion<T>& orientation)
		{
			// Invert the parameters
			Vector3<T> invTranslate = -position;
			Vector3<T> invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
			Quaternion<T> invRot = orientation.makeInverse();

			// Because we're inverting, order is translation, rotation, scale
			// So make translation relative to scale & rotation
			invTranslate = invRot * invTranslate; // rotate
			invTranslate *= invScale; // scale

			Vector3<T> rotEuler;
			invRot.toEuler(rotEuler);

			const TF32 cr = cosf( (TF32)rotEuler.X );
			const TF32 sr = sinf( (TF32)rotEuler.X );
			const TF32 cp = cosf( (TF32)rotEuler.Y );
			const TF32 sp = sinf( (TF32)rotEuler.Y );
			const TF32 cy = cosf( (TF32)rotEuler.Z );
			const TF32 sy = sinf( (TF32)rotEuler.Z );

			M[0][0] = (T)( cp*cy ) * invScale.X;
			M[0][1] = (T)( cp*sy );
			M[0][2] = (T)( -sp );
			M[0][3] = 0;

			const TF32 srsp = sr*sp;
			const TF32 crsp = cr*sp;

			M[1][0] = (T)( srsp*cy-cr*sy );
			M[1][1] = (T)( srsp*sy+cr*cy ) * invScale.Y;
			M[1][2] = (T)( sr*cp );
			M[1][3] = 0;

			M[2][0] = (T)( crsp*cy+sr*sy );
			M[2][1] = (T)( crsp*sy-sr*cy );
			M[2][2] = (T)( cr*cp ) * invScale.Z;
			M[2][3] = 0;

			M[3][0] = invTranslate.X;
			M[3][1] = invTranslate.Y;
			M[3][2] = invTranslate.Z;
			M[3][3] = 1;
		}

		inline TBOOL isAffine(TVOID) const
        {
            return M[0][3] == 0 && M[1][3] == 0 && M[2][3] == 0 && M[3][3] == 1;
        }

		inline TVOID extract3x3Matrix(Matrix3x3<T>& m3x3) const
        {
            m3x3.m[0][0] = M[0][0];
            m3x3.m[0][1] = M[0][1];
            m3x3.m[0][2] = M[0][2];
            m3x3.m[1][0] = M[1][0];
            m3x3.m[1][1] = M[1][1];
            m3x3.m[1][2] = M[1][2];
            m3x3.m[2][0] = M[2][0];
            m3x3.m[2][1] = M[2][1];
            m3x3.m[2][2] = M[2][2];
        }

        /** Decompose a Matrix4 to orientation / scale / position.
        */
        inline TVOID decomposition(Vector3<T>& position, Vector3<T>& scale, Quaternion<T>& orientation) const
		{
			position = this->GetTrans();
			scale = this->GetScale();
			orientation = this->GetRotation();
		}

		inline TBOOL Matrix4x4<T>::isOrthogonal() const
		{
			T dp=M[0][0] * M[1][0] + M[0][1] * M[1][1] + M[0][2] * M[1][2] + M[0][3] * M[1][3];
			if (!iszero(dp))
				return TFALSE;
			dp = M[0][0] * M[2][0] + M[0][1] * M[2][1] + M[0][2] * M[2][2] + M[0][3] * M[2][3];
			if (!iszero(dp))
				return TFALSE;
			dp = M[0][0] * M[3][0] + M[0][1] * M[3][1] + M[0][2] * M[3][2] + M[0][3] * M[3][3];
			if (!iszero(dp))
				return TFALSE;
			dp = M[1][0] * M[2][0] + M[1][1] * M[2][1] + M[1][2] * M[2][2] + M[1][3] * M[2][3];
			if (!iszero(dp))
				return TFALSE;
			dp = M[1][0] * M[3][0] + M[1][1] * M[3][1] + M[1][2] * M[3][2] + M[1][3] * M[3][3];
			if (!iszero(dp))
				return TFALSE;
			dp = M[2][0] * M[3][0] + M[2][1] * M[3][1] + M[2][2] * M[3][2] + M[2][3] * M[3][3];
			return (iszero(dp));
		}

		// --------------------------------------- 以下为生成特殊矩阵 ------------------------------------

		//! Builds a left-handed perspective projection matrix based on a field of view
		inline Matrix4x4<T>& BuildProjectionMatrixPerspectiveFovLH(TF32 fieldOfViewRadians, TF32 aspectRatio, TF32 zNear, TF32 zFar)
		{
			const TF64 h = 1.0/tan(fieldOfViewRadians/2.0);
			const T w = (T)(h / aspectRatio);

			M[0][0] = w;
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)h;
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(zFar/(zFar-zNear));
			M[2][3] = 1;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(-zNear*zFar/(zFar-zNear));
			M[3][3] = 0;
			return *this;
		}

		//! Builds a right-handed perspective projection matrix based on a field of view
		inline Matrix4x4<T>& BuildProjectionMatrixPerspectiveFovRH(TF32 fieldOfViewRadians, TF32 aspectRatio, TF32 zNear, TF32 zFar)
		{
			const TF64 h = 1.0 / tan(fieldOfViewRadians / 2.0);
			const T w = (T)(h / aspectRatio);

			M[0][0] = w;
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)h;
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(zFar / (zNear - zFar));
			M[2][3] = -1;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(zNear*zFar / (zNear - zFar));
			M[3][3] = 0;
			return *this;
		}

		//! Builds a left-handed perspective projection matrix.
		inline Matrix4x4<T>& BuildProjectionMatrixPerspectiveLH(TF32 widthOfViewVolume, TF32 heightOfViewVolume, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2*zNear/widthOfViewVolume);
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2*zNear/heightOfViewVolume);
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(zFar/(zFar-zNear));
			M[2][3] = 1;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(zNear*zFar/(zNear-zFar));
			M[3][3] = 0;
			return *this;
		}

		//! Builds a right-handed perspective projection matrix.
		inline Matrix4x4<T>& BuildProjectionMatrixPerspectiveRH(TF32 widthOfViewVolume, TF32 heightOfViewVolume, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2 * zNear / widthOfViewVolume);
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2 * zNear / heightOfViewVolume);
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(zFar / (zNear - zFar));
			M[2][3] = -1;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(zNear*zFar / (zNear - zFar));
			M[3][3] = 0;
			return *this;
		}

		// build a left-handed perspective projection matrix by center
		inline Matrix4x4<T>& BuildProjectionMatrixPerspectiveOffCenterLH(TF32 left, TF32 right, TF32 bottom, TF32 top, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2 * zNear / (right - left));
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2 * zNear / (top - bottom));
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = (T)((left + right) / (left - right));
			M[2][1] = (T)((top + bottom) / (bottom - top));
			M[2][2] = (T)(zFar / (zFar - zNear));
			M[2][3] = 1;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(zNear*zFar / (zNear - zFar));
			M[3][3] = 0;
			return *this;
		}

		// build a right-handed perspective projection matrix by center
		inline Matrix4x4<T>& BuildProjectionMatrixPerspectiveOffCenterRH(TF32 left, TF32 right, TF32 bottom, TF32 top, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2 * zNear / (right - left));
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2 * zNear / (top - bottom));
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = (T)((left + right) / (right - left));
			M[2][1] = (T)((top + bottom) / (top - bottom));
			M[2][2] = (T)(zFar / (zNear - zFar));
			M[2][3] = -1;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(zNear*zFar / (zNear - zFar));
			M[3][3] = 0;
			return *this;
		}

		//! Builds a left-handed orthogonal projection matrix.
		inline Matrix4x4<T>& BuildProjectionMatrixOrthoLH(TF32 widthOfViewVolume, TF32 heightOfViewVolume, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2/widthOfViewVolume);
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2/heightOfViewVolume);
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(1/(zFar-zNear));
			M[2][3] = 0;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(zNear/(zNear-zFar));
			M[3][3] = 1;
			return *this;
		}

		//! Builds a right-handed orthogonal projection matrix.
		inline Matrix4x4<T>& BuildProjectionMatrixOrthoRH(TF32 widthOfViewVolume, TF32 heightOfViewVolume, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2 / widthOfViewVolume);
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2 / heightOfViewVolume);
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(1 / (zNear - zFar));
			M[2][3] = 0;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = (T)(zNear / (zNear - zFar));
			M[3][3] = 1;
			return *this;
		}

		// build a left-handed orthogonal projection matrix by center
		inline Matrix4x4<T>& BuildProjectionMatrixOrthoOffCenterLH(TF32 left, TF32 right, TF32 bottom, TF32 top, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2 / (right - left));
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2 / (top - bottom));
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(1 / (zFar - zNear));
			M[2][3] = 0;

			M[3][0] = (T)((left + right) / (left - right));
			M[3][1] = (T)((top + bottom) / (bottom - top));
			M[3][2] = (T)(zNear / (zNear - zFar));
			M[3][3] = 1;
			return *this;
		}

		// build a right-handed orthogonal projection matrix by center
		inline Matrix4x4<T>& BuildProjectionMatrixOrthoOffCenterRH(TF32 left, TF32 right, TF32 bottom, TF32 top, TF32 zNear, TF32 zFar)
		{
			M[0][0] = (T)(2 / (right - left));
			M[0][1] = 0;
			M[0][2] = 0;
			M[0][3] = 0;

			M[1][0] = 0;
			M[1][1] = (T)(2 / (top - bottom));
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = 0;
			M[2][1] = 0;
			M[2][2] = (T)(1 / (zNear - zFar));
			M[2][3] = 0;

			M[3][0] = (T)((left + right) / (left - right));
			M[3][1] = (T)((top + bottom) / (bottom - top));
			M[3][2] = (T)(zNear / (zNear - zFar));
			M[3][3] = 1;
			return *this;
		}

		//! Builds a left-handed look-at matrix.
		inline Matrix4x4<T>& BuildCameraLookAtMatrixLH(
				const Vector3<T>& position,
				const Vector3<T>& target,
				const Vector3<T>& upVector)
		{
			Vector3<T> zaxis = target - position;
			zaxis.normalize();

			Vector3<T> xaxis = upVector.crossProduct(zaxis);
			xaxis.normalize();

			Vector3<T> yaxis = zaxis.crossProduct(xaxis);

			M[0][0] = (T)xaxis.X;
			M[0][1] = (T)yaxis.X;
			M[0][2] = (T)zaxis.X;
			M[0][3] = 0;

			M[1][0] = (T)xaxis.Y;
			M[1][1] = (T)yaxis.Y;
			M[1][2] = (T)zaxis.Y;
			M[1][3] = 0;

			M[2][0] = (T)xaxis.Z;
			M[2][1] = (T)yaxis.Z;
			M[2][2] = (T)zaxis.Z;
			M[2][3] = 0;

			M[3][0] = (T)-xaxis.dotProduct(position);
			M[3][1] = (T)-yaxis.dotProduct(position);
			M[3][2] = (T)-zaxis.dotProduct(position);
			M[3][3] = 1;
			return *this;
		}

		//! Builds a left-handed right-at matrix.
		inline Matrix4x4<T>& BuildCameraLookAtMatrixRH(
			const Vector3<T>& position,
			const Vector3<T>& target,
			const Vector3<T>& upVector)
		{
			Vector3<T> zaxis = position - target;
			zaxis.normalize();

			Vector3<T> xaxis = upVector.crossProduct(zaxis);
			xaxis.normalize();

			Vector3<T> yaxis = zaxis.crossProduct(xaxis);

			M[0][0] = (T)xaxis.X;
			M[0][1] = (T)yaxis.X;
			M[0][2] = (T)zaxis.X;
			M[0][3] = 0;

			M[1][0] = (T)xaxis.Y;
			M[1][1] = (T)yaxis.Y;
			M[1][2] = (T)zaxis.Y;
			M[1][3] = 0;

			M[2][0] = (T)xaxis.Z;
			M[2][1] = (T)yaxis.Z;
			M[2][2] = (T)zaxis.Z;
			M[2][3] = 0;

			M[3][0] = (T)-xaxis.dotProduct(position);
			M[3][1] = (T)-yaxis.dotProduct(position);
			M[3][2] = (T)-zaxis.dotProduct(position);
			M[3][3] = 1;
			return *this;
		}

		//! Builds a matrix that flattens geometry into a plane.
		/** \param light: light source
		\param plane: plane into which the geometry if flattened into
		\param point: value between 0 and 1, describing the light source.
		If this is 1, it is a point light, if it is 0, it is a directional light. */
		//Matrix4x4<T>& buildShadowMatrix(const Vector3<T>& light, core::plane3df plane, TF32 point=1.0f)
		inline Matrix4x4<T>& BuildShadowMatrix(const Vector3<T>& light, const Vector3<T>& planeNormal, T planeD, TF32 point=1.0f)
		{
			planeNormal.normalize();
			const T d = planeNormal.dotProduct(light);

			M[0][0] = (T)(-planeNormal.X * light.X + d);
			M[0][1] = (T)(-planeNormal.X * light.Y);
			M[0][2] = (T)(-planeNormal.X * light.Z);
			M[0][3] = (T)((TF32)(-planeNormal.X) * point);

			M[1][0] = (T)(-planeNormal.Y * light.X);
			M[1][1] = (T)(-planeNormal.Y * light.Y + d);
			M[1][2] = (T)(-planeNormal.Y * light.Z);
			M[1][3] = (T)((TF32)(-planeNormal.Y) * point);

			M[2][0] = (T)(-planeNormal.Z * light.X);
			M[2][1] = (T)(-planeNormal.Z * light.Y);
			M[2][2] = (T)(-planeNormal.Z * light.Z + d);
			M[2][3] = (T)((TF32)(-planeNormal.Z) * point);

			M[3][0] = (T)(-planeD * light.X);
			M[3][1] = (T)(-planeD * light.Y);
			M[3][2] = (T)(-planeD * light.Z);
			M[3][3] = (T)((TF32)(-planeD) * point + (TF32)d);
			return *this;
		}

		// Builds a matrix that reflects the coordinate system about a plane
		inline Matrix4x4<T>& BuildReflectMatrix(TF32 planeA, TF32 planeB, TF32 planeC, TF32 planeD)
		{
			M[0][0] = (T)(-2 * planeA * planeA + 1);
			M[0][1] = (T)(-2 * planeB * planeA);
			M[0][2] = (T)(-2 * planeC * planeA);
			M[0][3] = 0;

			M[1][0] = (T)(-2 * planeA * planeB);
			M[1][1] = (T)(-2 * planeB * planeB + 1);
			M[1][2] = (T)(-2 * planeC * planeB);
			M[1][3] = 0;

			M[2][0] = (T)(-2 * planeA * planeC);
			M[2][1] = (T)(-2 * planeB * planeC);
			M[2][2] = (T)(-2 * planeC * planeC + 1);
			M[2][3] = 0;

			M[3][0] = (T)(-2 * planeA * planeD);
			M[3][1] = (T)(-2 * planeB * planeD);
			M[3][2] = (T)(-2 * planeC * planeD);
			M[3][3] = 1;
			return *this;
		}

		// -----------------------------------------------------------------------------------------------

		// ------------------------------ 和纹理相关的3X3矩阵 --------------------------------------------

		/*
			construct 2D Texture transformations
			rotate about center, scale, and transform.
		*/
		//! Set to a texture transformation matrix with the given parameters.
		inline Matrix4x4<T>& BuildTextureTransform( TF32 rotateRad,
				const Vector2<T> &rotatecenter,
				const Vector2<T> &translate,
				const Vector2<T> &scale)
		{
			Identity();
			const TF32 c = cosf(rotateRad);
			const TF32 s = sinf(rotateRad);

			M[0][0] = (T)(c * scale.X);
			M[0][1] = (T)(s * scale.Y);
			M[0][2] = 0;
			M[0][3] = 0;
			M[1][0] = (T)(-s * scale.X);
			M[1][1] = (T)(c * scale.Y);
			M[1][2] = 0;
			M[1][3] = 0;

			M[2][0] = (T)(c * scale.X * rotatecenter.X + -s * rotatecenter.Y + translate.X);
			M[2][1] = (T)(s * scale.Y * rotatecenter.X +  c * rotatecenter.Y + translate.Y);
			M[2][2] = 1;
			M[2][3] = 0;

			M[3][0] = 0;
			M[3][1] = 0;
			M[3][2] = 0;
			M[3][3] = 1;
			return *this;
		}

		//! Set texture transformation rotation
		/** Rotate about z axis, recenter at (0.5,0.5).
		Doesn't clear other elements than those affected
		\param radAngle Angle in radians
		\return Altered matrix */
		inline Matrix4x4<T>& SetTextureRotationCenter( TF32 radAngle )
		{
			Identity();
			const TF32 c = cosf(radAngle);
			const TF32 s = sinf(radAngle);
			M[0][0] = (T)c;
			M[0][1] = (T)s;
			M[1][0] = (T)-s;
			M[1][1] = (T)c;
			M[2][0] = (T)(0.5f * ( s - c) + 0.5f);
			M[2][1] = (T)(-0.5f * ( s + c) + 0.5f);
			return *this;
		}

		//! Set texture transformation translation
		/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
		inline Matrix4x4<T>& SetTextureTranslate( T x, T y )
		{
			Identity();
			M[2][0] = x;
			M[2][1] = y;
			return *this;
		}

		//! Set texture transformation scale
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		inline Matrix4x4<T>& SetTextureScale( T sx, T sy )
		{
			Identity();
			M[0][0] = sx;
			M[1][1] = sy;
			return *this;
		}

		//! Set texture transformation scale, and recenter at (0.5,0.5)
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		inline Matrix4x4<T>& SetTextureScaleCenter( T sx, T sy )
		{
			Identity();
			M[0][0] = sx;
			M[1][1] = sy;
			M[2][0] = (T)(0.5f - 0.5f * (TF32)sx);
			M[2][1] = (T)(0.5f - 0.5f * (TF32)sy);
			return *this
		}

		// -----------------------------------------------------------------------------------------------
	};

	// define float matrix4x4
	typedef Matrix4x4<TF32> Matrix4x4Float;
	// define double matrix4x4
	typedef Matrix4x4<TF64> Matrix4x4Double;
	// define int matrix4x4
	typedef Matrix4x4<TS32> Matrix4x4Int;
	// define uint matrix4x4
	typedef Matrix4x4<TU32> Matrix4x4UInt;

}

#endif