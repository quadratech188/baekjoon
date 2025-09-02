#include "FastIO2.h"
#include "ModInt.h"
#include "Math.h"
#include <iostream>

int main() {
	uint v;
	Fast::cin >> v;

	for (uint i = 0; i < v * v; i++) {
		int trash;
		Fast::cin >> trash;
	}

	sm32_1e9_7 n(v);

	std::cout << Math::C(n * 2, n) << ' ' <<  n * n;
}
