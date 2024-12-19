#include "modules/Matrix.h"

bool solution(Matrix<char>& matrix, Range2::iterator it) {
	if (it == matrix.bounds().end())
		return true;

	Int2 pos = *it;

	if (matrix[pos] != '0') return solution(matrix, std::next(it));

	Int2 square(3 * (pos.x / 3), 3 * (pos.y / 3));

	for (char i = '1'; i <= '9'; i++) {
		bool available = true;

		for (int x = 0; x < 9; x++) {
			if (matrix[x][pos.y] == i) available = false;
		}
		if (!available) continue;

		for (int y = 0; y < 9; y++) {
			if (matrix[pos.x][y] == i) available = false;
		}
		if (!available) continue;

		for (Int2 t: Range2(square, square + Int2(3, 3))) {
			if (matrix[t] == i) available = false;
		}
		if (!available) continue;

		matrix[pos] = i;

		if (solution(matrix, std::next(it))) return true;

		matrix[pos] = '0';
	}
	return false;
}

int main() {
	Matrix<char> grid(9, 9);
	std::cin >> grid;

	solution(grid, grid.bounds().begin());

	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			std::cout << grid[i][j];
		}
		std::cout << '\n';
	}
}
