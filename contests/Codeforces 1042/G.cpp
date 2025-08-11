#include "CodeForces.h"
#include "FastIO2.h"
#include "ModInt.h"
#include "Math.h"
#include <algorithm>
#include <iostream>

void loop() {
	uint n, k;
	Fast::cin >> n >> k;

	auto values = Fast::cin.to_vec<uint>(n);
	std::ranges::sort(values);

	sm32_1e9_7 score = 1;

	uint counter = k;
	for (auto& base: values) {
		if (base >= 32 || (1u << (base - 1)) >= counter) { // We end here
			score *= base;
			counter --;

			uint64_t power = 1;
			for (uint i = 0; i < std::min(base, 32u); i++) {
				uint64_t times_used = (counter + power) / (2 * power);
				score *= Math::power(sm32_1e9_7(i + 1), times_used);

				power *= 2;
			}

			std::cout << score << '\n';
			return;
		}
		else {
			score *= base;

			if (base >= 2) {
				sm32_1e9_7 extra = 1;
				for (uint i = 1; i < base; i++) {
					extra *= extra;
					extra *= i;
				}
				score *= extra;
			}

			counter -= 1 << (base - 1);
		}
	}
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
