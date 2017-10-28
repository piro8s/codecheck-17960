// #include<bits/stdc++.h>
#include "working.h"

int main() {
	char *in_ym = (char *)malloc((TARGET_YEAR_MONTH_CHR_LEN+1)*sizeof(char));
	char *in_wh = (char *)malloc((WORKING_HOUR_PERIOD_CHR_LEN+1)*sizeof(char));
	TotalWorkHours *total = (TotalWorkHours *)malloc(sizeof(TotalWorkHours));
	int i=0, j=0;

	//Target Year-Month
	scanf("%7[0-9/]", in_ym);
	flush();

	int initTWH = initTotalWorkHours(in_ym, total);
	// if (initTWH == ERROR_P) return 1;

	while(1) {
		static time_t temp_weeklyWH = (time_t)0;
		static int lastWorkDay = 0;
		static int lastWorkWeekday = 7;
		DailyWorkHours *daily = (DailyWorkHours *)malloc(sizeof(DailyWorkHours));

		scanf("%60[ 0-9/:-]", in_wh);
		flush();

		int initDWH = initDailyWorkHours(in_wh, daily);
		if (initDWH == END) break;
		else if (initDWH == ERROR_P) return 1;
		if (isWorkingOnSameWeek(daily, lastWorkDay, lastWorkWeekday) == FAILED) temp_weeklyWH = (time_t)0;
		lastWorkDay = getWorkingDayNum(daily);
		lastWorkWeekday = getWorkingWeekdayNum(daily);
		setWeeklyWHOf(daily, temp_weeklyWH);

		int culcWH = culcWorkHours(total->yearMonth, daily);

		if (culcWH == CONTINUE) continue;
		else if (culcWH == ERROR_P) return 1;

		updateTotalWorkingHours(total, daily);

		temp_weeklyWH = getWeeklyWHOf(daily);

		free(daily);
		i++;
	}

	// printf("%d\n", roundSecToHour(total->nomalWH));
	printf("%d\n", roundSecToHour(total->fixedOWH));
	printf("%d\n", roundSecToHour(total->legalOWH));
	printf("%d\n", roundSecToHour(total->midnightOWH));
	printf("%d\n", roundSecToHour(total->nonlegalHolydayWH));
	printf("%d\n", roundSecToHour(total->legalHolydayWH));

	return 0;
}