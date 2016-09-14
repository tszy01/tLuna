#pragma once
#include "TVector3.h"

// 注意:这里的Ray只实现3D浮点的运算,便于3D计算使用

namespace TLunaEngine{

	template<typename T>
	class TRay
	{
	public:
		TRay(void)
		{
		}
		~TRay(void)
		{
		}
		TRay(const TRay<T>& other)
		{
			(*this) = other;
		}
		TRay(const TVector3<T>& orig,const TVector3<T>& dir)
		{
			m_Orig = orig;
			m_Dir = dir;
			m_Dir.normalize();
		}
	public:
		// ----------- 成员 --------------------

		// 起始点
		TVector3<T> m_Orig;
		// 方向
		TVector3<T> m_Dir;

		// -------------------------------------
	public:
		// --------- 方法 ----------------------

		// 重载=
		inline void operator=(const TRay<T>& other)
		{
			m_Orig = other.m_Orig;
			m_Dir = other.m_Dir;
		}
		// 起始点
		inline TVector3<T>& GetOrig()
		{
			return m_Orig;
		}
		inline void SetOrig(const TVector3<T>& orig)
		{
			m_Orig = orig;
		}
		// 方向
		inline TVector3<T>& GetDir()
		{
			return m_Dir;
		}
		inline void SetDir(const TVector3<T>& dir)
		{
			m_Dir = dir;
			m_Dir.normalize();
		}
		// 得到t步之后的点
		inline TVector3<T> GetDest(T t)
		{
			TVector3<T> pos = m_Orig + m_Dir * t;
			return pos;
		}
		// 往相反方向走t步之后的点
		inline TVector3<T> GetNegDest(T t)
		{
			TVector3<T> pos = m_Orig - m_Dir * t;
			return pos;
		}

		// -------------------------------------
	};

	// define float ray
	typedef TRay<TF32> TRayFloat;
	// define double ray
	typedef TRay<TF64> TRayDouble;
	// define int ray
	typedef TRay<TS32> TRayInt;
	// define uint ray
	typedef TRay<TU32> TRayUInt;

}