#include <stdio.h>


/* strlen2
 *
 * Uses array indexing and a while loop.
 * Start at index 0 and keep counting until
 * the null terminator '\0' is found.
 */
int strlen2(char s[])
{
	int i = 0;
	while (s[i] != '\0') {
		i++;
	}
	return i;
}

/* strlen3
 *
 * Same algorithm as strlen2, but written
 * using a for loop.
 */
int strlen3(char s[])
{
	int i;
	for (i = 0; s[i] != '\0'; i++);
	return i;
}

/* strlen4
 *
 * Uses a pointer instead of an index.
 *
 * p starts at the beginning of the string.
 * Increment p until it reaches the null
 * terminator.
 * The length is the difference between
 * the final pointer and the starting pointer.
 */
int strlen4(char s[])
{
	char *p = s;
	while (*p) {
		p++;
	}
	return p - s;
}

/* strlen5
 *
 * Uses pointer arithmetic and post-increment.
 *
 * *s++ means:
 *     *(s++)
 *
 * Read the current character, then move
 * the pointer to the next character.
 * Count how many non-zero characters are
 * encountered before reaching '\0'.
 */
int strlen5(char s[])
{
	int n = 0;
	while (*s++) {
		n++;
	}
	return n;
}


// ***************************************
//  MAIN 
// ***************************************
int main(int argc, char *argv[])
{
	int len = 0;
	char *s = argv[1];
	if (argc >= 2) {
		len = strlen2(s);
	}
	printf("string length : %d\n", len);
	return 0;
}
/* -*- SilverNuke911 -*- */
