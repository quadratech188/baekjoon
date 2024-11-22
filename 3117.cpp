#include "modules/SparseTable.h"
#include <iostream>

int main() {
	int n, k, m;
	std::cin >> n >> k >> m;

	std::vector<int> initial;
	initial.reserve(n);

	for (int i = 0; i < n; i++) {
		int temp;
		std::cin >> temp;
		initial.push_back(temp - 1);
	}

	std::vector<int> lookup;
	lookup.reserve(n);

	for (int i = 0; i < k; i++) {
		int temp;
		std::cin >> temp;
		lookup.push_back(temp - 1);
	}

	SparseTable table(k, m, [&lookup](size_t n) {
			return lookup[n];
			});

	for (auto a: initial) {
		std::cout << table.after(a, m - 1) + 1 << ' ';
	}
}
