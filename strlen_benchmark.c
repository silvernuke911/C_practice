// -*- SilverNuke911 -*- //
#include <stdio.h>
#include <string.h>
#include <time.h>

size_t strlen2(const char s[])
{
	int i = 0;
	while (s[i] != '\0') {
		i++;
	}
	return i;
}

size_t strlen3(const char s[])
{
	int i;
	for (i = 0; s[i] != '\0'; i++);
	return i;
}

size_t strlen4(const char s[])
{
	const char *p = s;
	while (*p) {
		p++;
	}
	return p - s;
}

size_t strlen5(const char s[])
{
	size_t n = 0;
	while (*s++) {
		n++;
	}
	return n;
}

#define RUNS 10000000
// bench marks the functions given by running it RUNS times
// and returns the total time and average function runtime
// that's about it yes
void benchmark(const char *name, size_t (*func)(const char *), const char *s)
{
	clock_t start, end;
	volatile size_t len;

	start = clock();

	for (int i = 0; i < RUNS; i++) {
		len = func(s);
	}
	(void)len;

	end = clock();

	double total = (double)(end - start) / CLOCKS_PER_SEC;
	double avg = total / RUNS;

	printf("%s\n", name);
	printf("  total time  : %12.6f s\n", total);
	printf("  avg time    : %12.6f ns\n\n", avg * 1e9);
}

int main(void)
{
	const char *s = "okane kazegu, watashi wa star";

	benchmark("strlen  (libc)", strlen, s);
	benchmark("strlen2 (array while)", strlen2, s);
	benchmark("strlen3 (for loop)", strlen3, s);
	benchmark("strlen4 (pointer diff)", strlen4, s);
	benchmark("strlen5 (pointer ++)", strlen5, s);

	return 0;
}
