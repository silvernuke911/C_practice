#include <stdio.h>

float F_to_C (float fahr){
	return (5.0 /9.0) * (fahr - 32.0);
}

float C_to_F (float celsius) {
	return (celsius * (9.0 / 5.0) + 32);
}

int main()
{
	float lower = 0;
	float upper = 300;
	float step  = 20;
	float celsius;
	float fahr;
	printf("C    \tF     \n");
	printf("---------------\n");
	for (int f = lower; f <= upper; f += step) {
		fahr = (float) f;
		celsius = F_to_C(fahr);
		printf("%3.0f\t%6.2f\n",fahr,celsius);
	}
	printf("\n");	
	printf("F    \tC     \n");
	printf("---------------\n");
	for (int c; c <= upper; c += step) {
		celsius = (float) c;
		fahr = C_to_F(celsius);
		printf("%3.0f\t%6.2f\n", celsius, fahr);
	}
	printf("\n");	
	printf("C    \tF     \n");
	printf("---------------\n");
	for (int f = upper; f >= lower; f -= step) {
		fahr = (float) f;
		celsius = F_to_C(fahr);
		printf("%3.0f\t%6.2f\n",fahr,celsius);
	}
}
