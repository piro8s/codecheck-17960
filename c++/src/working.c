#include "include/working.h"

int splitCount;
time_t atOpeningTime;
time_t atClosingTime;
time_t atLateNightTime;
time_t atMidnightTime;

/* 入力判定 */
int isEnd(char *input) {
	if (input[0] == '0') return END;
	return SUCCESS;
}
int isERROR_P(char *input) {
	if (input[0] == '1') return ERROR_P;
	return SUCCESS;
}

int initTotalWorkHours(char *in, TotalWorkHours *total) {
	char *del;
	del = (char *)calloc(TARGET_YEAR_MONTH_CHR_LEN, sizeof(char));
	if (delch(in, '/', del, TARGET_YEAR_MONTH_CHR_LEN) != 1) return 900;;

	total->yearMonth = atoi(del);
	total->workingHours = (time_t)0;
	total->nomalWH = (time_t)0;
	total->fixedOWH = (time_t)0;
	total->legalOWH = (time_t)0;
	total->midnightOWH = (time_t)0;
	total->nonlegalHolydayWH = (time_t)0;
	total->legalHolydayWH = (time_t)0;

	free(del);
	return SUCCESS;
}

int initDailyWorkHours(char *in, DailyWorkHours *daily) {

	char *splited[MAX_BREAK_TIMES+3];
	int i;

	if (isEnd(in)==END) return END;

	splitCount = split(in, ' ', splited, WORKING_HOUR_PERIOD_CHR_LEN);
	if (splitCount < 1) return 910;
	for (i=0; i<splitCount; i++) {
		daily->workPeriod[i] = splited[i+1];
	}

	if (setWorkingDate(daily, splited[0]) == ERROR_P) return 911;
	setOpeningTime(getWorkingDate(daily), EIGHT_HOUR_SEC);
	setClosingTime(getWorkingDate(daily), SIXTEEN_HOUR_SEC);
	setLateNightTime(getWorkingDate(daily), TWENTY_TWO_HOUR_SEC);
	setMidnightTime(getWorkingDate(daily), TWENTY_FOUR_HOUR_SEC);
	
	daily->weekdayNum = subZeller(getWorkingDate(daily));
	daily->tmorrowWeekdayNum = subZeller(getWorkingDate(daily) + ONE_DAY_SEC);

	daily->nomalWH = (time_t)0;
	daily->fixedOWH = (time_t)0;
	daily->legalOWH = (time_t)0;
	daily->midnightOWH = (time_t)0;
	daily->nonlegalHolydayWH = (time_t)0;
	daily->legalHolydayWH = (time_t)0;
	daily->dailyWH = (time_t)0;
	daily->weeklyWH = (time_t)0;

	return SUCCESS;
}

/* Opening Time */
void setOpeningTime(const time_t zero, const time_t open) {
	atOpeningTime = zero + open;
}
time_t getOpeningTime(void) {
	return atOpeningTime;
}

/* Closing Time */
void setClosingTime(const time_t zero, const time_t close) {
	atClosingTime = zero + close;
}
time_t getClosingTime(void) {
	return atClosingTime;
}

/* Start of Late Night Time */
void setLateNightTime(const time_t zero, const time_t latenight) {
	atLateNightTime = zero + latenight;
}
time_t getLateNightTime(void) {
	return atLateNightTime;
}

/* Start of Midnight Time */
void setMidnightTime(const time_t zero, const time_t midnight) {
	atMidnightTime = zero + midnight;
}
time_t getMidnightTime(void) {
	return atMidnightTime;
}

/* Split Count */
void setSplitCount(const int cnt) {
	splitCount = cnt;
}
int getSplitCount(void) {
	return splitCount;
}

