#include "stdio_ex.h"

void flush() {
	int c;
	while(1) {
		c = getchar();
		if (c == 10 || c == EOF) break;
	}
}

int log(const char *tag, const char *str) {
	return printf("%s(%d):%s:[%s] %s\n", __FILE__, __LINE__, tag, __func__, str);
}