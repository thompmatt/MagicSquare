///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2021, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
   
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      N/A
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
// 
// Author:           Matthew Thompson Soto
// Email:            mathompson23@wisc.edu
// CS Login:         thompson-soto
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
//					 - Sambhav Satija: Helped solve free() issue. 
//					   CS 354 Teaching Assistant. Email: ssatija@wisc.edu.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
	int squareSize = 0;
	printf("Enter magic square's size (odd integer >=3)\n"); // Prompts user input
	scanf("%d", &squareSize); // Reads in number
	
	if (squareSize % 2 == 0) { // Checks if size is not odd
		printf("Magic square size must be odd.\n");
		exit(1);
	} else if (squareSize < 3) { // Checks if size is less than 3
		printf("Magic square size must be >= 3.\n");
		exit(1);
	}	

	return squareSize;
} 
   
/* Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {
	// Allocate heap memory for a MagicSquare pointer (will be returned)
    MagicSquare *square = malloc(sizeof(MagicSquare));
	
	if (square == NULL) { // Checks return statement of previous malloc
		printf("Failed to use malloc() on *square, returned NULL\n");
	}
	square -> size = n; // Sets square's size value to n
	
	// Dynamically allocate a 2D array for given size
	int **arr = malloc(sizeof(int *) * n);
	
	// Checks return statement of previous malloc
	if (arr == NULL) {
		printf("Failed to use malloc() on **arr, returned NULL\n");
		exit(1);
	}

	// Allocate a row of n ints to arr's pointers
	for (int i = 0; i < n; i++) {
		*(arr + i) = malloc(sizeof(int) * n);
		
		// Check the return value of previous malloc
		if (*(arr + i) == NULL) {
			printf("Failed to use malloc() on *(arr + i), returned NULL\n");
			exit(1);
		}
		
		for (int j = 0; j < n; j++) { // Rows become arrays of all zeroes
			*(*(arr + i) + j) = 0;
		}
	}
	
	square -> magic_square = arr; // Sets value of magic_square to arr
	int num = 1; // Number to be placed in magic square
	int row = ((n + 1) / 2) - 1; // Index to start in middle row
	int col = n - 1; // Index to start in last column

	// Perform the Alternate Siamese Method algorithm. Moves down a row and
	// rightwards a column after each iteration. If a numeber (not 0) is
	// already at location, we move back a row and move rightwards a column.
	// Iterates n^2 times (n x n matrix)
	while (num <= (n * n)) {
		// Checks if there is a number already placed at location row, col
		if ((*(*((square -> magic_square) + row) + col))) {
			if (!row) { // If row is equal to 0, set new index to n - 1
				row = n - 1;
			} else {
				row--;
			}
			
			if (col + 1 == n) { // If col is n - 1, set new index to 0
				col = 0;
			} else {
				col++;
			}
		}

		*(*((square -> magic_square) + row) + col) = num; // Sets num at row, col
		num++;
		
		if (row + 1 == n) { // Evaluates next index for row (downwards)
			row = 0;
		} else {
			row++;
		}

		if (col + 1 == n) { // Evaluates next index for col (rightwards)
			col = 0;
		} else {
			col++;
		}
	} 	

	return square;   
} 

/* Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	FILE *fp = fopen(filename, "w+"); // Open the file with specified filename
	if (fp == NULL) { // Checks fopen return value
		printf("Can't open specified file for writing.\n");
		exit(1);
	}
	
	fprintf(fp, "%d\n", magic_square -> size); // Write the magic square's size
	
	// Writes the magic square to the file
	for (int i = 0; i < magic_square -> size; i++) {
		for (int j = 0; j < magic_square -> size; j++) {
			// Prints number at row i, column j
			fprintf(fp, "%d", *(*(magic_square -> magic_square + i) + j));
			
			// Checks if it is last number, to avoid adding extra comma
			if (j + 1 == magic_square -> size) {
				continue;
			} else {
				fprintf(fp, ",");	
			}
		}
		fprintf(fp, "\n");
	}	
	
	// Closes file and checks fclose() return value
	if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
}

/* Generates a magic square of the user specified size and
 * output the square to the output filename
 */
int main(int argc, char *argv[]) {
    // Checks input arguments to get output filename
	if (argc != 2) {
		printf("Usage: ./myMagicSquare <output_filename>\n");
		exit(1);
	}

	int squareSize = getSize(); // Gets magic square's size from user input
	MagicSquare *square = generateMagicSquare(squareSize); // Generate magic square
	fileOutputMagicSquare(square, *(argv + 1)); // Output the magic square to a new file

    // First frees the dynamically allocated memory of inner pointers of magic_square
	for (int i = 0; i < square -> size; i++) {
		free(*(square -> magic_square + i));
		*(square -> magic_square + i) = NULL;
	}

	free(square -> magic_square); // Free the dynamically allocated memory for magic_square
	square -> magic_square = NULL;
	
	free(square); // Free allocated memory for MagicSquare pointer
	square = NULL;	

	return 0;
} 






                                                         
//		s21		myMagicSquare.c