/* Working Date */
int setWorkingDate(DailyWorkHours *daily, const char *strYMD) {
	struct tm tm_struct;
	char *ymd[3], ymd_temp[(int)strlen(strYMD)+1];

	strcpy(ymd_temp, strYMD);
	split(ymd_temp, '/', ymd, WORKING_HOUR_PERIOD_CHR_LEN);

	tm_struct.tm_year = atoi(ymd[0]) - 1900;
	tm_struct.tm_mon = atoi(ymd[1]) - 1;
	tm_struct.tm_mday = atoi(ymd[2]);
	tm_struct.tm_hour = 0;
	tm_struct.tm_min = 0;
	tm_struct.tm_sec = 0;
	tm_struct.tm_isdst = -1;

	time_t today = mktime(&tm_struct);
	if (today == (time_t)(-1)) return ERROR_P;
	daily->today = today;
	return SUCCESS;
}
time_t getWorkingDate(DailyWorkHours *daily) {
	return daily->today;
}

/* Weekly Working Hours */
void setWeeklyWHOf(DailyWorkHours *daily, time_t t) {
	daily->weeklyWH = t;
}
time_t getWeeklyWHOf(DailyWorkHours *daily) {
	return daily->weeklyWH;
}

/* getter: Working Day Number */
int getWorkingDayNum(DailyWorkHours *daily) {
	return getDate(daily->today);
}
/* getter: Working Weekday Number */
int getWorkingWeekdayNum(DailyWorkHours *daily) {
	return daily->weekdayNum;
}

int isEndOnMonth(DailyWorkHours *daily) {
	//月末美取得
	time_t eom = getEndOfMonth(daily->today);
	//月末の曜日番号取得(日-土:0-6)
	int wnum = subZeller(eom);
	//0,6の場合、-1dayして再度曜日番号取得
	//1-5になるまで繰り返す
	struct tm *t_tm;
	while (wnum < 1 || 5 < wnum) {
		t_tm = localtime(&eom);
		t_tm->tm_mday -= 1;
		eom = mktime(t_tm);
		wnum = subZeller(eom);
	}
	//入力が月末(営業日)なら
	if (getDate(daily->today) == getDate(eom)) return END;
}

/* 同一週(月〜日)中の仕事か判定 */
int isWorkingOnSameWeek(DailyWorkHours *daily, int lastWorkDay) {

	struct tm *last_tm;
	time_t last;
	last_tm = localtime(&(daily->today));
	last_tm->tm_mday = lastWorkDay;
	last = mktime(last_tm);

	if ((lastWorkDay < getWorkingDayNum(daily)) 
		&& (localtime(&last)->tm_wday < localtime(&(daily->today))->tm_wday)) return TRUE;
	return FALSE;
}

/* 労働した時間を、日次、週次の労働時間に追加 */
void addWorkingHours(DailyWorkHours *daily, time_t workinghours) {
	daily->dailyWH += workinghours;
	daily->weeklyWH += workinghours;
}

/* 日次の労働時間等から月次の労働時間等を更新する */
void updateTotalWorkingHours(TotalWorkHours *total, DailyWorkHours *daily) {

	total->workingHours += daily->dailyWH;
	total->nomalWH += daily->nomalWH;
	total->fixedOWH += daily->fixedOWH;
	total->legalOWH += daily->legalOWH;
	total->midnightOWH += daily->midnightOWH;
	total->nonlegalHolydayWH += daily->nonlegalHolydayWH;
	total->legalHolydayWH += daily->legalHolydayWH;
}

/* 残業していないか判定 */
time_t checkOvertimeWorking(DailyWorkHours *daily, time_t diff) {
	time_t overtime = (time_t)0;

	//Already work overtime
	if (daily->dailyWH > DAIRY_LEGAL_WORKING_HOUR_SEC || daily->weeklyWH > WEEKLY_LEGAL_WORKING_HOUR_SEC) {
		overtime = diff;
	}
	//Work overtime on this period. 
	else if ((daily->dailyWH+diff) > DAIRY_LEGAL_WORKING_HOUR_SEC || (daily->weeklyWH+diff) > WEEKLY_LEGAL_WORKING_HOUR_SEC) {
		overtime = (daily->dailyWH + diff - DAIRY_LEGAL_WORKING_HOUR_SEC) < (daily->weeklyWH + diff - WEEKLY_LEGAL_WORKING_HOUR_SEC) ? (daily->weeklyWH + diff - WEEKLY_LEGAL_WORKING_HOUR_SEC) : (daily->dailyWH + diff - DAIRY_LEGAL_WORKING_HOUR_SEC);
	}

	return overtime;
}

