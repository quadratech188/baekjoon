#include "CodeForces.h"
#include "FastIO2.h"
#include <algorithm>
#include <functional>
#include <iostream>

void loop() {
	uint n, k;
	Fast::cin >> n >> k;

	auto values = Fast::cin.to_vec<int>(n);

	std::vector<bool> contained(n);
	for (auto v: values) {
		contained[v - 1] = true;
	}
	if (std::ranges::all_of(contained, std::identity{})) {
		for (uint i = 0; i < k; i++) {
			std::cout << values[i] << ' ';
		}
		std::cout << '\n';
		return;
	}

	int unused = std::ranges::find(contained, false) - contained.begin();
	std::cout << unused + 1 << ' ';
	if (k == 1) {
		std::cout << '\n';
		return;
	}

	if (k == 2) {
		for (int i = 0; i < n; i++) {
			if (i != unused && i != values.back() - 1) {
				std::cout << i + 1 << '\n';
				break;
			}
		}
		return;
	}

	std::vector<int> result;

	int ptr = n - 1;
	for (uint i = 0; i < k - 1; i++) {
		if (unused == ptr) ptr --;
		result.push_back(ptr);
		ptr --;
	}

	if (result.front() == values.back() - 1) {
		std::swap(result[0], result[1]);
	}
	for (auto x: result) {
		std::cout << x + 1 << ' ';
	}
	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
