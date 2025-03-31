#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include "modules/Math.h"
#include "modules/FastIO.h"
#include <iostream>

int main() {
	FastIO();

	int n, k;
	std::cin >> n >> k;
	Matrix<mL1e9_7> matrix(n, n);
	std::cin >> matrix;

	Matrix<mL1e9_7> result = Math::power(matrix, k, Matrix<mL1e9_7>::identity(n));

	std::cout << result.sum();
}
