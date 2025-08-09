#include "FastIO.h"
#include "ListGraph2.h"
#include "SSC.h"
#include "Bins.h"
#include <iostream>
#include <variant>

bool loop() {
	FastIO();
	uint n, m;
	std::cin >> n >> m;

	if (std::cin.eof()) return false;

	ListGraph<std::monostate, std::monostate> graph(2 * n);

	auto to_index = [n](int i) -> uint {
		if (i > 0) return i - 1;

		return n - i - 1;
	};

	for (uint _ = 0; _ < m; _++) {
		int i, j;
		std::cin >> i >> j;

		graph.connect(to_index(-i), to_index(j));
		graph.connect(to_index(-j), to_index(i));
	}
	graph.connect(n, 0);

	IdBins ssc_groups(graph.size());

	SSC(graph, ssc_groups)();

	for (uint i = 0; i < n; i++) {
		if (ssc_groups[i] == ssc_groups[n + i]) {
			std::cout << "no\n";
			return true;
		}
	}
	std::cout << "yes\n";
	return true;
}

int main() {
	while (loop()) {}
}
