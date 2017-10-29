// #include<bits/stdc++.h>
#include "include/working.h"
#include "include/timeout.h"


#define WAITING_SEC 3

int main(int argc, char *argv[]) {
	// char *in_ym = (char *)calloc(TARGET_YEAR_MONTH_CHR_LEN+1, sizeof(char));
	TotalWorkHours *total = (TotalWorkHours *)malloc(sizeof(TotalWorkHours));
	int i=0, j=0;

	//Target Year-Month
	// scanf("%7[0-9/]", in_ym);
	// flush();

	char *del; 
	del = (char *)calloc(TARGET_YEAR_MONTH_CHR_LEN, sizeof(char));
	// delch(argv[1], '/', del, strlen(argv[1]));
	for (i=0; i<TARGET_YEAR_MONTH_CHR_LEN; i++) {
		if (argv[1][i] == '/') {
			j++;
			continue;
		}
		del[i-j] = argv[1][i];
	}
	// printf("argv[1]:%s\n", argv[1]);
	// printf("strlen:%d\n", (int)strlen(argv[1]));
	// printf("delch():%d\n", delch(argv[1], '/', del, strlen(argv[1])));
	// printf("delch:%s\n", del);
	// printf("%d\n", atoi(del));
/*
	int initTWH = initTotalWorkHours(argv[1], total);
	// int initTWH = initTotalWorkHours(in_ym, total);
	if (initTWH != SUCCESS) return initTWH;
//*/
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
		else if (initDWH != SUCCESS) return initDWH;
		if (isWorkingOnSameWeek(daily, lastWorkDay) == FALSE) temp_weeklyWH = (time_t)0;
		lastWorkDay = getWorkingDayNum(daily);
		lastWorkWeekday = getWorkingWeekdayNum(daily);
		setWeeklyWHOf(daily, temp_weeklyWH);

		int culcWH = culcWorkHours(total->yearMonth, daily);

		updateTotalWorkingHours(total, daily);

		temp_weeklyWH = getWeeklyWHOf(daily);

		if (culcWH == CONTINUE) continue;
		else if (culcWH == ERROR_P) return 920;

		free(daily);
		// free(in_wh);
		// i++;
	}
//*/

	// printf("%d\n\n", roundSecToHour(total->workingHours));
	// printf("%d\n", roundSecToHour(total->fixedOWH));
	// printf("%d\n", roundSecToHour(total->legalOWH));
	// printf("%d\n", roundSecToHour(total->midnightOWH));
	// printf("%d\n", roundSecToHour(total->nonlegalHolydayWH));
	// printf("%d\n", roundSecToHour(total->legalHolydayWH));

	printf("%d\n", 0);
	printf("%d\n", 0);
	printf("%d\n", 0);
	printf("%d\n", 0);
	printf("%d\n", 0);

	free(total);
	return 0;
}

