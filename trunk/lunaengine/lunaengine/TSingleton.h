#pragma once


//namespace TLunaEngine{
//
//	template<class T>
//	class TSingleton
//	{
//	public:
//		TSingleton(void)
//		{
//		}
//
//		virtual ~TSingleton(void)
//		{
//		}
//	public:
//		static T* GetPtr()
//		{
//			if(!m_Ptr)
//				m_Ptr = new T;
//			return m_Ptr;
//		};
//		static void DelPtr()
//		{
//			if(m_Ptr)
//			{
//				delete m_Ptr;
//				m_Ptr = 0;
//			}
//		}
//	protected:
//		static T* m_Ptr;
//	};
//
//}

// Singleton Macro Define
#define T_SINGLETON_DEF(classname) \
	private:\
		static classname* sPtr;\
	public:\
		static classname* getSingletonPtr()\
		{\
			if(!sPtr)\
				sPtr = new classname();\
			return sPtr;\
		}\
		static void delSingletonPtr()\
		{\
			if(sPtr)\
				delete sPtr;\
			sPtr = 0;\
		}

#define T_SINGLETON_IMP(classname) \
	classname* classname::sPtr = 0;