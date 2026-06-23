#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// this function makes a new string and prints that out 
// it takes the string 's', makes a copy of it, and performs 
// a to-upper operation, and returns the copied string
char* to_upper_copy( const char* s)
{
	// int len = strlen(s);
	int len;
	for (len = 0; s[len] != '\0'; len++){}; // replaced with a for loop to find strlen

	char* p = malloc(len + 1);

	if (p == NULL) {
		return NULL; // out of memory
	}
	
	for (int i = 0; i <= len; i++) {
		p[i] = s[i];
	}

	for (int i = 0; p[i] != '\0'; i++) {
		if ((p[i] >= 'a') && (p[i] <= 'z')) {
			p[i] = p[i] - ('a' - 'A');
		}
	}
	return p;
}

// this function makes a new string and prints that out 
// it takes the string 's', makes a new copy, performs a 
// to-lower operation, and returns the copied string
char* to_lower_copy( const char* s) 
{
	// int len = strlen(s);
	int len;
	for (len = 0; s[len] != '\0'; len++){}; 

	char* p = malloc(len + 1);

	if (p == NULL) {
		return NULL; // out of memory
	}

	// strcpy(p,s);
	for (int i = 0; i <= len; i++) {
		p[i] = s[i];
	}

	for (int i = 0; p[i] != '\0'; i++) {
		if ((p[i] >= 'A') && (p[i] <= 'Z')) {
			p[i] = p[i] + ('a' - 'A');
		}
	}
	return p;
}

// this function takes the mutable string and changes its contents
// it takes the adress of a mutable array, and replaces the contents 
// of those adresses with a toupper operation
void to_upper_ptr(char* s)
{
	for (int i = 0; s[i] != '\0'; i++) {
		if ((s[i] >= 'a') && (s[i] <= 'z')) {
			s[i] = s[i] - ('a' - 'A');
		}
	}
}


// this function takes the mutable string and changes its contents
// it takes the address of a mutable array, and replaces the contents 
// with a tolower operation, changing the original word.
void to_lower_ptr(char* s)
{
	for (int i = 0; s[i] != '\0'; i++) {
		if ((s[i] >= 'A') && (s[i] <= 'Z')) {
			s[i] = s[i] + ('a' - 'A');
		}
	}
}

int main()
{
	// make a sample word
	char word[] = "ThiS iS a BunCh oF wOrDs";
	printf("words        :\t%s\n", word);

	// make the new string for upper
	char *word_copy_up = to_upper_copy( word );
	printf("to_upper_cpy :\t%s\n",word_copy_up);

	// check what happened to the original
	printf("words og     :\t%s\n", word);
	free(word_copy_up); // manual free

	// make the new string for lower
	char *word_copy_low = to_lower_copy( word );
	printf("to_lower_cpy :\t%s\n",word_copy_low);

	// check what happened to the original
	printf("words og     :\t%s\n", word);
	free(word_copy_low); // manual free
	
	// perform a to-upper operation on the word itself
	to_upper_ptr(word);
	
	// check the current original 
	printf("words up ptr :\t%s\n", word);

	// perform a to-lower operation on the word itself
	to_lower_ptr(word);

	// check what happened to the original
	printf("words lw ptr :\t%s\n", word);

	return 0;
}
// what did we learn?
// string[] points to mutable arrays that can be changed by functions when their pointers are passed;
// *string points to arrays which are immutable/read only. and cannot be changed;
// yippee

// char[] creates an array that owns storage.
// char* is just a pointer.
//
// Whether the data is mutable depends on what the pointer points to,
// not on whether you use [] or *.


// Wallahi habibi that is all we learned 
/* -*- SilverNuke911 -*- */

