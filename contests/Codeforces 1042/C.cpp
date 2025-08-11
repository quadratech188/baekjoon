#include "CodeForces.h"
#include "FastIO2.h"
#include <algorithm>
#include <iostream>

void loop() {
	uint n;
	int k;
	Fast::cin >> n >> k;

	auto S = Fast::cin.to_vec<int>(n);

	for (auto& val: S) {
		val = val % k;
		if (2 * val > k) val = k - val;
	}
	std::ranges::sort(S);

	auto T = Fast::cin.to_vec<int>(n);

	for (auto& val: T) {
		val = val % k;
		if (2 * val > k) val = k - val;
	}
	std::ranges::sort(T);

	if (S == T)
		std::cout << "YES\n";
	else
	 	std::cout << "NO\n";
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
