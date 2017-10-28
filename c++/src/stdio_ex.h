#include <stdio.h>

#define ERROR(fmt, ...) err_msg(__FILE__, __FUNCTION__, __LINE__, "error", fmt, ##__VA_ARGS__)
#define WARNNING(fmt, ...) err_msg(__FILE__, __FUNCTION__, __LINE__, "warnning", fmt, ##__VA_ARGS__)

void err_msg(const char *, const char *, int, const char *, const char *, ...);
/**
 * Reset buffer on stdin stream.
 */
void flush(void);

int Log(const char *, const char *);