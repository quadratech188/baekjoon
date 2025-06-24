#include "../modules/Matrix.h"
#include "../modules/FastIO2.h"

int main() {
	size_t n, m;
	Fast::cin >> n >> m;
	Matrix<char> grid(m, n);
	Fast::cin >> grid;

	int max = -1;

	for (Int2 delta: Range2(Int2(-m + 1, -n + 1), Int2(m, n))) {
		for (Int2 start: grid.bounds()) {
			int value = 0;
			for (int i = 0; i <= 9; i++) {
				if (!grid.bounds().contains(start + delta * i)) break;
				value = 10 * value + grid[start + delta * i] - '0';

				int sqrt = std::round(std::sqrt(value));
				if (sqrt * sqrt == value)
					max = std::max(max, value);
			}
		}
	}

	std::cout << max;
}
