#pragma once
#include "TVector3.h"

// ע��:�����Rayֻʵ��3D���������,����3D����ʹ��

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
		// ----------- ��Ա --------------------

		// ��ʼ��
		TVector3<T> m_Orig;
		// ����
		TVector3<T> m_Dir;

		// -------------------------------------
	public:
		// --------- ���� ----------------------

		// ����=
		inline void operator=(const TRay<T>& other)
		{
			m_Orig = other.m_Orig;
			m_Dir = other.m_Dir;
		}
		// ��ʼ��
		inline TVector3<T>& GetOrig()
		{
			return m_Orig;
		}
		inline void SetOrig(const TVector3<T>& orig)
		{
			m_Orig = orig;
		}
		// ����
		inline TVector3<T>& GetDir()
		{
			return m_Dir;
		}
		inline void SetDir(const TVector3<T>& dir)
		{
			m_Dir = dir;
			m_Dir.normalize();
		}
		// �õ�t��֮��ĵ�
		inline TVector3<T> GetDest(T t)
		{
			TVector3<T> pos = m_Orig + m_Dir * t;
			return pos;
		}
		// ���෴������t��֮��ĵ�
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