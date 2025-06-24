#include "../modules/FastIO2.h"
#include "../modules/Matrix.h"
#include <algorithm>

int main() {
	int n, m;
	Fast::cin >> n >> m;

	Matrix<char> grid(m, n);
	Fast::cin >> grid;

	int k;
	Fast::cin >> k;

	int max = 0;
	std::vector<bool> flipped(grid.size().x);
	for (int row = 0; row < grid.size().y; ++row) {
		std::fill(flipped.begin(), flipped.end(), false);
		int flip_count = 0;
		for (int column = 0; column < grid.size().x; ++column) {
			if (grid(column, row) == '0') {
				flipped[column] = true;
				++flip_count;
			}
		}

		if (flip_count > k || (k - flip_count) % 2 != 0) continue;

		int cnt = 0;
		for (int other_row = 0; other_row < grid.size().y; ++other_row) {
			bool flag = true;
			for (int column = 0; column < grid.size().x; ++column) {
				if ((grid(column, other_row) == '1') ^ (flipped[column])) {
					// Continue
				}
				else {
					flag = false;
					break;
				}
			}
			if (flag) ++cnt;
		}

		max = std::max(max, cnt);
	}
	
	std::cout << max;
}
