#ifndef _TLDATETIME_H_
#define _TLDATETIME_H_

#include "TLString.h"

namespace TLunaEngine{

	// ---------- 日期时间类 -----------------
	// 综合处理日期和时间
	class DateTime
	{
	public:
		DateTime(TVOID);
		~DateTime(TVOID);
		DateTime(TS32 year,TS32 month,TS32 day);
		DateTime(const DateTime& dateTime);
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
		TS32 m_Year;
		TS32 m_Month;
		TS32 m_Day;
	public:
		// 得到字符串
		DAY_OF_WEEK GetDayOfWeek();
		String GetDayOfWeekString();
		String GetDateString();
		// 得到数值
		inline TS32 GetYear()
		{
			return m_Year;
		}
		inline TS32 GetMonth()
		{
			return m_Month;
		}
		inline TS32 GetDay()
		{
			return m_Day;
		}
		// 设置值
		inline TVOID SetDateTime(TS32 year,TS32 month,TS32 day)
		{
			m_Year = year;
			m_Month = month;
			m_Day = day;
		}
		// 这个nDays可以是负数，那就是减
		TVOID AddDays(TS32 nDays);
		// 是否和另一个相等
		TBOOL IsEqual(const DateTime& dateTime);
	private:
		TVOID AddOneDay();
		TVOID SubOneDay();
	};

}

#endif