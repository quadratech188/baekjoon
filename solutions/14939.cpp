#include "FastIO2.h"
#include "Matrix.h"
#include <limits>

int main() {
	Matrix<char> grid(10, 10);
	Fast::cin >> grid;

	int answer = std::numeric_limits<int>::max();
	for (uint mask = 0; mask < 1024; mask++) {
		Matrix<char> grid_copy(10, 10);
		for (Int2 index: grid_copy.bounds())
			grid_copy[index] = (grid[index] == 'O');

		int cnt = 0;

		static Int2 offsets[] = {
			{0, -1},
			{-1, 0},
			{0, 0},
			{1, 0},
			{0, 1}
		};

		for (Int2 index: Range2({0, 0}, {10, 1})) {
			if ((mask & (1 << index.x)) == 0) continue;

			for (Int2 offset: offsets) {
				if (!grid.bounds().contains(index + offset)) continue;
				grid_copy[index + offset] ^= true;
			}
			cnt ++;
		}
		for (Int2 index: Range2({0, 1}, {10, 10})) {
			if (!grid_copy[index - Int2::j()]) continue;

			for (Int2 offset: offsets) {
				if (!grid.bounds().contains(index + offset)) continue;
				grid_copy[index + offset] ^= true;
			}
			cnt ++;
		}

		bool valid = true;
		for (int i = 0; i < 10; i++) {
			if (grid_copy(i, 9)) {
				valid = false;
				break;
			}
		}
		if (!valid) continue;
		answer = std::min(answer, cnt);
	}
	std::cout << (answer == std::numeric_limits<int>::max()? -1: answer);
}
