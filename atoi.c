#include <stdio.h>
int atoi2( char *s) {
	int n = 0;
	for (int i = 0; s[i] >= '0' && s[i] <= '9'; i++) {
		n = 10 * n + (s[i] - '0');
	}
	return n;
}

char *s = "299792458";
int main() {
	int n = atoi2(s);
	printf("%i",n);
	return 0;
}
/* -*- SilverNuke911 -*- */