//5-8{法定内/法定外}/{所定休日/法定休日}
void checkMorning(DailyWorkHours *daily, time_t s, time_t e) {
	int wd = daily->weekdayNum;
	time_t diff = difftime(e, s);
	time_t overtime;

	switch(wd) {
		case 0:
			daily->legalHolydayWH += diff;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			overtime = checkOvertimeWorking(daily, diff);
			daily->fixedOWH += diff - overtime;
			daily->legalOWH += overtime;
			break;
		case 6:
			daily->nonlegalHolydayWH += diff;
			break;
	}
	addWorkingHours(daily, diff);
}

//8-16{所定/法定外}/{所定休日/法定休日}
void checkDaytime(DailyWorkHours *daily, time_t s, time_t e) {
	int wd = daily->weekdayNum;
	time_t diff = difftime(e, s);
	time_t overtime;

	switch(wd) {
		case 0:
			daily->legalHolydayWH += diff;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			overtime = checkOvertimeWorking(daily, diff);
			daily->nomalWH += diff - overtime;
			daily->legalOWH += overtime;
			break;
		case 6:
			daily->nonlegalHolydayWH += diff;
			break;
	}
	addWorkingHours(daily, diff);
}

//17-22{法定内/法定外}/{所定休日/法定休日}
void checkNight(DailyWorkHours *daily, time_t s, time_t e) {
	int wd = daily->weekdayNum;
	time_t diff = difftime(e, s);
	time_t overtime;

	switch(wd) {
		case 0:
			daily->legalHolydayWH += diff;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			overtime = checkOvertimeWorking(daily, diff);
			daily->fixedOWH += diff - overtime;
			daily->legalOWH += overtime;
			break;
		case 6:
			daily->nonlegalHolydayWH += diff;
			break;
	}
	addWorkingHours(daily, diff);
}

//22-24{法定内/法定外}/{所定休日/法定休日} +{深夜}
void checkLateNight(DailyWorkHours *daily, time_t s, time_t e) {
	int wd = daily->weekdayNum;
	time_t diff = difftime(e, s);
	time_t overtime;

	daily->midnightOWH += diff;
	switch(wd) {
		case 0:
			daily->legalHolydayWH += diff;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			overtime = checkOvertimeWorking(daily, diff);
			daily->fixedOWH += diff - overtime;
			daily->legalOWH += overtime;
			break;
		case 6:
			daily->nonlegalHolydayWH += diff;
			break;
	}
	addWorkingHours(daily, diff);
}

//24-29<<翌日判定>>{法定内/法定外}/{所定休日/法定休日} +{深夜}
void checkMidnight(DailyWorkHours *daily, time_t s, time_t e) {
	int wd = daily->tmorrowWeekdayNum;
	time_t diff = difftime(e, s);
	time_t overtime;

	daily->midnightOWH += diff;
	switch(wd) {
		case 0:
			daily->legalHolydayWH += diff;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			overtime = checkOvertimeWorking(daily, diff);
			daily->fixedOWH += diff - overtime;
			daily->legalOWH += overtime;
			break;
		case 6:
			daily->nonlegalHolydayWH += diff;
			break;
	}
	addWorkingHours(daily, diff);
}

