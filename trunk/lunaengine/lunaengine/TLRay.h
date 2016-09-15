#ifndef _TLRAY_H_
#define _TLRAY_H_
#include "TLVector3.h"

// ע��:�����Rayֻʵ��3D���������,����3D����ʹ��

namespace TLunaEngine{

	template<typename T>
	class Ray
	{
	public:
		Ray(void)
		{
		}
		~Ray(void)
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
		// ----------- ��Ա --------------------

		// ��ʼ��
		Vector3<T> m_Orig;
		// ����
		Vector3<T> m_Dir;

		// -------------------------------------
	public:
		// --------- ���� ----------------------

		// ����=
		inline void operator=(const Ray<T>& other)
		{
			m_Orig = other.m_Orig;
			m_Dir = other.m_Dir;
		}
		// ��ʼ��
		inline Vector3<T>& GetOrig()
		{
			return m_Orig;
		}
		inline void SetOrig(const Vector3<T>& orig)
		{
			m_Orig = orig;
		}
		// ����
		inline Vector3<T>& GetDir()
		{
			return m_Dir;
		}
		inline void SetDir(const Vector3<T>& dir)
		{
			m_Dir = dir;
			m_Dir.normalize();
		}
		// �õ�t��֮��ĵ�
		inline Vector3<T> GetDest(T t)
		{
			Vector3<T> pos = m_Orig + m_Dir * t;
			return pos;
		}
		// ���෴������t��֮��ĵ�
		inline Vector3<T> GetNegDest(T t)
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