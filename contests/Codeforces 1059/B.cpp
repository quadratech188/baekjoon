#include "CodeForces.h"
#include "FastIO2.h"
#include <algorithm>
#include <iostream>

void loop() {
	uint n;
	Fast::cin >> n;
	auto values = Fast::cin.to_vec<char>(n);

	for (uint k = 0; k < (1u << n); k++) {
		std::vector<char> remaining, cp;
		std::vector<uint> sub;
		char prev = '0';
		bool valid = true;
		for (uint i = 0; i < n; i++) {
			if ((1 << i) & k) {
				if (prev > values[i]) {
					valid = false;
					break;
				}
				prev = values[i];
				sub.push_back(i);
			}
			else {
				remaining.push_back(values[i]);
				cp.push_back(values[i]);
			}
		}
		if (!valid) continue;
		std::reverse(cp.begin(), cp.end());
		if (remaining == cp) {
			std::cout << sub.size() << '\n';

			for (auto x: sub) {
				std::cout << x + 1 << ' ';
			}
			std::cout << '\n';
			return;
		}
	}
	std::cout << "-1\n";
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
