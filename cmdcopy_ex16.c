#include <stdio.h>
/* -*- SilverNuke911 -*- */
// Verify that the expression getchar() != EOF is either 0 or 1.
// EOF in this case is Ctrl + Z


int main() 
{
	int c;
	c = getchar();
	int ctr = 0;
	int a;
	while (ctr <= 20) {
		a = ( c != EOF );
		printf("%d\n",a);
		if (a == 0) {
			printf("--------\n");
			printf("%d\n",c);
			printf("--------\n");
		}
		c = getchar();
		ctr++;
	}
}
