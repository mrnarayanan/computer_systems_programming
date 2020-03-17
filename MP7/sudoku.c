#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------
/*
Murugan Narayanan
MP7
This program solves a given sudoku puzzle. The first 3 functions check if a particular
number exists in a particular row, column, or 3x3 sector. The is_val_in_row function holds the row index
constant and runs through every column in the row to see if the number is present in it. The is_val_in_col
function does the same thing, but holds the column index constant and checks through every row in that column.
For the is_val_in_3x3_zone function, I partition the sudoku board into its 9 sectors, and check to see which
sector the given index lies in. I then run through the row and column indices within that sector using nested
for loops to check whether the number already exists in it or not. The is_val_valid function calls the 3 previous
functions to make sure that the number isn't already present in the current row, column, or 3x3 sector, and returns
1 if that is true, meaning the number can be filled at that location. The solve_sudoku function uses backtracking
recursion to fill in all the entries for the sudoku board. The base case occurs when the entire board is filled,
and the function then returns 1. If an empty entry is found (0 is filled in instead of 1-9), a number from 1-9 is
checked to see if it is valid at that entry, and is filled in. The function is then called recursively to continue
filling in empty entries. If an entry fails to work, then the entry is set back to 0. Then, a new number is tried. If no
numbers work, then 0 is returned, and the function backtracks, setting a previous entry to 0 and trying the next number for
it. The process keeps repeating until all the numbers are valid and the sudoku board is completely filled with values, 
thus solving the puzzle.
*/
// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  for (int a = 0; a < 9; a++) // used to cycle through columns
  {
    if (sudoku[i][a] == val) // holds row constant at i, checks through columns
      return 1;
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  for (int a = 0; a < 9; a++) // used to cycle through rows
  {
    if (sudoku[a][j] == val) // holds column constant at j, checks through rows
      return 1;
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  /*
  Sector Map
  1 2 3
  4 5 6
  7 8 9
  */

  // BEG TODO
  if (i >= 0 && i <= 2 && j >= 0 && j <= 2) // Sector 1
  {
    for (int x=0; x<=2; x++)
    {
      for (int y=0; y<=2; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else if (i >= 3 && i <= 5 && j >= 0 && j <= 2) // Sector 4
  {
    for (int x=3; x<=5; x++)
    {
      for (int y=0; y<=2; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else if (i >= 6 && i <= 8 && j >= 0 && j <= 2) // Sector 7
  {
    for (int x=6; x<=8; x++)
    {
      for (int y=0; y<=2; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else if (i >= 0 && i <= 2 && j >= 3 && j <= 5) // Sector 2
  {
    for (int x=0; x<=2; x++)
    {
      for (int y=3; y<=5; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else if (i >= 0 && i <= 2 && j >= 6 && j <= 8) // Sector 3
  {
    for (int x=0; x<=2; x++)
    {
      for (int y=6; y<=8; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else if (i >= 3 && i <= 5 && j >= 3 && j <= 5) // Sector 5
  {
    for (int x=3; x<=5; x++)
    {
      for (int y=3; y<=5; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else if (i >= 3 && i <= 5 && j >= 6 && j <= 8) // Sector 6
  {
    for (int x=3; x<=5; x++)
    {
      for (int y=6; y<=8; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else if (i >= 6 && i <= 8 && j >= 3 && j <= 5) // Sector 8
  {
    for (int x=6; x<=8; x++)
    {
      for (int y=3; y<=5; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  else // Sector 9 (implicitly for condition i >= 6 && i <= 8 && j >= 6 && j <= 8)
  {
    for (int x=6; x<=8; x++)
    {
      for (int y=6; y<=8; y++)
      {
        if (sudoku[x][y] == val)
          return 1;
      }
    }
  }

  return 0;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  // if val exists in row or column or 3x3 zone, then cannot fill in
  if (is_val_in_row(val,i,sudoku) || is_val_in_col(val,j,sudoku) || is_val_in_3x3_zone(val,i,j,sudoku))
    return 0;
  else
    return 1;
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i, j, flag = 0; // initializes matrix index counters and flag to check if no blanks in board
  for (i=0; i<9; i++)
  {
    for (j=0; j<9; j++)
    {
      if (sudoku[i][j] == 0) // checks if entry is unfilled
      {
        flag = 1; // sets flag to indicate board has unfilled entries
        goto post; // exits out of nested loops since empty entry has been found, at current i and j
      }
    }
  }
  post: // label indicating section of code to go to after finding an empty entry
  if (flag == 0) // if flag is still 0, that means all entries in board are filled
    return 1; // base case: returns 1 when board is completely filled

  for (int num = 1; num <= 9; num++) // runs through the 9 possible numbers to fill in board
  {
    if (is_val_valid(num, i, j, sudoku) == 1) // checks if number to enter isn't in current row, column, or 3x3 section
    {
      sudoku[i][j] = num; // sets current empty space to num
      if (solve_sudoku(sudoku) == 1) // checks if recursive call to solve_sudoku returns true, meaning next entry worked
        return 1; // if next entry worked, then current entry also worked
      sudoku[i][j] = 0; // if next entry didn't work, then reset current entry to 0
    }
  }

  return 0; // current entry didn't work, tried all 9 numbers and none fit
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
