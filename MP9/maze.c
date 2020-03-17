#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
/*
Murugan Narayanan
MP9
This program finds the correct path through a maze. The maze is read from a text file.
The createMaze function allocates memory for the maze_t struct, and memory is allocated
for each row of the maze, which contain pointers to each column of the maze, which have
memory allocated to them as well to hold each char. The destroyMaze function frees up the
memory that was allocated in the createMaze function. The printMaze function goes through
each character in the maze individually and prints it out, as well as a new line after every
row. Finally, the solveMazeManhattanDFS function uses recursion and backtracking to go through
possible paths in the maze until the correct one that leads to the end is found.
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    FILE *file;
    if (fopen(fileName, "r") == NULL)
      printf("Error opening file.");
    else
      file = fopen(fileName, "r"); // opens file in 'read' mode
    maze_t *maze = (maze_t*) malloc(sizeof(maze_t)); // allocates memory for maze_t struct
    fscanf(file, "%d %d\n", &(maze->width), &(maze->height)); // gets width and height from file
    maze->cells = malloc(sizeof(char*)*maze->height); // allocates memory for rows of pointers
    int k;
    for (k=0; k < maze->height; k++)
    {
      maze->cells[k] = malloc(sizeof(char)*maze->width); // allocates memory for each row, for the chars
    }
    int i,j;
    for (i = 0; i < maze->height; i++)
    {
      for (j = 0; j < maze->width; j++)
      {
        fscanf(file, "%c", &(maze->cells[i][j])); // fills maze char by char
      }
      fgetc(file);
    }
    fclose(file);
    int a,b;
    for (a=0; a < maze->height; a++)
    {
      for (b=0; b < maze->width; b++)
      {
        if (maze->cells[a][b] == START) // looks for START location
        {
          maze->startRow = a;
          maze->startColumn = b;
        }
        if (maze->cells[a][b] == END) // looks for END location
        {
          maze->endRow = a;
          maze->endColumn = b;
        }
      }
    }
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    int k;
    for (k=0; k < maze->height; k++)
    {
      free(maze->cells[k]); // first frees rows of chars
    }
    free(maze->cells); // then frees row of pointers
    free(maze); // finally frees maze_t struct
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    int i,j;
    for(i=0; i < maze->height; i++)
    {
      for(j=0; j < maze->width; j++)
      {
        char p = maze->cells[i][j]; // prints maze char by char
        printf("%c", p);
      }
      printf("\n"); // prints new line after every row
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */
int solveMazeManhattanDFS(maze_t * maze, int col, int row)
{
    if (row < 0 || col < 0 || row >= maze->height || col >= maze-> width) // check if in bounds
      return 0;
    if (maze->cells[row][col] == END) // check if reached end
      return 1;
    if (maze->cells[row][col] == WALL || maze->cells[row][col] == PATH || maze->cells[row][col] == VISITED) // paths you cannot go down
      return 0;
    if (maze->cells[row][col] == EMPTY)
      maze->cells[row][col] = PATH; // take the path
    if (solveMazeManhattanDFS(maze, col-1, row) == 1) // left
      return 1;
    if (solveMazeManhattanDFS(maze, col, row+1) == 1) // down
      return 1;
    if (solveMazeManhattanDFS(maze, col+1, row) == 1) // right
      return 1;
    if (solveMazeManhattanDFS(maze, col, row-1) == 1) // up
      return 1;
    if (maze->cells[row][col] != START)
      maze->cells[row][col] = VISITED; // backtracking
    return 0;
}
