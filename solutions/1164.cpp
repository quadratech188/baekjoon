#include "modules/Matrix.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <iostream>
#include <array>

std::array<Matrix<char>, 2> pieces;
std::array<int, 2> counts;

bool solution(Matrix<char>& grid, int count, int max) {
	if (count == max) return true;

	Int2 min_blank = grid.size();
	for (Int2 index: grid.bounds()) {
		if (grid[index] == 'X') {
			min_blank = index;
			break;
		}
	}
	
	for (int i = 0; i < 2; i++) {
		Matrix<char>& piece = pieces[i];
		if (min_blank.x + piece.size().x > grid.size().x || min_blank.y + piece.size().y > grid.size().y) continue;

		if (std::any_of(piece.bounds().begin(), piece.bounds().end(), [&piece, &grid, min_blank](Int2 index) {
					return piece[index] != 0 && grid[index + min_blank] != 'X';
					})) continue;

		for (Int2 index: piece.bounds()) {
			if (piece[index])
				grid[index + min_blank] = piece[index];
		}

		if (solution(grid, count + counts[i], max)) return true;

		for (Int2 index: piece.bounds()) {
			if (piece[index])
				grid[index + min_blank] = 'X';
		}
	}

	return false;
}

int main() {
	FastIO();
	
	pieces[0] = Matrix<char>(4, 2);
	pieces[0](0, 0) = 'A';
	pieces[0](0, 1) = 'A';
	pieces[0](1, 1) = 'A';
	pieces[0](2, 1) = 'A';
	pieces[0](3, 1) = 'A';
	pieces[0](3, 0) = 'A';

	pieces[1] = Matrix<char>(2, 1, 'B');

	counts[0] = 6;
	counts[1] = 2;

	int n, m;
	std::cin >> n >> m;
	Matrix<char> grid(m, n);

	std::cin >> grid;

	int count;
	for (Int2 index: grid.bounds())
		if (grid[index] == 'X') count++;

	if (solution(grid, 0, count)) {
		for (int y = 0; y < grid.size().y; y++) {
			for (int x = 0; x < grid.size().x; x++) {
				std::cout << grid(x, y);
			}
			std::cout << '\n';
		}
	}
	else {
		std::cout << -1;
	}
}
