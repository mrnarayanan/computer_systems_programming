#include <stdio.h>
#include <stdlib.h>

/*
Murugan Narayanan
MP3
This program prints out a specified line of Pascal's Triangle.
To compute the factorial of numbers, I wrote a recursive function that
multiplies the number to one less than itself and keeps going until the
number becomes 0, with the final operand being 1. The function returns the
factorial of the parameter as a double.
In the main function, the input n for the row index is received from the
user. Each entry in a row is (n choose k), with k running from 0 to n
incrementing by 1. So, I created a for loop with those specifications.
I compute each (n choose k) as a double for greater precision, but then when
printing it I use %.0lf so that no decimal places print, only the integer part.
*/

double factorial (double num)
{
  if (num == 0)
  {
    return 1; // 1 is the last operand of the factorial
  }
  else
  {
    return num * factorial(num - 1); // factorial is recursive multiplication
  }
}

int main()
{
  int n; // row index

  printf("Enter the row index : ");
  scanf("%d",&n);

  // Your code starts from here
  for(int k=0; k <= n; k++)
  {
    /* perform computation as double for more precision, but print
    output with no decimal places, so that only integer part displays */
    double entry = factorial(n)/(factorial(k)*factorial(n-k));
    printf("%.0lf ", entry); // prints term followed by a space
  }
  printf("\n");

  return 0;
}
