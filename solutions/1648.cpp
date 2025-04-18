#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include <iostream>
#include <optional>

using m_9901 = m<9901>;

Matrix<std::optional<m_9901>> cache;

m_9901 solve(int n, int m, int current_row, unsigned int available_last_rows, int ptr) {
	if (current_row == n) {
		if (available_last_rows == 0) return 1;
		return 0;
	}

	m_9901 result = 0;
}

int main() {
	int n, m;
	std::cin >> n >> m;

	cache = Matrix<std::optional<m_9901>>(1 << n, m);

	std::cout << solve(n, m, 0, 0, 0);
}
