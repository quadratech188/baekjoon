#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include "modules/Math.h"
#include "modules/FastIO.h"
#include <iostream>

using m1e6_3 = m<1'000'003>;

int main() {
	FastIO();
	int n, s, e, t;
	std::cin >> n >> s >> e >> t;

	Matrix<m1e6_3> matrix(5 * n, 5 * n);

	for (int i = 0; i < n; i++) {
		for (int j = 1; j < 5; j++) {
			matrix(5 * i + j, 5 * i + j - 1) = 1;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			char temp;
			std::cin >> temp;
			int time = temp - '0';
			if (time == 0) continue;

			matrix(i * 5, j * 5 + time - 1) = 1;
		}
	}

	auto result = Math::power(matrix, t, matrix.identity());

	std::cout << result(5 * (s - 1), 5 * (e - 1));
}
