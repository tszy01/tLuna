#include "TLDateTime.h"

namespace TLunaEngine{

	DateTime::DateTime(void) : m_Year(0),m_Month(1),m_Day(1)
	{
	}

	DateTime::~DateTime(void)
	{
	}

	DateTime::DateTime(int year, int month, int day) : m_Year(year),m_Month(month),m_Day(day)
	{
	}

	DateTime::DateTime(const DateTime& dateTime) : m_Year(dateTime.m_Year),m_Month(dateTime.m_Month),m_Day(dateTime.m_Day)
	{
	}

	void DateTime::AddDays(int nDays)
	{
		if(nDays==0)
			return;
		else if(nDays>0)
		{
			int count = nDays;
			while(count>0)
			{
				AddOneDay();
				count--;
			}
		}
		else if(nDays<0)
		{
			int count = nDays * -1;
			while(count>0)
			{
				SubOneDay();
				count--;
			}
		}
	}

	void DateTime::AddOneDay()
	{
		int newDay = m_Day + 1;
		bool bRun = m_Year % 4 == 0 && m_Year % 100 !=0 || m_Year % 400 ==0; //闰年可以被4或者400整除 但是不能被100整除
		bool bAddMonth = false;
		if(m_Month==2)
		{
			if(bRun)
			{
				if(newDay>=30)
					bAddMonth = true;
			}
			else
			{
				if(newDay>=29)
					bAddMonth = true;
			}
		}
		else if(m_Month==1 || m_Month==3 || m_Month==5 || m_Month==7 || m_Month==8 || m_Month==10 || m_Month==12)
		{
			if(newDay>=32)
				bAddMonth = true;
		}
		else
		{
			if(newDay>=31)
				bAddMonth = true;
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

	void DateTime::SubOneDay()
	{
		int newDay = m_Day - 1;
		bool bRun = m_Year % 4 == 0 && m_Year % 100 !=0 || m_Year % 400 ==0; //闰年可以被4或者400整除 但是不能被100整除
		bool bSubMonth = false;
		if(newDay<=0)
			bSubMonth = true;
		int newMonth = m_Month;
		int newYear = m_Year;
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
		int A = (m_Day + 2*m_Month + 3*(m_Month+1)/5 + m_Year + m_Year/4 - m_Year/100 + m_Year/400) % 7;
		return (DateTime::DAY_OF_WEEK)A;
	}

	String DateTime::GetDayOfWeekString()
	{
		int dow = (int)GetDayOfWeek();
		switch(dow)
		{
		case 0:
			return String("星期一");
		case 1:
			return String("星期二");
		case 2:
			return String("星期三");
		case 3:
			return String("星期四");
		case 4:
			return String("星期五");
		case 5:
			return String("星期六");
		case 6:
			return String("星期日");
		}
		return String("错误");
	}

	String DateTime::GetDateString()
	{
		String strRe;
		strRe.Format("%d年%d月%d日",m_Year,m_Month,m_Day);
		return strRe;
	}

	bool DateTime::IsEqual(const DateTime& dateTime)
	{
		if(m_Year != dateTime.m_Year)
			return false;
		if(m_Month != dateTime.m_Month)
			return false;
		if(m_Day != dateTime.m_Day)
			return false;
		return true;
	}

}