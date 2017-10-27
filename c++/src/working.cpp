#include "working.h"

int splitCount;
time_t atOpeningTime;
time_t atClosingTime;
time_t atLateNightTime;
time_t atMidnightTime;

void setOpeningTime(const time_t zero, const time_t open) {
	atOpeningTime = zero + open;
}
time_t getOpeningTime(void) {
	return atOpeningTime;
}

void setClosingTime(const time_t zero, const time_t close) {
	atClosingTime = zero + close;
}
time_t getClosingTime(void) {
	return atClosingTime;
}

void setLateNightTime(const time_t zero, const time_t latenight) {
	atLateNightTime = zero + latenight;
}
time_t getLateNightTime(void) {
	return atLateNightTime;
}

void setMidnightTime(const time_t zero, const time_t midnight) {
	atMidnightTime = zero + midnight;
}
time_t getMidnightTime(void) {
	return atMidnightTime;
}

void setSplitCount(const int cnt) {
	splitCount = cnt;
}
int getSplitCount(void) {
	return splitCount;
}

int setBeggingOfday(DailyWorkHours *daily, const char *strYMD) {
	struct tm tm_struct;
	char *ymd[3], ymd_temp[strlen(strYMD)+1];

	strcpy(ymd_temp, strYMD);
	split(ymd_temp, '/', ymd);

	tm_struct.tm_year = atoi(ymd[0]) - 1900;
	tm_struct.tm_mon = atoi(ymd[1]) - 1;
	tm_struct.tm_mday = atoi(ymd[2]);
	tm_struct.tm_hour = 0;
	tm_struct.tm_min = 0;

	if ((daily->today = mktime(&tm_struct)) == (time_t)-1)
		return ERROR;
	return SUCCESS;
}
time_t getBeggingOfday(DailyWorkHours *daily) {
	return daily->today;
}

int getWorkingDate(DailyWorkHours *daily) {
	return getDate(daily->today);
}

int getWorkingWeekdayNum(DailyWorkHours *daily) {
	return daily->weekdayNum;
}

int isEnd(char *input) {
	if (input[0] == '0') return END;
	return SUCCESS;
}

int isError(char *input) {
	if (input[0] == '1') return ERROR;
	return SUCCESS;
}

int checkWorkDaily(DailyWorkHours *daily, int lastWorkDay, int lastWorkWeekday) {
	if ((getWorkingDate(daily) > lastWorkDay) && (getWorkingWeekdayNum(daily) < lastWorkWeekday)) return FAIL;
	return SUCCESS;
}

void addWorkingHours(DailyWorkHours *daily, time_t workinghours) {
	daily->dailyWH += workinghours;
	daily->weeklyWH += workinghours;
}

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

