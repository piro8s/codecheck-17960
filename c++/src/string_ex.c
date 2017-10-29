#include "include/string_ex.h"
#include "include/stdio_ex.h"

void lntrim(char *str) {
	int i = 0;
	while(1) {
	if(str[i] == '\n') {
		str[i] = '\0';
		break;
	}
	i++;
	}
}

int searchc(char *str, char c){
	int i, cnt = 0;

	for(i=0; i<strlen(str); i++){
		if(str[i] == c) cnt++;
	}
	return cnt;
}

int split(char *str, char c, char *outlist[] ) {
	char *tk;
	int cnt = searchc(str, c);
	int i;

	if (cnt < 1) return cnt;
	tk = strtok(str, &c);
	outlist[0] = tk;

	for (i=1; i<cnt+1; i++) {
		tk = strtok(NULL, &c);
		outlist[i] = tk;
	}

	return cnt;
}

int delch(char *str, char c, char *out) {
	int i=0, j=0;

	for (i=0; i<strlen(str); i++) {
		if (str[i] == c) {
			j++;
			continue;
		}
		out[i-j] = str[i];
	}
	return j;
}