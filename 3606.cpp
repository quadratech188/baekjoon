#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include "modules/Math.h"
#include "modules/FastIO.h"
#include <iostream>

int main() {
	// FastIO();
	int n, m, d, k;
	std::cin >> n >> m >> d >> k;

	Matrix<ModInt> initial(n, 1, ModInt(m));
	std::cin >> initial;

	Matrix<ModInt> multiplier(n, n, ModInt(m));

	for (int i = 0; i < n; i++) {
		for (int offset = -d; offset <= d; offset++) {
			multiplier[i][ModInt(i + offset, n)] = 1;
		}
	}

	Matrix<ModInt> result = initial * Math::power(multiplier, k);

	for (Int2 index: result.bounds())
		std::cout << result[index] << ' ';
}