int initTotalWorkHours(char *in, TotalWorkHours *total) {
	if (isEnd(in)==END || isError(in)==ERROR) return END;
	char *del;
	del = (char *)malloc((strlen(in)-searchc(in, '/'))*sizeof(char));
	if (delch(in, '/', del) == 0) return ERROR;

	total->yearMonth = atoi(del);
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
	else if (isError(in)==ERROR) return ERROR;

	split_count = split(in, ' ', splited);
	if (split_count < 1) return 2;
	for (i=0; i<split_count; i++) {
		daily->workPeriod[i] = splited[i+1];
	}

	if (setBeggingOfday(daily, splited[0]) == ERROR) return ERROR;
	setOpeningTime(getBeggingOfday(daily), EIGHT_HOUR_SEC);
	setClosingTime(getBeggingOfday(daily), SIXTEEN_HOUR_SEC);
	setLateNightTime(getBeggingOfday(daily), TWENTY_TWO_HOUR_SEC);
	setMidnightTime(getBeggingOfday(daily), TWENTY_FOUR_HOUR_SEC);

	daily->weekdayNum = subZeller(getBeggingOfday(daily));
	daily->tmorrowWeekdayNum = subZeller(getBeggingOfday(daily) + ONE_DAY_SEC);

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

int culcWorkHours(int targetYearMonth, DailyWorkHours *daily) {
// int culcWorkHours(WorkHours *total, WorkHours *daily) {
	time_t start_tm, end_tm;
	struct tm tm_struct;
	char *sewt[2], *shm[2], *ehm[2];
	int hour, minute, dayStride;
	int j;
	for (j=0; j<split_count; j++) {
		// 時刻を Time型 に変換する
		char *period = (char *)malloc(12*sizeof(char));
		strcpy(period, daily->workPeriod[j]);
		split(period, '-', sewt);
		char *start = (char *)malloc(6*sizeof(char));
		char *end = (char *)malloc(6*sizeof(char));
		strcpy(start, sewt[0]);
		strcpy(end, sewt[1]);
		split(start, ':', shm);
		split(end, ':', ehm);

		//start time
		hour = atoi(shm[0]);
		minute = atoi(shm[1]);
		dayStride = 0;
		if (hour > 23) {
			dayStride = 1;
			hour -= 24;
		}
		tm_struct.tm_mday += dayStride;
		tm_struct.tm_hour = hour;
		tm_struct.tm_min = minute;
		if ((start_tm = mktime(&tm_struct)) == (time_t)-1)
			return ERROR;

		//end time
		hour = atoi(ehm[0]);
		minute = atoi(ehm[1]);
		dayStride = 0;
		if (hour > 23) {
			dayStride = 1;
			hour -= 24;
		}
		tm_struct.tm_mday += dayStride;
		tm_struct.tm_hour = hour;
		tm_struct.tm_min = minute;
		if ((end_tm = mktime(&tm_struct)) == (time_t)-1)
			return ERROR;

		// 労働時間を計算する
		// target year-month と daily->yearMonthDayのyear-month が一致していない場合は労働時間だけ計算する
		if (targetYearMonth != getYearMonth(daily->today)) {
			addWorkingHours(daily, difftime(end_tm, start_tm));
			return END;
		}
		// 労働時間数を計算する
		if(start_tm < atOpeningTime) {
			log("DEBUG", "start < atOpeningTime, ");
			if (end_tm <= atOpeningTime) {
				log("DEBUG", "end <= atOpeningTime\n");//Moring(s-e)
				checkMorning(daily, start_tm, end_tm);
			}
			else if (end_tm <= atClosingTime) {
				log("DEBUG", "atOpeningTime < end <= atClosingTime\n");//Morning(s-8)/Daytime(8-e)
				checkMorning(daily, start_tm, atOpeningTime);
				checkDaytime(daily, atOpeningTime, end_tm);
			}
			else if (end_tm <= atBeginningOfLateNight) {
				log("DEBUG", "atClosingTime < end <= atBeginningOfLateNight \n");//Morning(s-8)/Daytime(8-16)/Night(16-e)
				checkMorning(daily, start_tm, atOpeningTime);
				checkDaytime(daily, atOpeningTime, atClosingTime);
				checkNight(daily, atClosingTime, end_tm);
			}
			else if (end_tm <= atChangeOfDate) {
				log("DEBUG", "atBeginningOfLateNight < end \n");//Morning(s-8)/Daytime(8-16)/Night(16-22)/LateNight(22-e)
				checkMorning(daily, start_tm, atOpeningTime);
				checkDaytime(daily, atOpeningTime, atClosingTime);
				checkNight(daily, atClosingTime, atBeginningOfLateNight);
				checkLateNight(daily, atBeginningOfLateNight, end_tm);
			}
			else if (atChangeOfDate < end_tm) {
				log("DEBUG", "atChangeOfDate < end \n");//Morning(s-8)/Daytime(8-16)/Night(16-22)/LateNight(22-24)/Midnight(24-e)
				checkMorning(daily, start_tm, atOpeningTime);
				checkDaytime(daily, atOpeningTime, atClosingTime);
				checkNight(daily, atClosingTime, atBeginningOfLateNight);
				checkLateNight(daily, atBeginningOfLateNight, atChangeOfDate);
				checkMidnight(daily, atChangeOfDate, end_tm);
			}
		}
		else if (atOpeningTime <= start_tm && start_tm < atClosingTime) {
			log("DEBUG", "atOpeningTime <= start < atClosingTime, ");
			if (end_tm <= atClosingTime) {
				log("DEBUG", "end <= atClosingTime\n");//Daytime(s-e)
				checkDaytime(daily, start_tm, end_tm);
			}
			else if (end_tm <= atBeginningOfLateNight) {
				log("DEBUG", "atClosingTime < end <= atBeginningOfLateNight \n");//Daytime(s-16)/Night(16-e)
				checkDaytime(daily, start_tm, atClosingTime);
				checkNight(daily, atClosingTime, end_tm);
			}
			else if (end_tm <= atChangeOfDate) {
				log("DEBUG", "atBeginningOfLateNight < end \n");//Daytime(s-16)/Night(16-22)/LateNight(22-e)
				checkDaytime(daily, start_tm, atClosingTime);
				checkNight(daily, atClosingTime, atBeginningOfLateNight);
				checkLateNight(daily, atBeginningOfLateNight, end_tm);
			}
			else if (atChangeOfDate < end_tm) {
				log("DEBUG", "atChangeOfDate < end \n");//Daytime(s-16)/Night(16-22)/LateNight(22-24)/Midnight(24-e)
				checkDaytime(daily, start_tm, atClosingTime);
				checkNight(daily, atClosingTime, atBeginningOfLateNight);
				checkLateNight(daily, atBeginningOfLateNight, atChangeOfDate);
				checkMidnight(daily, atChangeOfDate, end_tm);
			}
		}
		else if (atClosingTime <= start_tm && start_tm < atBeginningOfLateNight) {
			log("DEBUG", "atClosingTime <= start < atBeginningOfLateNight, ");
			if (end_tm <= atBeginningOfLateNight) {
				log("DEBUG", "end <= atBeginningOfLateNight \n");//Night(s-e)
				checkNight(daily, start_tm, end_tm);
			}
			else if (end_tm <= atChangeOfDate) {
				log("DEBUG", "atBeginningOfLateNight < end \n");//Night(s-22)/LateNight(22-e)
				checkNight(daily, start_tm, atBeginningOfLateNight);
				checkLateNight(daily, atBeginningOfLateNight, end_tm);
			}
			else if (atChangeOfDate < end_tm) {
				log("DEBUG", "atChangeOfDate < end \n");//Night(s-22)/LateNight(22-24)/Midnight(24-e)
				checkNight(daily, start_tm, atBeginningOfLateNight);
				checkLateNight(daily, atBeginningOfLateNight, atChangeOfDate);
				checkMidnight(daily, atChangeOfDate, end_tm);
			}
		}
		else if (atBeginningOfLateNight <= start_tm && start_tm < atChangeOfDate) {
			log("DEBUG", "atBeginningOfLateNight <= start_tm < atChangeOfDate, ");
			if (end_tm <= atChangeOfDate) {
				log("DEBUG", "atBeginningOfLateNight < end <= atChangeOfDate\n");//LateNight(s-e)
				checkLateNight(daily, start_tm, end_tm);
			}
			else if (atChangeOfDate < end_tm) {
				log("DEBUG", "atChangeOfDate < end \n");//LateNight(s-24)/Midnight(24-e)
				checkLateNight(daily, start_tm, atChangeOfDate);
				checkMidnight(daily, atChangeOfDate, end_tm);
			}
		}
		else if (atChangeOfDate <= start_tm) {
			log("DEBUG", "atChangeOfDate < start\n");//Midnight(s-e)
			checkMidnight(daily, start_tm, end_tm);
		}
		free(period);
		free(start);
		free(end);
	}
	return SUCCESS;
}