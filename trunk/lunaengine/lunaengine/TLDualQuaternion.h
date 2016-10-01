#ifndef _TLDUALQUATERNION_H_
#define _TLDUALQUATERNION_H_
#include "Quaternion.h"
#include "TLVector3.h"
#include <assert.h>

namespace TLunaEngine{
	// dual quaternion from ogre
	template<typename T>
	class DualQuaternion
	{
	public:
		/// Default constructor, initializes to identity rotation (aka 0?, and zero translation (0,0,0)
		DualQuaternion ()
			: w(1), x(0), y(0), z(0), dw(1), dx(0), dy(0), dz(0)
		{
		}
		
		/// Construct from an explicit list of values
		DualQuaternion (T fW, T fX, T fY, T fZ, 
				T fdW, T fdX, T fdY, T fdZ)
			: w(fW), x(fX), y(fY), z(fZ), dw(fdW), dx(fdX), dy(fdY), dz(fdZ)
		{
		}
        
		/// Construct a dual quaternion from a transformation matrix
		DualQuaternion(const Matrix4x4<T>& rot)
		{
			this->fromTransformationMatrix(rot);
		}
		
		/// Construct a dual quaternion from a unit quaternion and a translation vector
		DualQuaternion(const Quaternion<T>& q, const Vector3<T>& trans)
		{
			this->fromRotationTranslation(q, trans);
		}
		
		/// Construct a dual quaternion from 8 manual w/x/y/z/dw/dx/dy/dz values
		DualQuaternion(T* valptr)
		{
			memcpy(&w, valptr, sizeof(T)*8);
		}

		/// Array accessor operator
		inline T operator [] ( const TSIZE i ) const
		{
			assert( i < 8 );

			return *(&w+i);
		}

		/// Array accessor operator
		inline T& operator [] ( const TSIZE i )
		{
			assert( i < 8 );

			return *(&w+i);
		}
		
		inline DualQuaternion<T>& operator= (const DualQuaternion<T>& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			dw = rkQ.dw;
			dx = rkQ.dx;
			dy = rkQ.dy;
			dz = rkQ.dz;
			
			return *this;
		}

		inline TBOOL operator== (const DualQuaternion<T>& rhs) const
		{
			return (rhs.w == w) && (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && 
				(rhs.dw == dw) && (rhs.dx == dx) && (rhs.dy == dy) && (rhs.dz == dz);
		}

		inline TBOOL operator!= (const DualQuaternion<T>& rhs) const
		{
			return !operator==(rhs);
		}

		/// Pointer accessor for direct copying
		inline T* ptr()
		{
			return &w;
		}

		/// Pointer accessor for direct copying
		inline const T* ptr() const
		{
			return &w;
		}
		
		/// Check whether this dual quaternion contains valid values
		inline TBOOL isNaN() const
		{
			return TLunaEngine::isNaN(w) || TLunaEngine::isNaN(x) || TLunaEngine::isNaN(y) || TLunaEngine::isNaN(z) ||  
				TLunaEngine::isNaN(dw) || TLunaEngine::isNaN(dx) || TLunaEngine::isNaN(dy) || TLunaEngine::isNaN(dz);
		}

		/// Construct a dual quaternion from a rotation described by a Quaternion and a translation described by a Vector3
		TVOID fromRotationTranslation (const Quaternion<T>& q, const Vector3<T>& trans)
		{
			// non-dual part (just copy the quaternion):
			w = q.W;
			x = q.X;
			y = q.Y;
			z = q.Z;
		
			// dual part:
			TF32 half = 0.5f;
			dw = -half *  (trans.X * x + trans.Y * y + trans.Z * z ); 
			dx =  half *  (trans.X * w + trans.Y * z - trans.Z * y ); 
			dy =  half * (-trans.X * z + trans.Y * w + trans.Z * x ); 
			dz =  half *  (trans.X * y - trans.Y * x + trans.Z * w ); 
		}
		
		/// Convert a dual quaternion into its two components, a Quaternion representing the rotation and a Vector3 representing the translation
		TVOID toRotationTranslation (Quaternion<T>& q, Vector3<T>& translation) const
		{
			// regular quaternion (just copy the non-dual part):
			q.W = w;
			q.X = x;
			q.Y = y;
			q.Z = z;

			// translation vector:
			TF32 doub = 2.0;
			translation.X = doub * (-dw*x + dx*w - dy*z + dz*y);
			translation.Y = doub * (-dw*y + dx*z + dy*w - dz*x);
			translation.Z = doub * (-dw*z - dx*y + dy*x + dz*w);
		}

		T w, x, y, z, dw, dx, dy, dz;
	};
}

#endif