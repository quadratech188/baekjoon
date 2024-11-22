#include <iostream>
#include "modules/SparseTable.h"
#include "modules/FastIO.h"

int main() {
	FastIO();
	int m;
	std::cin >> m;

	std::vector<int> lookup;
	lookup.reserve(m);
	for (int i = 0; i < m; i++) {
		int temp;
		std::cin >> temp;
		lookup.push_back(temp - 1);
	}

	SparseTable table(m, 500000, [&lookup](size_t index) {
			return lookup[index];
			});

	int q;
	std::cin >> q;

	for (int i = 0; i < q; i++) {
		int n, x;
		std::cin >> n >> x;
		std::cout << table.after(x - 1, n) + 1 << '\n';
	}
}
