#include "CodeForces.h"
#include "FastIO2.h"
#include "UnionFind.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

void loop() {
	uint n, m;
	Fast::cin >> n >> m;

	auto values = Fast::cin.to_vec<int>(n);

	UnionFind uf(n);
	for (uint i = 0; i < m; i++) {
		uint l, r;
		Fast::cin >> l >> r;
		l --;

		for (uint p = l + 1; p < r; p++) {
			uf.connect(l, p);
		}
	}

	uf.flatten();

	std::map<uint, int> cnts;
	for (uint i = 0; i < n; i++) {
		cnts[uf.find(i)] ++;
	}
	std::vector<int> bags;
	for (auto [k, v]: cnts) {
		bags.push_back(v);
	}
	int total_cnt = std::accumulate(bags.begin(), bags.end(), 0);

	// bags: a_1 ... a_k
	// (selection) <= x + 1 && (!selection) = sum - selection <= n - x - 1
	// sum - n + x + 1 <= (selection) <= x + 1
	
	std::vector<char> possible(n + 1);
	possible[0] = true;
	for (int bag: bags) {
		for (uint i = 0; i <= n - bag; i++) {
			possible[i + bag] |= possible[i];
		}
	}
	for (uint i = 0; i < n; i++) {
		uint l = std::max(0, total_cnt - (int)n + (int)i + 1);
		uint r = std::min(i + 1, n);
		bool result = std::any_of(possible.begin() + l, possible.begin() + r, std::identity{});
		std::cout << (result? '1': '0');
	}
	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
