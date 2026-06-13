#include <stdio.h>

// take an input file, check if there are spaces
// if there are multiple, just compress it to one space
int main()
{
	int c;
	int prev = 0;
	while ((c = getchar()) != EOF) {
		if (c == ' ') {
			if (!prev) {
				putchar(' ');
				prev = 1;
			} 
		}
		else {
			putchar(c);
			prev = 0;
		}
	}	
	return 0;
}
