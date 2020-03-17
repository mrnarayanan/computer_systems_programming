#include "prime.h"

int is_prime(int n)
{
  // Return 1 if n is a prime, and 0 if it is not
  for (int i = 2 ; i < n ; i++)
  {
    if (n%i == 0)
      return 0;
  }
  return 1;
}
