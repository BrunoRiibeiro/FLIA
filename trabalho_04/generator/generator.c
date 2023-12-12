#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "sudoku.h"

// Macros to manipulate bits:
#define ALL_BITS 1022 // 1022 = 1111111110 binary
#define SET_BIT(num, bit)  (num |= (1<<(bit)))
#define CHECK_BIT(num, bit) (num & (1<<(bit)))

static int is_solved;
int solved_sudoku[SIZE][SIZE]=
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

int count_set_bits(int num) {
	int count = 0;
	for (int i = 0; i < 32; i++)
		if (CHECK_BIT(num, i)) count++;
	return count;
}

void display_sudoku(const char *filename) {
	FILE *file = fopen(filename, "w");
	if (!filename) {fprintf(stderr, "Error to open file %s\n", filename); return;}
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++)
			fprintf(file, "%d", sudoku[i][j]);
		fprintf(file, "\n");
    }
	fclose(file);
}

int solve_sudoku(int level, int x, int y, int val) {
	int best_x = -1, best_y, best_mask, best_bits = 0, bits;
	if (is_solved) return 0;
	if (val >= 1 && val <= 9) sudoku[x][y] = val; // Make the move
	is_solved = 1;
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			// For each empty cell on the board
			if (sudoku[i][j] == 0) {
				int mask = 0;
				is_solved = 0;
				// Check row
				for (int k = 0; k < SIZE; k++)
					if (sudoku[i][k] != 0)
						SET_BIT(mask, sudoku[i][k]); // Set the bits representing impossible numbers
				// Check column
				for (int k = 0; k < SIZE; k++)
					if (sudoku[k][j] != 0)
						SET_BIT(mask, sudoku[k][j]); // Set the bits representing impossible numbers
				// Check box
				int box_i = (i/3)*3, box_j = (j/3)*3;
				for (int k = box_i; k < box_i+3; k++)
					for (int m = box_j; m < box_j+3; m++)
						if (sudoku[k][m] != 0)
							SET_BIT(mask, sudoku[k][m]); // Set the bits representing impossible numbers
				if (mask == ALL_BITS) goto unsolvable;
				if ((bits = count_set_bits(mask)) > best_bits)
					best_bits = bits, best_x = i, best_y = j, best_mask = mask;
			}
	if (is_solved) return 0;
	// Make the best move found...
	if (best_x != -1)
		for (int k = 1; k <= 9; k++)
			if (CHECK_BIT(best_mask, k) == 0)
				solve_sudoku(level+1, best_x, best_y, k);
unsolvable:
	if (val >= 1 && val <= 9)
		sudoku[x][y] = 0; // Undo the move
	return 1;
}

void generate_sudoku() {
	int k = 0, level;
	char letter;
	printf("Enter the level of the sudoku: (0-easy 1-medium 2-hard 3-crazy)\n");
	scanf("%c", &letter);
	switch (letter) {
		case '0': level = 51; break;
		case '1': level = 41; break;
		case '2': level = 31; break;
		case '3': level = 23; break;
		default: level = 31; break; 
	} do {
		memset(sudoku, 0, sizeof(int)*SIZE*SIZE);    
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				if (rand() % 100 < level+k)
					sudoku[i][j] = solved_sudoku[i][j];
		k++;
	} while (solve_sudoku(0, 0, 0, 0));
	display_sudoku("sudoku");
}

void initialize_sudoku() {
	int index = rand() % NUM_SUDOKUS;
	switch (index) {
		case 0:
			memcpy(solved_sudoku, solved_sudoku0, sizeof(solved_sudoku)); break;
		case 1:
			memcpy(solved_sudoku, solved_sudoku1, sizeof(solved_sudoku)); break;
		case 2:
			memcpy(solved_sudoku, solved_sudoku2, sizeof(solved_sudoku)); break;
		case 3:
			memcpy(solved_sudoku, solved_sudoku3, sizeof(solved_sudoku)); break;
		case 4:
			memcpy(solved_sudoku, solved_sudoku4, sizeof(solved_sudoku)); break;
		case 5:
			memcpy(solved_sudoku, solved_sudoku5, sizeof(solved_sudoku)); break;
		case 6:
			memcpy(solved_sudoku, solved_sudoku6, sizeof(solved_sudoku)); break;
		case 7:
			memcpy(solved_sudoku, solved_sudoku7, sizeof(solved_sudoku)); break;
		case 8:
			memcpy(solved_sudoku, solved_sudoku8, sizeof(solved_sudoku)); break;
		case 9:
			memcpy(solved_sudoku, solved_sudoku9, sizeof(solved_sudoku)); break;
		default:
			fprintf(stderr, "Error: Invalid sudoku index\n");
			exit(EXIT_FAILURE);
	}
}

int main() {
	srand(time(NULL));
	initialize_sudoku();
	generate_sudoku();
	return 0;
}
