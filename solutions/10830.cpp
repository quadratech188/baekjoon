#include "Matrix.h"
#include "Math.h"
#include "FastIO2.h"
#include "ModInt.h"
#include <iostream>

using sm32_1e3 = sm32<1000>;

int main() {
	int n;
	int64_t b;
	Fast::cin >> n >> b;

	Matrix<sm32_1e3> m(n, n, Fast::cin.to_range<sm32_1e3>(n * n));

	std::cout << Math::power(m, b, m.identity());
}
