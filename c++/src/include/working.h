#include <stdlib.h>
#include "stdio_ex.h"
#include "string_ex.h"
#include "calender_ex.h"
#include "time_ex.h"

#define FALSE -1
#define TRUE 0
#define SUCCESS 0
#define END 1
#define ERROR_P 2
#define CONTINUE 3

#define TARGET_YEAR_MONTH_CHR_LEN 7
#define WORKING_HOUR_PERIOD_CHR_LEN 60

#define DAIRY_LEGAL_WORKING_HOUR_SEC 28800 //sec = 8 Hours
#define WEEKLY_LEGAL_WORKING_HOUR_SEC 144000 //sec = 40 Hours
#define MAX_DAYS 37
#define MAX_BREAK_TIMES 5

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

	time_t workingHours;
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

extern int splitCount;
extern time_t atOpeningTime;
extern time_t atClosingTime;
extern time_t atBeginningOfLateNight;
extern time_t atChangeOfDate;

/**
 * Check the input string means that it is end.
 * @param char *	input		The input string.
 * @return int		If it is true, return 1.
 */
int isEnd(char *);

/**
 * Check the input string means that it is ERROR_P.
 * @param char *	input		The input string.
 * @return int		If it is true, return 1.
 */
int isERROR_P(char *);

/**
 * Initialize the total working hours structure.
 * @param char *		in		The input string.
 * @param WorkHours *	total		The total working hours structure.
 * @return int		The code; 1:end, 2:ERROR_P, 0:success.
 */
int initTotalWorkHours(char *, TotalWorkHours *);

/**
 * Initialize the daily working hours structure.
 * @param char *		in			The input string.
 * @param WorkHours *	daily		The daily working hours structure.
 * @return int		The code; 1:end, 2:ERROR_P, 0:success.
 */
int initDailyWorkHours(char *, DailyWorkHours *);

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

int setWorkingDate(DailyWorkHours *, char *);
time_t getWorkingDate(DailyWorkHours *);


int getWorkingDayNum(DailyWorkHours *);
int getWorkingWeekdayNum(DailyWorkHours *);

void setWeeklyWHOf(DailyWorkHours *, time_t);
time_t getWeeklyWHOf(DailyWorkHours *);

int isEndOnMonth(DailyWorkHours *);

int isWorkingOnSameWeek(DailyWorkHours *, int);

void addWorkingHours(DailyWorkHours *, time_t workinghours);

void updateTotalWorkingHours(TotalWorkHours *, DailyWorkHours *);

time_t checkOvertimeWorking(DailyWorkHours *, time_t );

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
 * Culculate how many hours work overtime.
 * @param WorkHours *	total		The total working hours structure.
 * @param WorkHours *	daily		The daily working hours structure.
 * @return int		The code; 1:end, 2:ERROR_P, 0:success.
 */
int culcWorkHours(int , DailyWorkHours *);

