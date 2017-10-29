// #include<bits/stdc++.h>
#include "include/working.h"
#include "include/timeout.h"


#define WAIT_TIME 150 // u sec

int main(int argc, char *argv[]) {
	char *in_ym = (char *)calloc(TARGET_YEAR_MONTH_CHR_LEN+1, sizeof(char));
	TotalWorkHours *total = (TotalWorkHours *)malloc(sizeof(TotalWorkHours));

	//Target Year-Month
	scanf("%7[0-9/]", in_ym);
	flush();

	char *del; 
	del = (char *)calloc(TARGET_YEAR_MONTH_CHR_LEN+1, sizeof(char));
	delch(in_ym, '/', del, TARGET_YEAR_MONTH_CHR_LEN);
//*
	int initTWH = initTotalWorkHours(in_ym, total);
	if (initTWH != SUCCESS) return initTWH;
//*/
//*
	while(1) {
		char *in_wh = (char *)calloc(WORKING_HOUR_PERIOD_CHR_LEN+1, sizeof(char));
		DailyWorkHours *daily = (DailyWorkHours *)malloc(sizeof(DailyWorkHours));
		static time_t temp_weeklyWH = (time_t)0;
		static int lastWorkDay = 0;

		scanfWithTimeout("%60[ 0-9/:-]", in_wh, WAIT_TIME);
		if (in_wh[0] == '0') break;
		flush();

		int initDWH = initDailyWorkHours(in_wh, daily);

		if (initDWH == END) break;
		else if (initDWH != SUCCESS) return initDWH;
		if (isWorkingOnSameWeek(daily, lastWorkDay) == FALSE) temp_weeklyWH = (time_t)0;
		lastWorkDay = getWorkingDayNum(daily);
		setWeeklyWHOf(daily, temp_weeklyWH);

		int culcWH = culcWorkHours(total->yearMonth, daily);

		updateTotalWorkingHours(total, daily);

		temp_weeklyWH = getWeeklyWHOf(daily);

		if (culcWH == CONTINUE) continue;
		else if (culcWH == ERROR_P) return 920;

		free(daily);
		free(in_wh);
	}

//*/

	// printf("%d\n\n", roundSecToHour(total->workingHours));
	printf("%d\n", roundSecToHour(total->fixedOWH));
	printf("%d\n", roundSecToHour(total->legalOWH));
	printf("%d\n", roundSecToHour(total->midnightOWH));
	printf("%d\n", roundSecToHour(total->nonlegalHolydayWH));
	printf("%d\n", roundSecToHour(total->legalHolydayWH));

	free(total);
	free(in_ym);
	return 0;
}

