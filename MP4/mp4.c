#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*
Murugan Narayanan
MP4
This program finds the roots of a fourth order polynomial given an interval (l,r). Budan's Theorem is
implemented in the function rootbound to check if any roots can possibly be found in that interval.
The function rootbound uses a function called sign_changes to determine the number of sign changes
in the polynomials f(x+l) and f(x+r). If it is possible for roots to exist in that interval,
then the function newrfind_halley is used to execute Halley's Method to find the nearest root to
the starting value x. Other functions are called within newrfind_halley to calculate the value of the
polynomial and its first and second derivatives at different x values. These methods are similar, as they
essentially multiply different coeffecients by x raised to different powers, all added up. If a root is
found, it will be printed out. If either Budan's Theorem fails or if Halley's Theorem doesn't find a
root, a corresponding message will be printed out.
*/

// double pow(x,y) = x^y (for reference)

// declaring function header for later use
int sign_changes(double x1, double x2, double x3, double x4, double x5);

double abs_double(double y)
{
    //Change this to return the absolute value of y i.e |y|
    // y = fabs(y); equivalent function from <math.h>
    if (y < 0) // if negative, change to positive
      y = -y;
    return y;
}

double fx_val(double a, double b, double c, double d, double e, double x)
{
    //Change this to return the value of the polynomial at the value x
    // polynomial is ax^4 + bx^3 + cx^2 + dx + e
    double ans = a*pow(x,4) + b*pow(x,3) + c*pow(x,2) + d*x + e;
    return ans;
}

double fx_dval(double a, double b, double c, double d, double e, double x)
{
    //Change this to return the value of the derivative of the polynomial at the value x
    // derivative is 4ax^3 + 3bx^2 + 2cx + d
    double ans = 4*a*pow(x,3) + 3*b*pow(x,2) + 2*c*x + d;
    return ans;
}

double fx_ddval(double a, double b, double c, double d, double e, double x)
{
    //Change this to return the value of the double derivative of the polynomial at the value x
    // second derivative is 12ax^2 + 6bx + 2c
    double ans = 12*a*pow(x,2) + 6*b*x + 2*c;
    return ans;
}


double newrfind_halley(double a, double b, double c, double d, double e, double x)
{
    //Change this to return the root found starting from the initial point x using Halley's method
    double y = x;
    double y_old, num, den;
    for (int i = 0; i < 10000; i++) // should stop running after 10,000 times max (assume divergence)
    {
      y_old = y; // saves current value of y to compare to next value of y
      // calculates numerator and denominator
      num = (2*fx_val(a,b,c,d,e,y)*fx_dval(a,b,c,d,e,y));
      den = (2*pow(abs_double(fx_dval(a,b,c,d,e,y)),2)-fx_val(a,b,c,d,e,y)*fx_ddval(a,b,c,d,e,y));
      y = y - (num/den); // calculates next value of y
      if (abs_double(y - y_old) < 0.000001)
      {
        return y; // returns y if difference between old and new y < 0.000001 (convergence)
      }
    }
    return -99999; // if diverges, then return an extraneous value that wouldn't be a solution
}

int rootbound(double a, double b, double c, double d, double e, double r, double l)
{
    //Change this to return the upper bound on the number of roots of the polynomial in the interval (l, r)
    // compute p(x + l) coeffecients
    double c1, c2, c3, c4, c5;
    c1 = a;
    c2 = 4*a*l + b;
    c3 = 6*a*pow(l,2) + 3*b*l + c;
    c4 = 4*a*pow(l,3) + 3*b*pow(l,2) + 2*c*l + d;
    c5 = a*pow(l,4) + b*pow(l,3) + c*pow(l,2) + d*l + e;
    int vl = sign_changes(c1,c2,c3,c4,c5); // # of sign changes in p(x + l)

    // compute p(x + r) coeffecients
    double d1, d2, d3, d4, d5;
    d1 = a;
    d2 = 4*a*r + b;
    d3 = 6*a*pow(r,2) + 3*b*r + c;
    d4 = 4*a*pow(r,3) + 3*b*pow(r,2) + 2*c*r + d;
    d5 = a*pow(r,4) + b*pow(r,3) + c*pow(r,2) + d*r + e;
    int vr = sign_changes(d1,d2,d3,d4,d5); // # of sign changes in p(x + r)

    int ans = (int) abs_double(vl - vr); // returns upper bound as an integer
    return ans;
}

int sign_changes(double x1, double x2, double x3, double x4, double x5)
{
  // this function calculates the number of sign changes given the polynomial's coeffecients
  int flagp, flagn, count = 0;
  // if positive coeffecient, corresponding t is 1. If negative coeffecient, then 0.
  int t1=0, t2=0, t3=0, t4=0, t5=0;
  if (x1 > 0)
    t1 = 1;
  if (x2 > 0)
    t2 = 1;
  if (x3 > 0)
    t3 = 1;
  if (x4 > 0)
    t4 = 1;
  if (x5 > 0)
    t5 = 1;
  int arr[5] = {t1,t2,t3,t4,t5} ; // stores 'sign' of each coeffecient in an array

  // runs through array comparing adjacent pairs of 'signs'
  int i;
  for (int j = 1 ; j < 5 ; j++)
  {
    i = j - 1;
    flagp = arr[i];
    flagn = arr[j];

    // if the 'signs' are not the same, then increment counter because sign change has occurred
    if (flagp != flagn)
      count++;
  }
  return count; // returns number of sign changes
}

int main(int argc, char **argv)
{
	double a, b, c, d, e, l, r;
	FILE *in;

	if (argv[1] == NULL)
  {
		printf("You need an input file.\n");
		return -1;
	}

	in = fopen(argv[1], "r");
	if (in == NULL)
		return -1;
	fscanf(in, "%lf", &a);
	fscanf(in, "%lf", &b);
	fscanf(in, "%lf", &c);
	fscanf(in, "%lf", &d);
	fscanf(in, "%lf", &e);
	fscanf(in, "%lf", &l);
	fscanf(in, "%lf", &r);

    /*
    The values are read into the variable a, b, c, d, e, l and r.
    You have to find the bound on the number of roots using rootbound function.
    If it is > 0 try to find the roots using newrfind function.
    You may use the fval, fdval and fddval functions accordingly in implementing
    Halleys's Method.
    */

    fclose(in);

    // use Budan's Theorem to check if there are any roots at all within range
    if (rootbound(a,b,c,d,e,r,l) == 0)
    {
      printf("The polynomial has no roots in the given interval.\n");
      return 0; // end program in no roots are present in interval
    }

    // finds nearest roots for every k in interval (l,r) incrementing by 0.5
    for (double k = l; k <= r ; k+=0.5)
    {
      double ans = newrfind_halley(a,b,c,d,e,k);
      if (ans == -99999) // this means divergence has occurred
        printf("No roots found.\n");
      else
        printf("Root found: %lf\n", ans); // root printed as a double if found
    }

    return 0;
}
