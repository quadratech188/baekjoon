#include "FastIO2.h"
#include "Matrix.h"

enum Cell {
	WIN,
	LOSE,
	INVALID
};

Matrix<char> grid;
Matrix<Cell> cache;

int n, m, k;

Cell solve(Int2 index) {
	if (!grid.bounds().contains(index)) return WIN; // Not really
	if (grid[index] == '#') return WIN;

	Cell& result = cache[index];
	if (result != INVALID)
		return result;

	if (solve(index + Int2::i()) == LOSE) {
		result = WIN;
		return result;
	}
	if (solve(index + Int2::j()) == LOSE) {
		result = WIN;
		return result;
	}

	for (int delta = 1; delta <= k; delta++) {
		Int2 new_index = index + Int2::one() * delta;
		if (!grid.bounds().contains(new_index)) break;

		if (solve(new_index) == LOSE) {
			result = WIN;
			return result;
		}
	}
	result = LOSE;
	return result;
}

int main() {
	Fast::cin >> n >> m >> k;

	grid = Matrix<char>(n, m);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			Fast::cin >> grid(i, j);
		}
	}

	cache = Matrix<Cell>(n, m, INVALID);

	uint q;
	Fast::cin >> q;

	for (Int2 query: Fast::cin.to_range<Int2>(q)) {
		std::cout << (solve(query - Int2::one()) == WIN? "First": "Second") << '\n';
	}
}
