/*
Murugan Narayanan
MP6
This program provides the primary functions used by the Game of Life game.
The countLiveNeighbor function coutns the number of alive neighbors in the surrounding
cells of your current cell. Each cell has up to 8 neighbors, so I make sure to check if
a neighbor exists before checking to see if it is alive or dead. This is very important
for the cells at the corners and edges of the board. The updateBoard function takes the
game board to the next state by checking all the cells and changing their liveliness based
on their current state and number of live neighbors. I create a clone of the game board first
so that I can check each cell based on their original state values, so that a cell doesn't update
by checking against another cell's new state instead of its original state. I find the number of live
neighbors by calling the countLiveNeighbor function, then checking the conditions for changing a cell's
liveliness before changing that value if the condtions are met. Finally, the aliveStable function checks
if the next state of the board is the same as the current state, meaining that the game has reached a stable
state. Once again, I create a clone of the board so that I can update the clone without changing the original
board. I then compare each value of the updated clone board to the original board, and if there is a difference,
that means that a change occurred and the board is not in a stable state. If all the values are the same, that means
that the board is in a stable state. 
*/

/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */
int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col)
{
	int ct = 0; // counter for number of live neighbors

	// row*boardColSize + col : how to access any given location in 2D array

	/* Imaginary grid
	1 2 3
	4 5 6
	7 8 9
	center square 5 is whose neighbors are being checked
	*/

	if (row - 1 >= 0) // checks top border (directly above 5) for validity
	{
		if (board[((row-1)*boardColSize) + col] == 1) // 2 is checked for liveliness
			ct++;
	}

	if (col + 1 < boardColSize) // checks right border (directly to right of 5) for validity
	{
		if (board[row*boardColSize + (col + 1)] == 1) // 6 is checked for liveliness
			ct++;
	}

	if (row - 1 >= 0 && col + 1 < boardColSize) // checks if top right corner valid
	{
		if (board[((row-1)*boardColSize) + (col + 1)] == 1) // 3 is checked for liveliness
			ct++;
	}

	if (row + 1 < boardRowSize) // checks bottom border (directly below 5) for validity
	{
		if (board[((row+1)*boardColSize) + col] == 1) // 8 is checked for liveliness
			ct++;
	}

	if (row + 1 < boardRowSize && col + 1 < boardColSize) // checks if bottom right corner valid
	{
		if (board[((row+1)*boardColSize) + (col + 1)] == 1) // 9 is checked for liveliness
			ct++;
	}

	if (col - 1 >= 0) // checks left border (directly to left of 5) for validity
	{
		if (board[row*boardColSize + (col - 1)] == 1) // 4 is checked for liveliness
			ct++;
	}

	if (row - 1 >= 0 && col - 1 >= 0) // checks if top left corner valid
	{
		if (board[((row-1)*boardColSize) + (col-1)] == 1) // 1 is checked for liveliness
			ct++;
	}

	if (row + 1 < boardRowSize && col - 1 >= 0) // checks if bottom left corner valid
	{
		if (board[((row+1)*boardColSize) + (col - 1)] == 1) // 7 is checked for liveliness
			ct++;
	}
	return ct; // return count of live neighbors
}


/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize)
{
	// creates a clone of board
	int clone[boardRowSize*boardColSize]; // creates array of same size as board
	for (int i = 0; i < boardRowSize*boardColSize; i++)
	{
		clone[i] = board[i]; // creates exact copy of board array in clone array
	}

	// nested for loops run through every entry in array
	for (int r = 0; r < boardRowSize; r++)
	{
		for (int c = 0; c < boardColSize; c++)
		{
			int num = countLiveNeighbor(clone, boardRowSize, boardColSize, r, c); // finds number of live neighbors
			int status = clone[r*boardColSize + c]; // finds status of current cell- if alive or dead
			if (status == 1 && (num > 3 || num < 2)) // conditions that change live cell into dead cell
			{
				board[r*boardColSize + c] = 0; // change cell to dead
			}
			else if (status == 0 && num == 3) // conditions that change dead cell into live cell
			{
				board[r*boardColSize + c] = 1; // change cell to live
			}
			else
			{
				// status of cell remains the same
			}
		}
	}

}


/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int* board, int boardRowSize, int boardColSize)
{
	// creates a clone of board
	int clone[boardRowSize*boardColSize]; // creates array of same size as board
	for (int i = 0; i < boardRowSize*boardColSize; i++)
	{
		clone[i] = board[i]; // creates exact copy of board array in clone array
	}

	updateBoard(clone, boardRowSize, boardColSize); // updates the clone array

	for (int j = 0; j < boardRowSize*boardColSize; j++) // runs iteratively through entire array
	{
		if (board[j] != clone[j]) // if exact cell in original doesn't match updated, then not stable state
			return 0; // indicates unstable state
	}

	return 1; // must be in stable state if every cell matches in original and updated boards
}
