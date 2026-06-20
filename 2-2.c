#include <stdio.h>
// get char
int main () {
	int lim = 256;
	char s[lim];
	int idx;
	for ( int i = 0; i < lim - 1; i++)
	{	
		char c = getchar();
		if (c == '\n') {
			break;
		}
		if (c == EOF) {
			break;
		}
		s[idx] = c;
		idx++;
	}
	s[idx] = '\0';
	return 0;
}
/* -*- SilverNuke911 -*- */