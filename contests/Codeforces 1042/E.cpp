#include "CodeForces.h"
#include "FastIO2.h"
#include <iostream>

bool loop() {
	uint n;
	Fast::cin >> n;

	auto a = Fast::cin.to_vec<int>(n);
	auto b = Fast::cin.to_vec<int>(n);

	if (a[n - 1] != b[n - 1]) return false;

	for (uint i = 0; i < n - 1; i++) {
		if (b[i] != a[i] && b[i] != (a[i] ^ a[i + 1]) && b[i] != (a[i] ^ b[i + 1]))
			return false;
	}

	return true;
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) {
		if (loop())
			std::cout << "YES\n";
		else
			std::cout << "NO\n";
	}
}
