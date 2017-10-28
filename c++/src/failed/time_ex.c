#include "time_ex.h"
#include "stdio_ex.h"

int roundSecToHour(time_t t) {
	return t/60/60;
}
