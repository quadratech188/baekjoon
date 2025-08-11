#include "CodeForces.h"

#include "FastIO2.h"
#include <iostream>

void loop() {
	uint n;
	Fast::cin >> n;

	auto a = Fast::cin.to_vec<int>(n);
	auto b = Fast::cin.to_vec<int>(n);

	int64_t sum =  1;

	for (uint i = 0; i < n; i++) {
		if (a[i] > b[i])
			sum += a[i] - b[i];
	}

	std::cout << sum << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
