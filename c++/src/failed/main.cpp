#include<bits/stdc++.h>
#include "working.h"

int main() {
	char *in_ym = (char *)malloc(TARGET_YEAR_MONTH_CHR_LEN+1);
	char *in_wh = (char *)malloc(WORKING_HOUR_PERIOD_CHR_LEN+1);
	TotalWorkHours *total = (TotalWorkHours *)malloc(sizeof(TotalWorkHours));
	int i=0, j=0;

	//Target Year-Month
	int ym = scanf("%7s", in_ym);
	flush();
	log("DEBUG", "%s\n", ym);
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

	// log("DEBUG", "%d\n", "所定労働", roundTimeToHour(total->nomalWH));
	log("DEBUG", "%d\n", roundTimeToHour(total->fixedOWH));
	log("DEBUG", "%d\n", roundTimeToHour(total->legalOWH));
	log("DEBUG", "%d\n", roundTimeToHour(total->midnightOWH));
	log("DEBUG", "%d\n", roundTimeToHour(total->nonlegalHolydayWH));
	log("DEBUG", "%d\n", roundTimeToHour(total->legalHolydayWH));

	return 0;
}