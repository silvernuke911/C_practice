/* -*- SilverNuke911 -*- */
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
	rewind(stdin);
	int lengths[max_len + 1];

	for (int i = 0; i <= max_len; i++) {
		lengths[i] = 0;
	}

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
		// printf("s %d c %d l[c]%d\n", state,cur_len, lengths[cur_len]);
	}
	// for (int i = 0; i <= max_len; i++) {
	// 	printf("%2d ",i);
	// }
	// printf("\n");
	// for (int i = 0; i <= max_len; i++) {
	// 	printf("%2d ",lengths[i]);
	// }
	printf("--- File word length frequency ---\n");
	for (int i = 1; i <= max_len; i++) {
		printf("%2d  ", i);
		for (int j = 0; j <= lengths[i]; j++) {
			printf("#");
		}
		printf("\n");
	}
}

