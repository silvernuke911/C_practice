#include <stdio.h>
/* -*- SilverNuke911 -*- */
// word counter 
#define IN  1 //  inside a word
#define OUT 0 // outside a word
			  
/* This program counts lines, words, and characters
 * as from an input
 * We define words as a sequence of characters that does not contain a blank
 * tab, or new line
 */

int main()
{
	int c, nl, nw, nc, state;
	state = OUT;
	nl = nw = nc = 0;
	while ((c = getchar()) != EOF) {
		nc++;
		if (c == '\n') {
			nl++;
		}
		if (c ==' '|| c == '\n' || c == '\t') {
			state = OUT;
		} else if (state == OUT) {
			printf("\n");
			state = IN;
			nw++;
		}
		if (state == IN) {
			putchar(c);
		}
	}
	printf("\n--------------\n");
	printf("nl %d nw %d nc %d\n", nl, nw, nc);
	return 0;
}
