#pragma once

#include "TString.h"
using namespace TLunaEngine;

namespace TLunaEngine{

	// ---------- ����ʱ���� -----------------
	// �ۺϴ������ں�ʱ��
	class TDateTime
	{
	public:
		TDateTime(void);
		~TDateTime(void);
		TDateTime(int year,int month,int day);
		TDateTime(const TDateTime& dateTime);
	public:
		enum DAY_OF_WEEK
		{
			DOW_MONDAY		= 0,
			DOW_TUESDAY		= 1,
			DOW_WEDNESDAY	= 2,
			DOW_THURSDAY	= 3,
			DOW_FRIDAY		= 4,
			DOW_SATURDAY	= 5,
			DOW_SUNDAY		= 6,
			DOW_DAY_COUNT
		};
	private:
		int m_Year;
		int m_Month;
		int m_Day;
	public:
		// �õ��ַ���
		DAY_OF_WEEK GetDayOfWeek();
		TString GetDayOfWeekString();
		TString GetDateString();
		// �õ���ֵ
		inline int GetYear()
		{
			return m_Year;
		}
		inline int GetMonth()
		{
			return m_Month;
		}
		inline int GetDay()
		{
			return m_Day;
		}
		// ����ֵ
		inline void SetDateTime(int year,int month,int day)
		{
			m_Year = year;
			m_Month = month;
			m_Day = day;
		}
		// ���nDays�����Ǹ������Ǿ��Ǽ�
		void AddDays(int nDays);
		// �Ƿ����һ�����
		bool IsEqual(const TDateTime& dateTime);
	private:
		void AddOneDay();
		void SubOneDay();
	};

}