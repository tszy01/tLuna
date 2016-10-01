#include "TLDateTime.h"

namespace TLunaEngine{

	DateTime::DateTime(TVOID) : m_Year(0),m_Month(1),m_Day(1)
	{
	}

	DateTime::~DateTime(TVOID)
	{
	}

	DateTime::DateTime(TS32 year, TS32 month, TS32 day) : m_Year(year),m_Month(month),m_Day(day)
	{
	}

	DateTime::DateTime(const DateTime& dateTime) : m_Year(dateTime.m_Year),m_Month(dateTime.m_Month),m_Day(dateTime.m_Day)
	{
	}

	TVOID DateTime::AddDays(TS32 nDays)
	{
		if(nDays==0)
			return;
		else if(nDays>0)
		{
			TS32 count = nDays;
			while(count>0)
			{
				AddOneDay();
				count--;
			}
		}
		else if(nDays<0)
		{
			TS32 count = nDays * -1;
			while(count>0)
			{
				SubOneDay();
				count--;
			}
		}
	}

	TVOID DateTime::AddOneDay()
	{
		TS32 newDay = m_Day + 1;
		TBOOL bRun = m_Year % 4 == 0 && m_Year % 100 !=0 || m_Year % 400 ==0; //闰年可以被4或者400整除 但是不能被100整除
		TBOOL bAddMonth = TFALSE;
		if(m_Month==2)
		{
			if(bRun)
			{
				if(newDay>=30)
					bAddMonth = TTRUE;
			}
			else
			{
				if(newDay>=29)
					bAddMonth = TTRUE;
			}
		}
		else if(m_Month==1 || m_Month==3 || m_Month==5 || m_Month==7 || m_Month==8 || m_Month==10 || m_Month==12)
		{
			if(newDay>=32)
				bAddMonth = TTRUE;
		}
		else
		{
			if(newDay>=31)
				bAddMonth = TTRUE;
		}
		if(bAddMonth)
		{
			if(m_Month==12)
			{
				m_Month = 1;
				m_Year += 1;
			}
			else
			{
				m_Month += 1;
			}
			m_Day = 1;
		}
		else
		{
			m_Day += 1;
		}
	}

	TVOID DateTime::SubOneDay()
	{
		TS32 newDay = m_Day - 1;
		TBOOL bRun = m_Year % 4 == 0 && m_Year % 100 !=0 || m_Year % 400 ==0; //闰年可以被4或者400整除 但是不能被100整除
		TBOOL bSubMonth = TFALSE;
		if(newDay<=0)
			bSubMonth = TTRUE;
		TS32 newMonth = m_Month;
		TS32 newYear = m_Year;
		if(bSubMonth)
		{
			newMonth -= 1;
			if(newMonth<=0)
			{
				newYear -= 1;
				newMonth = 12;
			}
		}
		if(bSubMonth)
		{
			if(newMonth==2)
			{
				if(bRun)
				{
					m_Day = 29;
				}
				else
				{
					m_Day = 28;
				}
			}
			else if(m_Month==1 || m_Month==3 || m_Month==5 || m_Month==7 || m_Month==8 || m_Month==10 || m_Month==12)
			{
				m_Day = 31;
			}
			else
			{
				m_Day = 30;
			}
			m_Year = newYear;
			m_Month = newMonth;
		}
		else
		{
			m_Day -= 1;
		}
	}

	DateTime::DAY_OF_WEEK DateTime::GetDayOfWeek()
	{
		if ((m_Month == 1) || (m_Month == 2))  
		{  
			m_Month += 12;  
			m_Year--;  
		}  
		TS32 A = (m_Day + 2*m_Month + 3*(m_Month+1)/5 + m_Year + m_Year/4 - m_Year/100 + m_Year/400) % 7;
		return (DateTime::DAY_OF_WEEK)A;
	}

	String DateTime::GetDayOfWeekString()
	{
		TS32 dow = (TS32)GetDayOfWeek();
		switch(dow)
		{
		case 0:
			return String("MON");
		case 1:
			return String("TUE");
		case 2:
			return String("WED");
		case 3:
			return String("THU");
		case 4:
			return String("FRI");
		case 5:
			return String("SET");
		case 6:
			return String("SUN");
		}
		return String("ERROR");
	}

	String DateTime::GetDateString()
	{
		String strRe;
		strRe.Format("%d - %d - %d",m_Year,m_Month,m_Day);
		return strRe;
	}

	TBOOL DateTime::IsEqual(const DateTime& dateTime)
	{
		if(m_Year != dateTime.m_Year)
			return TFALSE;
		if(m_Month != dateTime.m_Month)
			return TFALSE;
		if(m_Day != dateTime.m_Day)
			return TFALSE;
		return TTRUE;
	}

}