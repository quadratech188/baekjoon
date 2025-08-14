#include "FastIO2.h"
#include "Matrix.h"
#include "ModInt.h"
#include <iostream>

int main() {
	uint t, n, d;
	Fast::cin >> t >> n >> d;

	std::vector<Matrix<sm32_1e9_7>> maps(t, Matrix<sm32_1e9_7>(n, n, 0));

	for (uint i = 0; i < t; i ++) {
		uint m;
		Fast::cin >> m;
		for (uint _ = 0; _ < m; _ ++) {
			uint a, b, c;
			Fast::cin >> a >> b >> c;
			maps[i](b - 1, a - 1) = c;
		}
	}
	
	auto remainder = Matrix<sm32_1e9_7>::identity(n);
	for (uint i = 0; i < d % t; i++)
		remainder = remainder * maps[i];

	auto total = remainder;
	for (uint i = d % t; i < t; i++)
		total = total * maps[i];

	auto result = Math::power(total, d / t, Matrix<sm32_1e9_7>::identity(n)) * remainder;

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < n; j++)
			std::cout << result(j, i) << ' ';
		std::cout << '\n';
	}
}
