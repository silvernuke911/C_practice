#include <stdio.h>

// Take an input, replace all tabs with '\t', all backspaces with \b
// all newlines with \nn and each backslash with \\

int main()
{
	int c;
	while ((c = getchar()) != EOF) {
		switch (c) {
			case '\t':
				putchar('\\');
				putchar('t');
				break;
			case '\b':
				putchar('\\');
				putchar('b');
				break;
			case '\\':
				putchar('\\');
				putchar('\\');
				break;
			case '\n':
				putchar('\\');
				putchar('n');
				break;
			default:
				putchar(c);
				break;
		}
	}	
	return 0;
}
