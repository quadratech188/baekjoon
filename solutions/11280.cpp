#include "FastIO2.h"
#include "ListGraph2.h"
#include "SSC.h"
#include "Bins.h"
#include <variant>

int main() {
	auto [n, m] = Fast::cin.get<uint, uint>();

	ListGraph<std::monostate, std::monostate> graph(2 * n);

	auto to_index = [n](int i) -> uint {
		if (i > 0) return i - 1;

		return n - i - 1;
	};

	for (uint _ = 0; _ < m; _++) {
		auto [i, j] = Fast::cin.get<int, int>();

		graph.connect(to_index(-i), to_index(j));
		graph.connect(to_index(-j), to_index(i));
	}

	IdBins ssc_groups(graph.size());

	SSC(graph, ssc_groups)();

	for (uint i = 0; i < n; i++) {
		if (ssc_groups[i] == ssc_groups[n + i]) {
			std::cout << 0;
			return 0;
		}
	}

	std::cout << 1;
	return 0;
}
