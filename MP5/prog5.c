/* Murugan Narayanan
 * MP5
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */

 /*
Intro paragraph:
The object of this Codebreaker game is to guess 4 random numbers within 12 attempts.
This C file provides the functions used by the main.c file. The set_seed function checks
if the user input for the seed is valid and sets that value as the seed if it is. The
start_game function initializes 4 psuedo random numbers to create the solution set, and
initializes the guess attempts counter. Finally, the bulk of the game's logic resides in
the make_guess function. First, the user's guesses are checked to make sure they are valid
and fall within the valid range for this game, from 1 to 8. Then, exact matches are checked for,
which occur when the guess number matches the solution number in the same position. To do this,
I store the guesses and solutions in separate arrays and run through them using a for loop, and test
to see if the values at the same index are equal. I have another two arrays to track if each guess and
solution have been paired, to prevent double counting of a single guess corresponding to multiple of the
same number in the solution set. If the number at the corresponding index is 0, then that number hasn't
been paired, and if it is 1, then it has been paired. After checking for exact matches, I check for misplaced
matches. For this, I use nested for loops to run through the indices of the guesses array and the solutions
array. I compare each value in the guesses array with each value in the solutions array, and increment the
misplaced matches counter only if the two numbers are equal and both of them haven't been paired yet. The results
of each guess attempt is then printed onto the screen. The remaining logistics of the program are managed by the
main.c file. 
*/

/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */

#include <stdio.h>
#include <stdlib.h>
#include "prog5.h"

/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;

/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid.
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int set_seed (const char seed_str[])
{
//    Example of how to use sscanf to read a single integer and check for anything other than the integer
//    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
//    The user should enter only an integer, and nothing else, so we will check that only "seed" is read.
//    We declare
      int seed;
      char post[20];
//    The sscanf statement below reads the integer into seed.
      int num = sscanf (seed_str, "%d%1s", &seed, post);
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully.
//    Check that the return value is 1 to ensure the user enters only an integer.
//    Feel free to uncomment these statements, modify them, or delete these comments as necessary.
//    You may need to change the return statement below
    if (num==1) // checks if sscanf yielded valid seed number
    {
      srand(seed); // sets the seed
      return 1;
    }
    else
    {
      printf("set_seed: invalid seed\n");
      return 0;
    }
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above.
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void start_game (int* one, int* two, int* three, int* four)
{
    // generates a number from 0 to 7, then adds 1 to make range 1 to 8
    solution1 = rand() % 8 + 1;
    solution2 = rand() % 8 + 1;
    solution3 = rand() % 8 + 1;
    solution4 = rand() % 8 + 1;
    *one = solution1;
    *two = solution2;
    *three = solution3;
    *four = solution4;
    guess_number = 1; // initializes guess_number to 1
    return;
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str.
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess)
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int make_guess (const char guess_str[], int* one, int* two, int* three, int* four)
{
//  One thing you will need to read four integers from the string guess_str, using a process
//  similar to set_seed
//  The statement, given ...
//  will read four integers from guess_str into the integers and read anything else present into post
//  The return value of sscanf indicates the number of items sucessfully read from the string.
//  You should check that exactly four integers were sucessfully read.
//  You should then check if the 4 integers are between 1-8. If so, it is a valid guess
//  Otherwise, it is invalid.
//  Feel free to use this sscanf statement, delete these comments, and modify the return statement as needed

  char post[20];
  int w,x,y,z;
  int val = sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post);

  if (val != 4) // invalid guess if 4 ints not read by sscanf
  {
    printf("make_guess: invalid guess\n");
    return 0;
  }

  // checks if any input is out of legal range for game
  if (w < 1 || w > 8 || x < 1 || x > 8 || y < 1 || y > 8 || z < 1 || z > 8)
  {
    printf("make_guess: invalid guess\n");
    return 0;
  }

  *one = w;
  *two = x;
  *three = y;
  *four = z;

  // creates 4 arrays for: guesses, solutions, pair checking for guesses, pair checking for solutions
  int guesses[4] = {w,x,y,z};
  int reals[4] = {solution1,solution2,solution3,solution4};
  int pairg[4] = {0,0,0,0}; // pairing check for guesses
  int pairs[4] = {0,0,0,0}; // pairing check for solutions
  int exact = 0, misp = 0; // initializes counters for exact matches and misplaced matches

  for (int i = 0; i < 4; i++)
  {
    if (guesses[i] == reals[i]) // checks for exact matches - must be at same index in both arrays
    {
      exact++; // increment exact match counter
      pairg[i] = 1; // if matched, corresponding index of that matched number changed to 1 to indicate it was paired
      pairs[i] = 1; // indicate that matched number is now paired
    }
  }

  for (int j = 0; j < 4; j++) // runs through indices of guesses array
  {
    for (int k = 0; k < 4; k++) // runs through indices of solutions array
    {
      if (guesses[j] == reals[k] && pairg[j] == 0 && pairs[k] == 0) // misplaced match if same number but not already paired
      {
        misp++; // increment misplaced match counter
        pairg[j] = 1; // indicate that matched number is now paired
        pairs[k] = 1; // indicate that matched number is now paired
      }
    }
  }
  // print results of current guess
  printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n", guess_number, exact, misp);
  guess_number++; // increment guess_number
  return 1;
}
