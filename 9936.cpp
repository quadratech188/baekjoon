#include "modules/Matrix.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <iostream>
#include <limits>

Matrix<int> board;

int cache[1000][8][1001];

int solve(int current_row, int available_last_rows, int remaining_dominoes) {
	if (remaining_dominoes <= 0) return 0;
	if (current_row == board.size().y) {
		if (remaining_dominoes == 0) return 0;
		return std::numeric_limits<int>::min() / 2;
	}

	if (cache[current_row][available_last_rows][remaining_dominoes] != -1)
		return cache[current_row][available_last_rows][remaining_dominoes];

	int result = std::numeric_limits<int>::min() / 2;
	for (int i = 0; i < 8; i++) {
		// If available_last_rows doesn't contain i, continue
		if ((available_last_rows | i) != available_last_rows) continue;

		int delta = 0;
		int count = 0;
		for (int k = 0; k < 3; k++) {
			if ((i & (1 << k)) != 0) {
				delta += board(k, current_row) + board(k, current_row - 1);
				count++;
			}
		}

		if (count > remaining_dominoes) continue;

		int max = std::numeric_limits<int>::min() / 2;
		max = std::max(max, solve(current_row + 1, 0b111 - i, remaining_dominoes - count));

		if (count < remaining_dominoes) {
			// Y Y N
			if ((i & 0b011) == 0) {
				max = std::max(max, solve(current_row + 1, (0b111 - i) & 0b100, remaining_dominoes - count - 1)
						+ board(0, current_row) + board(1, current_row));
			}
			// N Y Y
			if ((i & 0b110) == 0) {
				max = std::max(max, solve(current_row + 1, (0b111 - i) & 0b001, remaining_dominoes - count - 1)
						+ board(1, current_row) + board(2, current_row));
			}
		}
		result = std::max(result, delta + max);
	}

	cache[current_row][available_last_rows][remaining_dominoes] = result;

	return result;
}

int main() {
	FastIO();
	int n, k;
	std::cin >> n >> k;

	for (int a = 0; a < n; a++) {
		for (int b = 0; b < 8; b++) {
			for (int c = 0; c <= k; c++) {
				cache[a][b][c] = -1;
			}
		}
	}

	board = Matrix<int>(3, n);
	std::cin >> board;

	std::cout << solve(0, 0, k);
}
