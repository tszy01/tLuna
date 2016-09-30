#ifndef _TLSINGLETON_H_
#define _TLSINGLETON_H_

#include "TLCommonTypeDef.h"
namespace TLunaEngine{

	template<class T>
	class Singleton
	{
	public:
		Singleton(TVOID)
		{
		}

		virtual ~Singleton(TVOID)
		{
		}
	public:
		static T* getSingletonPtr()
		{
			if(!m_Ptr)
				m_Ptr = new T;
			return m_Ptr;
		};
		static TVOID delSingletonPtr()
		{
			if(m_Ptr)
			{
				delete m_Ptr;
				m_Ptr = 0;
			}
		}
	protected:
		static T* m_Ptr;
	};

}

//// Singleton Macro Define
//#define T_SINGLETON_DEF(classname) \
//	private:\
//		static classname* sPtr;\
//	public:\
//		static classname* getSingletonPtr()\
//		{\
//			if(!sPtr)\
//				sPtr = new classname();\
//			return sPtr;\
//		}\
//		static TVOID delSingletonPtr()\
//		{\
//			if(sPtr)\
//				delete sPtr;\
//			sPtr = 0;\
//		}
//
//#define T_SINGLETON_IMP(classname) \
//	classname* classname::sPtr = 0;

#endif