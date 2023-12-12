#include <stdio.h>

#define SIZE 9
// Macros to manipulate bits:
#define ALL_BITS 1022 // 1022 = 1111111110 binary
#define SET_BIT(num, bit)  (num |= (1<<(bit)))
#define CHECK_BIT(num, bit) (num & (1<<(bit)))

static int is_solved;
int sudoku[SIZE][SIZE]=
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
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++)
			fprintf(file, "%d", sudoku[i][j]);
		fprintf(file, "\n");
	}
	fclose(file);
}

int solve_sudoku(int x, int y, int val) {
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
	if (is_solved) {
		display_sudoku("solved_sudoku");
		return 0;
	}
	// Make the best move found...
	if (best_x != -1)
		for (int k = 1; k <= 9; k++)
			if (CHECK_BIT(best_mask, k) == 0)
				solve_sudoku(best_x, best_y, k);
unsolvable:
	if (val >= 1 && val <= 9)
		sudoku[x][y] = 0; // Undo the move
	return 1;
}

int main() {
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			scanf("%1d", &sudoku[i][j]);
	solve_sudoku(0, 0, 0);
	return 0;
}
