#include <string.h>
/**
 * delch '\n' from string.
 * @param char *	str		The target string.
 */
void lntrim(char *);

/**
 * Count how many character:c in the string
 * @param char *		str		The original string.
 * @param char *		c		The target character.
 * @return int			cnt		The number of trimed characters.
 */
int searchc(char *, char, int);

/**
 * Split string by separater string
 * @param char *		str		The original string.
 * @param char *	delim	The separater string.
 * @param char *[]		outlist		The output array of strings.
 * @return int			cnt		The split count.
 */
int split(char *, char, char *[], int);

/**
 * delch a character:c from string.
 * @param char *		str		The original string.
 * @param char	c		The target character.
 * @param char *		out		The string with delchd character.
 * @return int			cnt		The delchd count.
 */
int delch(char *, char, char *, int);