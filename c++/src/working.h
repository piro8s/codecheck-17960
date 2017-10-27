#include <stdlib.h>
#include "stdio_ex.h"
#include "string_ex.h"
#include "calender_ex.h"
#include "time_ex.h"

#define FAIL -1
#define SUCCESS 0
#define END 1
#define ERROR 2

#define TARGET_YEAR_MONTH_CHR_LEN 7
#define WORKING_HOUR_PERIOD_CHR_LEN 60

#define DAIRY_LEGAL_WORKING_HOUR_SEC 28800 //sec = 8 Hours
#define WEEKLY_LEGAL_WORKING_HOUR_SEC 144000 //sec = 40 Hours
#define MAX_DAYS 37
#define MAX_BREAK_TIMES 3

/**
* The working hours structure.
* @member char *	yearMonth			Target Month: YYYY/MM
* @member char *	yearMonthDay		Working date: YYYY/MM/DD
* @member int 		weekdayNum			Weekday number of today
* @member int 		tmorrowWeekdayNum	Weekday number of tmorrow
* @member char *[]	workPeriod			Array of working hours by period
* @member time_t 	dailyWH				Daily Statutory Working Hours [sec]
* @member time_t 	weeklyWH			Weekly Statutory Working Hours [sec]
* @member time_t 	nomalWH				Normal Working Hours [sec]
* @member time_t 	fixedOWH			Fixed Overtime Working Hours [sec]
* @member time_t 	legalOWH			Legal Overtime Working Hours [sec]
* @member time_t 	midnightOWH			Midnight Overtime Working Hours [sec]
* @member time_t 	nonlegalHolydayWH	Non-Legal Holiday Working Hours [sec]
* @member time_t 	legalHolydayWH		Legal Holiday Working Hours [sec]
*/
typedef struct {
	int yearMonth;		//yyyymm

	time_t nomalWH;
	time_t fixedOWH;
	time_t legalOWH;
	time_t midnightOWH;
	time_t nonlegalHolydayWH;
	time_t legalHolydayWH;
} TotalWorkHours;

typedef struct {
	time_t today;		//新しく追加しようとしている。yearMonthDayはこちらに変更
	int weekdayNum;
	int tmorrowWeekdayNum;
	char *workPeriod[MAX_BREAK_TIMES+1];	//労働年月日・労働時間ピリオド

	time_t dailyWH;
	time_t weeklyWH;

	time_t nomalWH;
	time_t fixedOWH;
	time_t legalOWH;
	time_t midnightOWH;
	time_t nonlegalHolydayWH;
	time_t legalHolydayWH;
} DailyWorkHours;

extern int split_count;
extern time_t atOpeningTime;
extern time_t atClosingTime;
extern time_t atBeginningOfLateNight;
extern time_t atChangeOfDate;

/*
 * Setter and Getter
 */
void setOpeningTime(const time_t, const time_t);
time_t getOpeningTime(void);

void setClosingTime(const time_t, const time_t);
time_t getClosingTime(void);

void setLateNightTime(const time_t, const time_t);
time_t getLateNightTime(void);

void setMidnightTime(const time_t, const time_t);
time_t getMidnightTime(void);

void setSplitCount(const int);
int getSplitCount(void);

void setBegginingOfDay(DailyWorkHours *, const char *);
time_t getBeggingOfday(DailyWorkHours *);

int getWorkingDate(DailyWorkHours *);

int getWorkingWeekdayNum(DailyWorkHours *);

int isEnd(char *);

int isError(char *);

int checkWorkDaily(DailyWorkHours *, int, int);

void addWorkingHours(DailyWorkHours *, time_t workinghours);

/**
 * Get the target date.
 * @return int		The target date.
 */
int targetDate();

/**
 * Return the working hours[sec] of WorkHours.
 * @param WorkHours *	wh		The working hours structure.
 * @return time_t		The working hours[sec] of WorkHours.
 */
time_t weeklyWHOf(DailyWorkHours *);

/**
 * Return the weekday number of WorkHours.
 * @param WorkHours *	wh		The working hours structure.
 * @return int		The weekday number of WorkHours.
 */
int weekdayNumOf(DailyWorkHours *);

/**
 * Check the input string means that it is end.
 * @param char *	input		The input string.
 * @return int		If it is true, return 1.
 */
int isEnd(char *);

/**
 * Check the input string means that it is error.
 * @param char *	input		The input string.
 * @return int		If it is true, return 1.
 */
int isError(char *);


/**
 * Culculate how many hours work overtime.
 * @param WorkHours *	wh		The working hours structure.
 * @param time_t		diff	The diff time[sec] to be added to working hours.
 * @return time_t		The time[sec] how many hours work overtime.
 */
time_t checkOvertimeWorking(DailyWorkHours *, time_t);

/**
 * Culculate how many hours work at 5:00 to 8:00.
 * @param WorkHours *	wh		The working hours structure.
 * @param time_t		s		The time of start[sec] to work.
 * @param time_t		e		The time of end[sec] to work.
 */
void checkMorning(DailyWorkHours *, time_t, time_t);

/**
 * Culculate how many hours work at 8:00 to 16:00.
 * @param WorkHours *	wh		The working hours structure.
 * @param time_t		s		The time of start[sec] to work.
 * @param time_t		e		The time of end[sec] to work.
 */
void checkDaytime(DailyWorkHours *, time_t, time_t);

/**
 * Culculate how many hours work at 16:00 to 22:00.
 * @param WorkHours *	wh		The working hours structure.
 * @param time_t		s		The time of start[sec] to work.
 * @param time_t		e		The time of end[sec] to work.
 */
void checkNight(DailyWorkHours *, time_t, time_t);

/**
 * Culculate how many hours work at 22:00 to 24:00.
 * @param WorkHours *	wh		The working hours structure.
 * @param time_t		s		The time of start[sec] to work.
 * @param time_t		e		The time of end[sec] to work.
 */
void checkLateNight(DailyWorkHours *, time_t, time_t);

/**
 * Culculate how many hours work at 24:00 to 29:00.
 * @param WorkHours *	wh		The working hours structure.
 * @param time_t		s		The time of start[sec] to work.
 * @param time_t		e		The time of end[sec] to work.
 */
void checkMidnight(DailyWorkHours *, time_t, time_t);


/**
 * Initialize the total working hours structure.
 * @param char *		in		The input string.
 * @param WorkHours *	total		The total working hours structure.
 * @return int		The code; 1:end, 2:error, 0:success.
 */
int initTotalWorkHoursStruct(char *, TotalWorkHours *);

/**
 * Initialize the daily working hours structure.
 * @param char *		in			The input string.
 * @param WorkHours *	daily		The daily working hours structure.
 * @return int		The code; 1:end, 2:error, 0:success.
 */
int initDailyWorkHoursStruct(char *, DailyWorkHours *);

/**
 * Culculate how many hours work overtime.
 * @param WorkHours *	total		The total working hours structure.
 * @param WorkHours *	daily		The daily working hours structure.
 * @return int		The code; 1:end, 2:error, 0:success.
 */
int culcWorkHours(int , DailyWorkHours *);

/**
 * Check weekly working hours on today.
 * @param WorkHours *	daily			The daily working hours structure.
 * @param int			lastWorkDay		The date of working last time.
 * @param int			lastWorkWeekday	The weekday number of working last time.
 * @param time_t		temp_weeklyWH	Weekly working hours.
 * @return time_t		Updated weekly working hours.
 */
time_t checkWeeklyWH(DailyWorkHours *, int, int, time_t);
