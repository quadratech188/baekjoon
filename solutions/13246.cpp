#include "modules/Math.h"
#include "modules/Matrix.h"
#include "modules/ModInt.h"
#include "modules/FastIO.h"
#include <cmath>
#include <iostream>

Matrix<mInt> solution(Matrix<mInt> const& data, int const exponent) {
	if (exponent == 1) return data;

	if (exponent % 2 == 1) {
		auto power = Math::power(data, exponent, data.identity());
		auto sum = solution(data, exponent - 1);
		return sum + power;
	}
	else {
		auto power = Math::power(data, exponent / 2, data.identity());
		power += data.identity();

		auto sum = solution(data, exponent / 2);

		return sum * power;
	}
}

int main() {
	FastIO();
	int n, b;
	std::cin >> n >> b;

	mInt::setMod(1000);

	Matrix<mInt> a(n, n);
	std::cin >> a;

	std::cout << solution(a, b);
}
