#ifndef _TLRAY_H_
#define _TLRAY_H_
#include "TLVector3.h"

// 注意:这里的Ray只实现3D浮点的运算,便于3D计算使用

namespace TLunaEngine{

	template<typename T>
	class Ray
	{
	public:
		Ray(TVOID)
		{
		}
		~Ray(TVOID)
		{
		}
		Ray(const Ray<T>& other)
		{
			(*this) = other;
		}
		Ray(const Vector3<T>& orig,const Vector3<T>& dir)
		{
			m_Orig = orig;
			m_Dir = dir;
			m_Dir.normalize();
		}
	public:
		// ----------- 成员 --------------------

		// 起始点
		Vector3<T> m_Orig;
		// 方向
		Vector3<T> m_Dir;

		// -------------------------------------
	public:
		// --------- 方法 ----------------------

		// 重载=
		inline TVOID operator=(const Ray<T>& other)
		{
			m_Orig = other.m_Orig;
			m_Dir = other.m_Dir;
		}
		// 起始点
		inline Vector3<T>& GetOrig()
		{
			return m_Orig;
		}
		inline TVOID SetOrig(const Vector3<T>& orig)
		{
			m_Orig = orig;
		}
		// 方向
		inline Vector3<T>& GetDir()
		{
			return m_Dir;
		}
		inline TVOID SetDir(const Vector3<T>& dir)
		{
			m_Dir = dir;
			m_Dir.normalize();
		}
		// 得到t步之后的点
		inline Vector3<T> GetDest(TF32 t)
		{
			Vector3<T> pos = m_Orig + m_Dir * t;
			return pos;
		}
		// 往相反方向走t步之后的点
		inline Vector3<T> GetNegDest(TF32 t)
		{
			Vector3<T> pos = m_Orig - m_Dir * t;
			return pos;
		}

		// -------------------------------------
	};

	// define float ray
	typedef Ray<TF32> RayFloat;
	// define double ray
	typedef Ray<TF64> RayDouble;
	// define int ray
	typedef Ray<TS32> RayInt;
	// define uint ray
	typedef Ray<TU32> RayUInt;

}

#endif