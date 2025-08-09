#include "FastIO2.h"
#include "ListGraph2.h"
#include "SSC.h"
#include "Bins.h"
#include <algorithm>
#include <limits>
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

	SetBins<uint> ssc_groups;

	SSC(graph, ssc_groups)();

	std::vector<uint> assigned(n, std::numeric_limits<uint>::max());
	std::vector<bool> results(n);

	for (uint i = 0; i < ssc_groups.bins.size(); i++) {
		auto& bin = ssc_groups.bins[i];
		for (auto v: bin) {
			if (v < n) {
				if (assigned[v] == i) {
					std::cout << '0';
					return 0;
				}

				if (assigned[v] != std::numeric_limits<uint>::max()) continue;

				assigned[v] = i;

				results[v] = true;
			}
			else {
				if (assigned[v - n] == i) {
					std::cout << '0';
					return 0;
				}

				if (assigned[v - n] != std::numeric_limits<uint>::max()) continue;

				assigned[v - n] = i;

				results[v - n] = false;
			}
		}
	}

	std::cout << "1\n";

	for (bool v: results)
		std::cout << v << ' ';
}
