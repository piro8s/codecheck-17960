#include <string.h>
/**
 * Delete '\n' from string.
 * @param char *	str		The target string.
 */
void lntrim(char *);

/**
 * Count how many character:c in the string
 * @param const char *		str		The original string.
 * @param const char	c		The target character.
 * @return int			cnt		The number of trimed characters.
 */
int search(const char *, const char *);

/**
 * Split string by separater string
 * @param const char *		str		The original string.
 * @param const char *	delim	The separater string.
 * @param char *[]		outlist		The output array of strings.
 * @return int			cnt		The split count.
 */
int split(const char *, const char *, char *[]);

/**
 * Delete a character:c from string.
 * @param const char *		str		The original string.
 * @param const char	c		The target character.
 * @param char *		out		The string with deleted character.
 * @return int			cnt		The deleted count.
 */
int delete(const char *, const char *, char *);