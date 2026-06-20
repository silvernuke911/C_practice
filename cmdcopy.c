#include <stdio.h>
/* -*- SilverNuke911 -*- */
// read a character and output it (I guess) 

int main() 
{
	int c;
	c = getchar();
	int ctr = 0;
	int a;
	while ( c != EOF ) {
		putchar(c);
		c = getchar();
	}
}
