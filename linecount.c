#include <stdio.h>
/* -*- SilverNuke911 -*- */
int main()
{
	int c, nl, nb, nt;
	nl = 0;
	nt = 0;
	nb = 0;
	while ((c = getchar()) != EOF) {
		if (c == '\n') ++nl;
		if (c == '\t') ++nt;
		if (c == ' ') ++ nb;
	}
	printf("Number of new lines %d\n",nl);
	printf("Number of tabs %d\n", nt);
	printf("Number of blanks %d\n", nb);
}
