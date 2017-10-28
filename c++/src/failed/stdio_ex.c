#include "stdio_ex.h"

void flush() {
printf("##%s\n", __func__);
	int c;
	while(1) {
		c = getchar();
		if (c == 10 || c == EOF) break;
	}
}

int Log(const char *tag, const char *str) {
printf("##%s\n", __func__);
	return printf("%s(%d):%s:[%s] %s\n", __FILE__, __LINE__, tag, __func__, str);
}