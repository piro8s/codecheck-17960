// #include<bits/stdc++.h>
#include "include/working.h"


#define WAITING_SEC 3

int main(int argc, char *argv[]) {
	// char *in_ym = (char *)calloc(TARGET_YEAR_MONTH_CHR_LEN+1, sizeof(char));
	TotalWorkHours *total = (TotalWorkHours *)malloc(sizeof(TotalWorkHours));
	int i=0, j=0;

	// for (i=0; i<argc; i++) {
	// 	printf("argv[%d] : %s\n", i, argv[i]);
	// }

	//Target Year-Month
	// scanf("%7[0-9/]", in_ym);
	// flush();
	int initTWH = initTotalWorkHours(argv[1], total);
	// int initTWH = initTotalWorkHours(in_ym, total);
	if (initTWH != SUCCESS) return initTWH;

/*
	// while(1) {
	for (i=2; i<=argc; i++) {
		// char *in_wh = (char *)calloc(WORKING_HOUR_PERIOD_CHR_LEN+1, sizeof(char));
		DailyWorkHours *daily = (DailyWorkHours *)malloc(sizeof(DailyWorkHours));
		static time_t temp_weeklyWH = (time_t)0;
		static int lastWorkDay = 0;
		static int lastWorkWeekday = 7;

		// int flg =scanfWithTimeout("%60[ 0-9/:-]", in_wh, WAITING_SEC);
		// if (in_wh[0] == '0') break;
		// flush();

		int initDWH = initDailyWorkHours(argv[i], daily);
		// int initDWH = initDailyWorkHours(in_wh, daily);
		if (initDWH == END) break;
		// else if (initDWH == ERROR_P) return 101;
		else if (initDWH >= 900) return initDWH;
		if (isWorkingOnSameWeek(daily, lastWorkDay) == FALSE) temp_weeklyWH = (time_t)0;
		lastWorkDay = getWorkingDayNum(daily);
		lastWorkWeekday = getWorkingWeekdayNum(daily);
		setWeeklyWHOf(daily, temp_weeklyWH);

		int culcWH = culcWorkHours(total->yearMonth, daily);

		updateTotalWorkingHours(total, daily);

		temp_weeklyWH = getWeeklyWHOf(daily);

		if (culcWH == CONTINUE) continue;
		else if (culcWH == ERROR_P) return 102;

		free(daily);
		// free(in_wh);
		// i++;
	}
*/

_SHOW:
	// printf("%d\n\n", roundSecToHour(total->workingHours));
	printf("%d\n", roundSecToHour(total->fixedOWH));
	printf("%d\n", roundSecToHour(total->legalOWH));
	printf("%d\n", roundSecToHour(total->midnightOWH));
	printf("%d\n", roundSecToHour(total->nonlegalHolydayWH));
	printf("%d\n", roundSecToHour(total->legalHolydayWH));

	free(total);
	return 0;
}

