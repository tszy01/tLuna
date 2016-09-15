#ifndef _TLCONFIGFILE_H_
#define _TLCONFIGFILE_H_

// �ļ������࣬���ڴ������������ļ�
// Name=Value
// ���ܿո񣬲��ܻ���

#include <list>

#include "TLString.h"

namespace TLunaEngine{

	typedef struct _FILESTR
	{
		String m_strParameter;
		String m_strValue;
	}FILESTR,*LPFILESTR;

	typedef std::list<FILESTR> StrList;

	class ConfigFile
	{
	public:
		ConfigFile(void);
	public:
		~ConfigFile(void);
	public:
		// �򿪷�ʽö��
		enum OPEN_FILE_TYPE
		{
			OPEN_NONE,
			OPEN_WRITE,
			OPEN_READ,
			OPEN_WRITE_AND_READ,
		};
	protected:
		// �ļ�ָ��
		FILE* m_stream;
		// �����ǲ����Ѿ��������ļ�
		bool m_bOpen;
		// �������м�ֵ��
		StrList m_list;
		// �򿪵ķ�ʽ
		OPEN_FILE_TYPE m_openType;
		// �����ļ���
		char m_szFileName[256];
	protected:
		// ���ļ�����para��value
		bool LoadFromFile();
	public:
		// �򿪲������ļ�
		bool OpenFile(const char* file,OPEN_FILE_TYPE type);
		// �ر��ļ�
		void CloseFile();
		// �����޸�֮���ֵ���ļ�
		bool UpdateFile(); 
		// �õ�ĳ��ֵ
		void GetParameter(const char* paraName, String* pBuf);
		inline int GetParameterAsInt(const char* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsInt();
		}
		inline unsigned int GetParameterAsUInt(const char* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsUInt();
		}
		inline long GetParameterAsLong(const char* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsLong();
		}
		inline unsigned long GetParameterAsULong(const char* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsULong();
		}
		inline float GetParameterAsFloat(const char* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsFloat();
		}
		inline double GetParameterAsDouble(const char* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsDouble();
		}
		inline bool GetParameterAsBool(const char* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsBool();
		}
		// ���ڴ����޸�ĳ��ֵ
		void ChangeParameter(const char* paraName, const String* pBuf);
		inline void ChangeParameter(const char* paraName, int value)
		{
			String strTmp;
			strTmp.Format("%d",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline void ChangeParameter(const char* paraName, unsigned int value)
		{
			String strTmp;
			strTmp.Format("%u",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline void ChangeParameter(const char* paraName, long value)
		{
			ChangeParameter(paraName,(int)value);
		}
		inline void ChangeParameter(const char* paraName, unsigned long value)
		{
			ChangeParameter(paraName,(int)value);
		}
		inline void ChangeParameter(const char* paraName, float value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline void ChangeParameter(const char* paraName, double value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline void ChangeParameter(const char* paraName, bool value)
		{
			if(value)
				ChangeParameter(paraName,0);
			else
				ChangeParameter(paraName,1);
		}
		// ���һ��
		void AddParameter(const char* paraName, const String* pBuf);
		inline void AddParameter(const char* paraName, int value)
		{
			String strTmp;
			strTmp.Format("%d",value);
			AddParameter(paraName,&strTmp);
		}
		inline void AddParameter(const char* paraName, unsigned int value)
		{
			String strTmp;
			strTmp.Format("%u",value);
			AddParameter(paraName,&strTmp);
		}
		inline void AddParameter(const char* paraName, long value)
		{
			AddParameter(paraName,(int)value);
		}
		inline void AddParameter(const char* paraName, unsigned long value)
		{
			AddParameter(paraName,(int)value);
		}
		inline void AddParameter(const char* paraName, float value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			AddParameter(paraName,&strTmp);
		}
		inline void AddParameter(const char* paraName, double value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			AddParameter(paraName,&strTmp);
		}
		inline void AddParameter(const char* paraName, bool value)
		{
			if(value)
				AddParameter(paraName,0);
			else
				AddParameter(paraName,1);
		}
		// �ҵ�����
		bool FindParameter(const char* paraName, int* pIndex);
		// �õ�List�ĸ���
		inline int GetListNum(){return (int)m_list.size();}
	};

}

#endif