int culcWorkHours(int targetYearMonth, DailyWorkHours *daily) {
	time_t start_tm, end_tm;
	struct tm *tm_struct;
	char *sewt[2], *shm[2], *ehm[2];
	int hour, minute, dayStride;
	int i;
	tm_struct = localtime(&(daily->today));
	for (i=0; i<getSplitCount(); i++) {
		// 時刻を Time型 に変換する
		char *period = (char *)calloc(12, sizeof(char));
		char *start = (char *)calloc(6, sizeof(char));
		char *end = (char *)calloc(6, sizeof(char));

		// "HH:mm-HH:mm" --> "HH:mm", "HH:mm"
		strcpy(period, daily->workPeriod[i]);
		split(period, '-', sewt, 12);

		// "HH:mm" --> "HH", "mm"
		strcpy(start, sewt[0]);
		split(start, ':', shm, 6);

		// "HH:mm" --> "HH", "mm"
		strcpy(end, sewt[1]);
		split(end, ':', ehm, 6);

		//start time
		hour = atoi(shm[0]);
		minute = atoi(shm[1]);
		dayStride = 0;
		if (hour > 23) {
			dayStride = 1;
			hour -= 24;
		}
		tm_struct->tm_mday += dayStride;
		tm_struct->tm_hour = hour;
		tm_struct->tm_min = minute;
		if ((start_tm = mktime(tm_struct)) == (time_t)-1)
			return ERROR_P;

		//end time
		hour = atoi(ehm[0]);
		minute = atoi(ehm[1]);
		dayStride = 0;
		if (hour > 23) {
			dayStride = 1;
			hour -= 24;
		}
		tm_struct->tm_mday += dayStride;
		tm_struct->tm_hour = hour;
		tm_struct->tm_min = minute;
		if ((end_tm = mktime(tm_struct)) == (time_t)-1)
			return ERROR_P;

		// 労働時間を計算する
		// target year-month と daily->yearMonthDayのyear-month が一致していない場合は労働時間だけ計算する
		if (targetYearMonth != getYearMonth(daily->today)) {
			addWorkingHours(daily, difftime(end_tm, start_tm));
			return CONTINUE;
		}
		// 労働時間数を計算する
		if(start_tm < getOpeningTime()) {
			//Log("DEBUG", "start < atOpeningTime, ");
			if (end_tm <= getOpeningTime()) {
				//Log("DEBUG", "end <= atOpeningTime\n");//Moring(s-e)
				checkMorning(daily, start_tm, end_tm);
			}
			else if (end_tm <= getClosingTime()) {
				//Log("DEBUG", "atOpeningTime < end <= atClosingTime\n");//Morning(s-8)/Daytime(8-e)
				checkMorning(daily, start_tm, getOpeningTime());
				checkDaytime(daily, getOpeningTime(), end_tm);
			}
			else if (end_tm <= getLateNightTime()) {
				//Log("DEBUG", "atClosingTime < end <= atBeginningOfLateNight \n");//Morning(s-8)/Daytime(8-16)/Night(16-e)
				checkMorning(daily, start_tm, getOpeningTime());
				checkDaytime(daily, getOpeningTime(), getClosingTime());
				checkNight(daily, getClosingTime(), end_tm);
			}
			else if (end_tm <= getMidnightTime()) {
				//Log("DEBUG", "atBeginningOfLateNight < end \n");//Morning(s-8)/Daytime(8-16)/Night(16-22)/LateNight(22-e)
				checkMorning(daily, start_tm, getOpeningTime());
				checkDaytime(daily, getOpeningTime(), getClosingTime());
				checkNight(daily, getClosingTime(), getLateNightTime());
				checkLateNight(daily, getLateNightTime(), end_tm);
			}
			else if (getMidnightTime() < end_tm) {
				//Log("DEBUG", "atChangeOfDate < end \n");//Morning(s-8)/Daytime(8-16)/Night(16-22)/LateNight(22-24)/Midnight(24-e)
				checkMorning(daily, start_tm, getOpeningTime());
				checkDaytime(daily, getOpeningTime(), getClosingTime());
				checkNight(daily, getClosingTime(), getLateNightTime());
				checkLateNight(daily, getLateNightTime(), getMidnightTime());
				checkMidnight(daily, getMidnightTime(), end_tm);
			}
		}
		else if (getOpeningTime() <= start_tm && start_tm < getClosingTime()) {
			//Log("DEBUG", "atOpeningTime <= start < atClosingTime, ");
			if (end_tm <= getClosingTime()) {
				//Log("DEBUG", "end <= atClosingTime\n");//Daytime(s-e)
				checkDaytime(daily, start_tm, end_tm);
			}
			else if (end_tm <= getLateNightTime()) {
				//Log("DEBUG", "atClosingTime < end <= atBeginningOfLateNight \n");//Daytime(s-16)/Night(16-e)
				checkDaytime(daily, start_tm, getClosingTime());
				checkNight(daily, getClosingTime(), end_tm);
			}
			else if (end_tm <= getMidnightTime()) {
				//Log("DEBUG", "atBeginningOfLateNight < end \n");//Daytime(s-16)/Night(16-22)/LateNight(22-e)
				checkDaytime(daily, start_tm, getClosingTime());
				checkNight(daily, getClosingTime(), getLateNightTime());
				checkLateNight(daily, getLateNightTime(), end_tm);
			}
			else if (getMidnightTime() < end_tm) {
				//Log("DEBUG", "atChangeOfDate < end \n");//Daytime(s-16)/Night(16-22)/LateNight(22-24)/Midnight(24-e)
				checkDaytime(daily, start_tm, getClosingTime());
				checkNight(daily, getClosingTime(), getLateNightTime());
				checkLateNight(daily, getLateNightTime(), getMidnightTime());
				checkMidnight(daily, getMidnightTime(), end_tm);
			}
		}
		else if (getClosingTime() <= start_tm && start_tm < getLateNightTime()) {
			//Log("DEBUG", "atClosingTime <= start < atBeginningOfLateNight, ");
			if (end_tm <= getLateNightTime()) {
				//Log("DEBUG", "end <= atBeginningOfLateNight \n");//Night(s-e)
				checkNight(daily, start_tm, end_tm);
			}
			else if (end_tm <= getMidnightTime()) {
				//Log("DEBUG", "atBeginningOfLateNight < end \n");//Night(s-22)/LateNight(22-e)
				checkNight(daily, start_tm, getLateNightTime());
				checkLateNight(daily, getLateNightTime(), end_tm);
			}
			else if (getMidnightTime() < end_tm) {
				//Log("DEBUG", "atChangeOfDate < end \n");//Night(s-22)/LateNight(22-24)/Midnight(24-e)
				checkNight(daily, start_tm, getLateNightTime());
				checkLateNight(daily, getLateNightTime(), getMidnightTime());
				checkMidnight(daily, getMidnightTime(), end_tm);
			}
		}
		else if (getLateNightTime() <= start_tm && start_tm < getMidnightTime()) {
			//Log("DEBUG", "atBeginningOfLateNight <= start_tm < atChangeOfDate, ");
			if (end_tm <= getMidnightTime()) {
				//Log("DEBUG", "atBeginningOfLateNight < end <= atChangeOfDate\n");//LateNight(s-e)
				checkLateNight(daily, start_tm, end_tm);
			}
			else if (getMidnightTime() < end_tm) {
				//Log("DEBUG", "atChangeOfDate < end \n");//LateNight(s-24)/Midnight(24-e)
				checkLateNight(daily, start_tm, getMidnightTime());
				checkMidnight(daily, getMidnightTime(), end_tm);
			}
		}
		else if (getMidnightTime() <= start_tm) {
			//Log("DEBUG", "atChangeOfDate < start\n");//Midnight(s-e)
			checkMidnight(daily, start_tm, end_tm);
		}
		free(period);
		free(start);
		free(end);
	}
	return SUCCESS;
}