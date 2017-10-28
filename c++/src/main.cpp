#include<bits/stdc++.h>
#include "working.h"

int main() {
	char *in_ym = (char *)malloc((TARGET_YEAR_MONTH_CHR_LEN+1)*sizeof(char));
	char *in_wh = (char *)malloc((WORKING_HOUR_PERIOD_CHR_LEN+1)*sizeof(char));
	TotalWorkHours *total = (TotalWorkHours *)malloc(sizeof(TotalWorkHours));
	int i=0, j=0;

	//Target Year-Month
	scanf("%7s", in_ym);
	flush();
	char *msg; sprintf(msg, "%s\n", in_ym);
	Log("DEBUG", msg);
	int initTWH = initTotalWorkHours(in_ym, total);
	if (initTWH == ERROR_P) return 1;

	while(1) {
		static time_t temp_weeklyWH = (time_t)0;
		static int lastWorkDay = 0;
		static int lastWorkWeekday = 7;
		DailyWorkHours *daily = (DailyWorkHours *)malloc(sizeof(DailyWorkHours));
		// inputstreamの頭の0を削除する
		if (lastWorkDay != 0) {
			int c = fgetc(stdin);
			if (c != '0') {
				sprintf(msg, "fgetc:%c\n", c);
				Log("DEBUG", msg);
			}
		}

		scanf("%60[ 0-9/:-]", in_wh);
		flush();

		int initDWH = initDailyWorkHours(in_wh, daily);
		if (initDWH == END) break;
		else if (initDWH == ERROR_P) return 1;


		if (isWorkingOnSameWeek(daily, lastWorkDay, lastWorkWeekday) == FAILED) temp_weeklyWH = (time_t)0;
		lastWorkDay = getWorkingDayNum(daily);
		lastWorkWeekday = getWorkingWeekdayNum(daily);
		setWeeklyWHOf(daily, temp_weeklyWH);
		// daily->weeklyWH = temp_weeklyWH;
		// inputstreamの頭に0を追加する
		fputc('0', stdin);

		int culcWH = culcWorkHours(total->yearMonth, daily);

		if (culcWH == CONTINUE) continue;
		else if (culcWH == ERROR_P) return 1;

		updateTotalWorkingHours(total, daily);
		// total->nomalWH += daily->nomalWH;
		// total->fixedOWH += daily->fixedOWH;
		// total->legalOWH += daily->legalOWH;
		// total->midnightOWH += daily->midnightOWH;
		// total->nonlegalHolydayWH += daily->nonlegalHolydayWH;
		// total->legalHolydayWH += daily->legalHolydayWH;

		temp_weeklyWH = getWeeklyWHOf(daily);

		free(daily);
		i++;
	}

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