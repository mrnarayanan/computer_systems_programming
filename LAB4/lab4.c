#include <stdio.h>
#include "prime.h"

int main()
{
  /* Write the code to take a number n from user and print all
  the prime numbers between 1 and n */
  int n;
  printf("Enter the value of n: ");
  scanf("%d", &n);

  printf("\nPrinting primes less than or equal to: %d\n", n);

  for(int p = 2 ; p <= n ; p++)
  {
      if (is_prime(p) == 1)
        printf("%d ", p);
  }

  printf("\n");
  return 0;
}
