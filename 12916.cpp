#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include "modules/Math.h"
#include "modules/FastIO.h"
#include <iostream>

int main() {
	FastIO();
	mInt::setMod(1'000'000'007);

	int n, k;
	std::cin >> n >> k;
	Matrix<mInt> matrix(n, n);
	std::cin >> matrix;

	Matrix<mInt> result = Math::power(matrix, k);

	mInt sum = 0;
	for (Int2 index: result.bounds())
		sum += result[index];

	std::cout << sum;
}
