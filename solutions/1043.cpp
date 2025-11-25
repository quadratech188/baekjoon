#include "FastIO2.h"
#include "UnionFind.h"
#include <algorithm>
#include <iostream>

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	UnionFind uf(n + 1);

	uint cnt;
	Fast::cin >> cnt;

	for (auto x: Fast::cin.to_range<uint>(cnt)) {
		uf.connect(0, x);
	}

	std::vector<uint> firsts;

	for (uint i = 0; i < m; i++) {
		uint first, cnt;
		Fast::cin >> cnt >> first;
		for (auto x: Fast::cin.to_range<uint>(cnt - 1)) {
			uf.connect(first, x);
		}
		firsts.push_back(first);
	}

	uint result = 0;
	for (auto first: firsts) {
		if (!uf.connected(0, first)) {
			result ++;
		}
	}
	std::cout << result << '\n';
}
