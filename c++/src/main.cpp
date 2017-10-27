#include<bits/stdc++.h>
#include "working.h"

int main() {
	char *in_ym = (char *)malloc((TARGET_YEAR_MONTH_CHR_LEN+1)*sizeof(char));
	char *in_wh = (char *)malloc((WORKING_HOUR_PERIOD_CHR_LEN+1)*sizeof(char));
	TotalWorkHours *total = (TotalWorkHours *)malloc(sizeof(TotalWorkHours));
	int i=0, j=0;

	//Target Year-Month
	int ym = scanf("%7s", in_ym);
	flush();
	char *msg; sprintf(msg, "%s\n", ym);
	Log("DEBUG", msg);
	if (initTotalWorkHoursStruct(in_ym, total)) return 0;

	while(1) {
		static time_t temp_weeklyWH = (time_t)0;
		static int lastWorkDay = 0;
		static int lastWorkWeekday = 7;
		DailyWorkHours *daily = (DailyWorkHours *)malloc(sizeof(DailyWorkHours));
		scanf("%60[ 0-9/:-]", in_wh);
		flush();

		int initWH = initDailyWorkHoursStruct(in_wh, daily);

		if (initWH == 1) break;
		else if (initWH == 2) return 0;
		else {
			if (checkWorkDaily(daily, lastWorkDay, lastWorkWeekday) == FAIL) temp_weeklyWH = (time_t)0;
			lastWorkDay = getWorkingDate(daily);
			lastWorkWeekday = getWorkingWeekdayNum(daily);
			daily->weeklyWH = temp_weeklyWH;

			int culcWH = culcWorkHours(total->yearMonth, daily);

			if (culcWH == 1) continue;
			else if (culcWH == 2) return 0;

			total->nomalWH += daily->nomalWH;
			total->fixedOWH += daily->fixedOWH;
			total->legalOWH += daily->legalOWH;
			total->midnightOWH += daily->midnightOWH;
			total->nonlegalHolydayWH += daily->nonlegalHolydayWH;
			total->legalHolydayWH += daily->legalHolydayWH;

			temp_weeklyWH = weeklyWHOf(daily);

			free(daily);
		}
		i++;
	}

	char *msg; 
	// sprintf("%d\n", roundSecToHour(total->nomalWH));
	// Log("DEBUG", msg);
	sprintf(msg, "%d\n", roundSecToHour(total->fixedOWH));
	Log("DEBUG", msg);
	sprintf(msg, "%d\n", roundSecToHour(total->legalOWH));
	Log("DEBUG", msg);
	sprintf(msg, "%d\n", roundSecToHour(total->midnightOWH));
	Log("DEBUG", msg);
	sprintf(msg, "%d\n", roundSecToHour(total->nonlegalHolydayWH));
	Log("DEBUG", msg);
	sprintf(msg, "%d\n", roundSecToHour(total->legalHolydayWH));
	Log("DEBUG", msg);

	return 0;
}