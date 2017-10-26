#include "string_ex.h"

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

int search(const char *str, const char *c){
	int i, cnt = 0;

	for(i=0; i<strlen(str); i++){
		if(str[i] == *c) cnt++;
	}
	return cnt;
}

int split(const char *str, const char *c, char *outlist[] ) {
	char *tk;
	int cnt = search(str, c);
	int i;

	if (cnt < 1) return cnt;
	tk = strtok(str, c);
	outlist[0] = tk;
	for (i=1; i<cnt+1; i++) {
		tk = strtok(NULL, c);
		outlist[i] = tk;
	}

	return cnt;
}

int delete(const char *str, const char *c, char *out) {
	int i, j;

	for (i=0,j=0; i<strlen(str); i++) {
		if (str[i] == *c) {
			j++;
			continue;
		}
		new[i-j] = str[i];
	}
	return j;
}