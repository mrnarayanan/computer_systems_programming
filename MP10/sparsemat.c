#include "sparsemat.h"
#include <stdio.h>
#include <stdlib.h>
/*
Murugan Narayanan
MP 10
This program performs operations on sparse matrices. The addNode function is a helper
function that creates a new tuple. The load_tuples function creates a matrix from tuples
stored in a file. The gv_tuples function returns the value of a specified matrix. The bulk
of the program is the recursive function that sets the tuples and their values. 
*/

// adds a node (helper function)
sp_tuples_node* addNode(sp_tuples_node* prev, int row, int col, double val)
{
	sp_tuples_node* ptr = malloc(sizeof(sp_tuples_node));
	ptr->value = val;
	ptr->row = row;
	ptr->col = col;
	ptr->next = NULL;
	if (prev != NULL)
		prev->next = ptr;
	return ptr;
}

// loads matrices from file
sp_tuples * load_tuples(char* input_file)
{
    FILE* file;
    if (fopen(input_file, "r") == NULL)
      printf("Invalid file.");
    else
      file = fopen(input_file, "r");

    sp_tuples* matrix = malloc(sizeof(sp_tuples));
    fscanf(file, "%d %d\n", &(matrix->m), &(matrix->n));
    int a, b;
    double c;
		matrix->tuples_head = NULL;
		matrix->nz = 0;
    while (fscanf(file, "%d %d %lf\n", &a, &b, &c) == 3)
    {
			set_tuples(matrix, a, b, c);
    }
    fclose(file);
    return matrix;
}

// returns value of specified tuple
double gv_tuples(sp_tuples * mat_t,int row,int col)
{
  sp_tuples_node* current = mat_t->tuples_head;
  if (row >= mat_t->m || col >= mat_t->n || row < 0 || col < 0)
    return 0;

  while (current != NULL)
  {
    if(current->row == row && current->col == col)
      return current->value;
    else
      current = current->next;
  }
  return 0;
}


void recursive_add_tuple(sp_tuples_node *current, sp_tuples_node *prev, sp_tuples_node *to_add, sp_tuples *main){
  if(current == NULL){
    // If we have reached the end of the array, add it here
    if(to_add->value == 0){
      // If value is 0, then free the tuple and return
      free(to_add);
      return;
    }
    if(prev == NULL){
      // If this is the first element, the change the head pointer
      main->tuples_head = to_add;
      main->nz++;
      return;
    }
    else{
      // Otherwise change the previous pointer's next element to point to this
      main->nz++;
      prev->next = to_add;
      return;
    }
  }
  else if(current->row < to_add->row){
    // Too early to add, recurse again
    return recursive_add_tuple(current->next, current, to_add, main);
  }
  else if(current->row == to_add->row){
    if(current->col < to_add->col){
      // Too early to add, recurse again
      return recursive_add_tuple(current->next, current, to_add, main);
    }
    else if(current->col == to_add->col){
      // The element already exists, update the value
      if(to_add->value == 0){
        // If new value is 0, clear it
        if(prev != NULL)
          prev->next = current->next;
        else
          main->tuples_head = current->next;
        free(current);
        free(to_add);
        main->nz--;
        return;
      }
      else{
        // Otherwise update the value and free the tuple
        current->value = to_add->value;
        free(to_add);
        return;
      }
    }
    else{
      // We found the right location to put it
      if(to_add->value == 0){
        // If the value is 0, don't add the tuple in
        free(to_add);
        return;
      }
      // Add the node before current, and update prev
      to_add->next = current;
      if(prev != NULL)
        prev->next = to_add;
      else
        main->tuples_head = to_add;
      main->nz++;
      return;
    }
  }
  else{
    // We found the right location to put it
    if(to_add->value == 0){
      // If the value is 0, don't add it in
      free(to_add);
      return;
    }
    // Add the node before current, and update prev
    to_add->next = current;
    if(prev != NULL)
      prev->next = to_add;
    else
      main->tuples_head = to_add;
    main->nz++;
    return;
  }
}


void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
	// Call the recursive helper method
	sp_tuples_node* append = addNode(NULL,row, col, value);
	recursive_add_tuple(mat_t->tuples_head, NULL, append, mat_t);
  return;
}

// writes output matrices to file
void save_tuples(char * file_name, sp_tuples * mat_t)
{
  FILE* file = fopen(file_name, "w");
  fprintf(file, "%d %d\n", mat_t->m, mat_t->n);
  sp_tuples_node* ptr = mat_t->tuples_head;
  while (ptr != NULL)
  {
    fprintf(file, "%d %d %lf\n", ptr->row, ptr->col, ptr->value);
    ptr = ptr->next;
  }
  fclose(file);
	return;
}

// computes sum of two matrices
sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB)
{
  if(matA->m != matB->m || matA->n != matB->n)
    return NULL;
  sp_tuples* retmat = malloc(sizeof(sp_tuples));

	// init values in return matrix
  retmat->m = matA->m;
  retmat->n = matA->n;
	retmat->nz = 0;
	retmat->tuples_head = NULL;


	// traverse matA
	sp_tuples_node* pA = matA->tuples_head;
	while (pA != NULL)
	{
		if (pA->value != 0)
		{
			int rr = pA->row;
			int cc = pA->col;
			double ans = gv_tuples(matA, rr, cc);
			set_tuples(retmat, rr, cc, ans);
		}
		pA = pA->next; // moves to next node
	}
	// traverse matB
	sp_tuples_node* pB = matB->tuples_head;
	while (pB != NULL)
	{
		if (pB->value != 0)
		{
			int rrr = pB->row;
			int ccc = pB->col;
			double answ = gv_tuples(retmat, rrr, ccc) + gv_tuples(matB, rrr, ccc); // computes sum
			set_tuples(retmat, rrr, ccc, answ);
		}
		pB = pB->next; // moves to next node
	}

	return retmat;
}

// multiplies two matrices
sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB)
{
  if(matA->n != matB->m)
    return NULL;
  sp_tuples* retmat = malloc(sizeof(sp_tuples));

	// init values for return matrix
  retmat->m = matA->m;
  retmat->n = matB->n;
	retmat->nz = 0;
	retmat->tuples_head = NULL;

	// traverse matA
	sp_tuples_node* pA = matA->tuples_head;
	while (pA != NULL)
	{
		if (pA->value != 0)
		{
			int rr = pA->row;
			int cc = pA->col;
			// traverse matB
			sp_tuples_node* pB = matB->tuples_head;
			while (pB != NULL)
			{
				if (pB->value != 0 && pB->row == cc)
				{
					// compute product
					double res = gv_tuples(retmat, rr, pB->col) + gv_tuples(matA, rr, cc) * gv_tuples(matB, pB->row, pB->col);
					set_tuples(retmat, rr, pB->col, res);
				}
				pB = pB->next; // move to next node
			}
		}
		pA = pA->next; // move to next node
	}

  return retmat;
}


// frees memory used for matrix and tuple
void destroy_tuples(sp_tuples * mat_t)
{
    sp_tuples_node* ptr = mat_t->tuples_head;
    while (ptr != NULL)
    {
			// free each node while saving pointer to go to next one
      sp_tuples_node* next = ptr->next;
      free(ptr);
      ptr = next;
    }
    free(mat_t); // frees matrix
    return;
}
