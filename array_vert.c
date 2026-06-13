#include <stdio.h>

#define in  1
#define out 0

int isspace(int c) {
	if (c == ' ' || c == '\n' || c == '\t') {
		return 1;
	} else {
		return 0;
	}
}

int main()
{
  	int c; 
  	int max_len = 0;
   	int cur_len = 0;
   	int state = out;
   
	// Pass 1 - Find the max length of words
 	while ((c = getchar()) != EOF) {
     		if (isspace(c)) {
   			state = out;
   			cur_len = 0;
   		} else {
   			state = in;
   			cur_len++;
  		}
  
  		if (cur_len >= max_len) {
  			max_len = cur_len;
		}	
	}

	rewind(stdin); // rewind the input

	// create an array and populate
	int lengths[max_len + 1];
	for (int i = 0; i <= max_len; i++) {
		lengths[i] = 0;
	}

	state = out;
	cur_len = 0;
	//  Pass 2: find the frequency of each world length
 	while ((c = getchar()) != EOF) {
     	if (isspace(c)) {
			if (state == in) {
				lengths[cur_len]++;
			}
   			state = out;
   			cur_len = 0;
   		} else {
   			state = in;
			cur_len++;
		}	
	}

	// Find the max frequency
	int max_freq = 0;
	for (int i = 1; i <= max_len; i++) {
		if (lengths[i] >=  max_freq) {
			max_freq = lengths[i];
		} 
	}

	
	// Print the bars
	for (int j = max_freq; j>= 1; j--){
		for (int i = 1; i <= max_len; i++) {
			if (lengths[i] >= j) {
				printf("  #");
			} else {
				printf("   ");
			}
		}
		printf("\n");
	}	
	// Printing axes
	for (int i = 1; i <= max_len; i++) {
		printf("---");
	}	
	printf("\n");
	// Printing lengths
	for (int i = 1; i <= max_len; i++) {
		printf("%3d",i);
	}
	printf("\n");
	for (int i = 1; i <= max_len; i++) {
		printf("---");
	}	
	// Printing the freq at lengths
	printf("\n");
	for (int i = 1; i <= max_len; i++) {
		printf("%3d",lengths[i]);
	}

	printf("\n----- File word length frequency -----\n");
}


// THIS IS NOT PART OF THE PROGRAM

int define(int yes) {

}
