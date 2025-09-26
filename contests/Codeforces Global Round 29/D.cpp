#include "CodeForces.h"
#include "FastIO2.h"
#include <algorithm>
#include <iostream>
#include <map>

void loop() {
	uint n;
	Fast::cin >> n;

	std::map<int, uint> counts;

	for (int a: Fast::cin.to_range<int>(n)) {
		counts[a] ++;
	}

	std::vector<std::pair<uint, int>> pairs;

	for (auto [key, count]: counts) {
		pairs.emplace_back(count, key);
	}

	std::ranges::sort(pairs, std::greater{});

	int64_t alice_points = 0;
	int64_t bob_points = 0;
	bool bobs_turn = false;

	for (auto [count, v]: pairs) {
		if (v % 2 == 0) {
			alice_points += ((int64_t)v / 2) * count;
			bob_points += ((int64_t)v / 2) * count;
		}
		else {
			if (bobs_turn) {
				bob_points += ((int64_t)v / 2 + 1) * count;
				alice_points += ((int64_t)v / 2) * count;
				bobs_turn = false;
			}
			else {
				alice_points += ((int64_t)v / 2 + 1) * count;
				bob_points += ((int64_t)v / 2) * count;
				bobs_turn = true;
			}
		}
	}

	std::cout << alice_points << ' ' << bob_points << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
