#include "time_ex.h"
#include "stdio_ex.h"

int roundSecToHour(time_t t) {
printf("##%s\n", __func__);
	return t/60/60;
}
