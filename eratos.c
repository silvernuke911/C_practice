#include <stdio.h>
#include <math.h>
/* -*- SilverNuke911 -*- */
void eratosthenes() {
    int N = 1000;
    int arr[N + 1];  // Need index 0-100 (N+1 elements)
    
    // Initialize: assume all numbers are prime (1 = prime, 0 = composite)
    for (int i = 2; i <= N; i++) {
        arr[i] = 1;  // initialize value to 1 
    }
    
    // Sieve logic - mark composites as 0
    for (int p = 2; p * p <= N; p++) {  // Only need to go up to sqrt(N)
        if (arr[p] == 1) {  // If p is prime
            for (int i = p * p; i <= N; i += p) {  // Start at p*p
                arr[i] = 0;  // Mark as composite             
			}
        }
    }
    
    // Print primes
    printf("Primes up to %d:\n", N);
    for (int i = 2; i <= N; i++) {
        if (arr[i] == 1) {
            printf("%d\n", i);
        }
    }
}

int main()
{
	eratosthenes();
	return 0;
} 

