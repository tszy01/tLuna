#ifndef _TLCONFIGFILE_H_
#define _TLCONFIGFILE_H_

// 文件处理类，用于处理以下类型文件
// Name=Value
// 不能空格，不能换行

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
		ConfigFile(TVOID);
	public:
		~ConfigFile(TVOID);
	public:
		// 打开方式枚举
		enum OPEN_FILE_TYPE
		{
			OPEN_NONE,
			OPEN_WRITE,
			OPEN_READ,
			OPEN_WRITE_AND_READ,
		};
	protected:
		// 文件指针
		FILE* m_stream;
		// 现在是不是已经加载了文件
		TBOOL m_bOpen;
		// 保存所有键值对
		StrList m_list;
		// 打开的方式
		OPEN_FILE_TYPE m_openType;
		// 保存文件名
		TCHAR m_szFileName[256];
	protected:
		// 从文件加载para和value
		TBOOL LoadFromFile();
	public:
		// 打开并加载文件
		TBOOL OpenFile(const TCHAR* file,OPEN_FILE_TYPE type);
		// 关闭文件
		TVOID CloseFile();
		// 更新修改之后的值到文件
		TBOOL UpdateFile(); 
		// 得到某个值
		TVOID GetParameter(const TCHAR* paraName, String* pBuf);
		inline int GetParameterAsInt(const TCHAR* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsInt();
		}
		inline unsigned int GetParameterAsUInt(const TCHAR* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsUInt();
		}
		inline long GetParameterAsLong(const TCHAR* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsLong();
		}
		inline unsigned long GetParameterAsULong(const TCHAR* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsULong();
		}
		inline float GetParameterAsFloat(const TCHAR* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsFloat();
		}
		inline double GetParameterAsDouble(const TCHAR* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsDouble();
		}
		inline TBOOL GetParameterAsBool(const TCHAR* paraName)
		{
			String strTmp;
			GetParameter(paraName,&strTmp);
			return strTmp.GetStringAsBool();
		}
		// 在内存中修改某个值
		TVOID ChangeParameter(const TCHAR* paraName, const String* pBuf);
		inline TVOID ChangeParameter(const TCHAR* paraName, int value)
		{
			String strTmp;
			strTmp.Format("%d",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline TVOID ChangeParameter(const TCHAR* paraName, unsigned int value)
		{
			String strTmp;
			strTmp.Format("%u",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline TVOID ChangeParameter(const TCHAR* paraName, long value)
		{
			ChangeParameter(paraName,(int)value);
		}
		inline TVOID ChangeParameter(const TCHAR* paraName, unsigned long value)
		{
			ChangeParameter(paraName,(int)value);
		}
		inline TVOID ChangeParameter(const TCHAR* paraName, float value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline TVOID ChangeParameter(const TCHAR* paraName, double value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			ChangeParameter(paraName,&strTmp);
		}
		inline TVOID ChangeParameter(const TCHAR* paraName, TBOOL value)
		{
			if(value)
				ChangeParameter(paraName,0);
			else
				ChangeParameter(paraName,1);
		}
		// 添加一组
		TVOID AddParameter(const TCHAR* paraName, const String* pBuf);
		inline TVOID AddParameter(const TCHAR* paraName, int value)
		{
			String strTmp;
			strTmp.Format("%d",value);
			AddParameter(paraName,&strTmp);
		}
		inline TVOID AddParameter(const TCHAR* paraName, unsigned int value)
		{
			String strTmp;
			strTmp.Format("%u",value);
			AddParameter(paraName,&strTmp);
		}
		inline TVOID AddParameter(const TCHAR* paraName, long value)
		{
			AddParameter(paraName,(int)value);
		}
		inline TVOID AddParameter(const TCHAR* paraName, unsigned long value)
		{
			AddParameter(paraName,(int)value);
		}
		inline TVOID AddParameter(const TCHAR* paraName, float value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			AddParameter(paraName,&strTmp);
		}
		inline TVOID AddParameter(const TCHAR* paraName, double value)
		{
			String strTmp;
			strTmp.Format("%f",value);
			AddParameter(paraName,&strTmp);
		}
		inline TVOID AddParameter(const TCHAR* paraName, TBOOL value)
		{
			if(value)
				AddParameter(paraName,0);
			else
				AddParameter(paraName,1);
		}
		// 找到名字
		TBOOL FindParameter(const TCHAR* paraName, int* pIndex);
		// 得到List的个数
		inline int GetListNum(){return (int)m_list.size();}
	};

}

#endif