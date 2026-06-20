#include <stdio.h>
#include <math.h>
/* -*- SilverNuke911 -*- */
int main()
{
	double S;
	scanf("%lf", &S);

	double x = S;
	double x_prev = x;
	double eps = 1e-16;
	int max_iter = 1000;

	for ( int i = 0; i <= max_iter; i++){
		x =	0.5 * (x + S / x);
		if (fabs(x - x_prev) <= eps) {
			break;
		}
		x_prev = x;
	}

	printf("%.16lf\n", x);
	
	return 0;
}
