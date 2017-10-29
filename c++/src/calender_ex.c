#include "include/calender_ex.h"

int getDate(time_t t) {
	return (localtime(&t))->tm_mday;
}

int getMonth(time_t t) {
	return ((localtime(&t))->tm_mon) +1;
}

int getYear(time_t t) {
	return ((localtime(&t))->tm_year) + 1900;
}

int getYearMonth(time_t t) {
	return ((((localtime(&t))->tm_year)+1900)*100 + ((localtime(&t))->tm_mon)+1);
}

int subZeller(time_t t) {
	return isNationalHoloday(t);
}

int isNationalHoloday(time_t t) {
	struct tm *tm;
	tm = localtime(&t);
	int y = tm->tm_year + 1900;
	int m = tm->tm_mon + 1;
	int d = tm->tm_mday; 
	int wd = tm->tm_wday;

	/* Vernal Equinox Day */
	int vernalED = (20.8431 + 0.242194*(y - 1980) - (y - 1980)/4);
	/* Autumnal Equinox Day */
	int autumnalED = (23.2488 + 0.242194*(y - 1980) - (y - 1980)/4);

	//In the case, it is a Substitute Holiday on Monday.
	if ((m == 1 && d == 2)
		|| (m == 2 && d == 12)
		|| (m == 3 && d == vernalED+1)
		|| (m == 4 && d == 30)
		|| (m == 8 && d == 12)
		|| (m == 9 && d == autumnalED+1)
		|| (m == 11 && d == 3)
		|| (m == 11 && d == 24)
		|| (m == 12 && d == 24)) {
		if (wd == 1) return 6;
	}
	/* Happy Monday Rule of Japan */
	if (((m == 1 || m == 10) && ((double)d/7.0 > 1.0 && (double)d/7.0 <= 2.0))
		|| ((m == 7 || m == 9) && ((double)d/7.0 > 2.0 && (double)d/7.0 <= 3.0))) {
		if (wd == 1) return 6;
		
	}
	/* Golden Week */
	if ((m == 5 && d == 6) && (wd >= 1 && wd <= 3)) return 6;

	/*In the case, weekdays will be a national holiday.
	* According to the Japanese Labor Standards Law, 
	*  transfer holidays of national holidays are treated as non-legal holidays. */
	if ((m == 1 && d == 1)
		|| (m == 2 && d == 11)
		|| (m == 3 && d == vernalED)
		|| (m == 4 && d == 29)
		|| (m == 5 && d == 3)
		|| (m == 5 && d == 4)
		|| (m == 5 && d == 5)
		|| (m == 8 && d == 11)
		|| (m == 9 && d == autumnalED)
		|| (m == 11 && d == 3)
		|| (m == 11 && d == 23)
		|| (m == 12 && d == 23)) {
		if (wd >= 1 && wd <= 5) return 0;
	}

	return wd;
}

int IsLeapYear(int year) {
	if (((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0) 
		return 0;
	else
		return -1;
}

time_t getEndOfMonth(time_t t) {
	static int lmdays[] = {31,29,31,30,31,30,31,31,30,31,30,31};
	static int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};

	int leap = IsLeapYear(getYear(t));
	struct tm *t_tm;
	t_tm = localtime(&t);
	/* Leap Year */
	if (leap == 0) t_tm->tm_mday = lmdays[getMonth(t) - 1];
	/* Not Leap Year */
	else t_tm->tm_mday = mdays[getMonth(t) - 1];
	
	return mktime(t_tm);
}