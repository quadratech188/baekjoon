#include "FastIO2.h"
#include "Bins.h"
#include "SSC.h"
#include "ListGraph2.h"
#include <iostream>

int main() {
	uint n, m;
	Fast::cin >> m >> n;

	ListGraph<std::monostate, std::monostate> graph(2 * m);

	for (uint _ = 0; _ < n; _++) {
		uint i, j;
		char X, Y;
		Fast::cin >> i >> X >> j >> Y;
		i--;
		j--;

		bool s_a = X == 'P';
		bool s_b = Y == 'P';
		
		graph.connect(i + s_a * m, j + (1 - s_b) * m);
		graph.connect(j + s_b * m, i + (1 - s_a) * m);
	}

	SetBins<uint> ssc_groups;

	SSC(graph, ssc_groups) ();

	for (uint i = 0; i < ssc_groups.bins.size(); i++) {}

	std::vector<uint> assigned(n, std::numeric_limits<uint>::max());
	std::vector<bool> results(m);

	for (uint i = 0; i < ssc_groups.bins.size(); i++) {
		auto& bin = ssc_groups.bins[i];
		for (auto v: bin) {
			if (v < m) {
				if (assigned[v] == i) {
					std::cout << "-1";
					return 0;
				}

				if (assigned[v] != std::numeric_limits<uint>::max()) continue;

				assigned[v] = i;

				results[v] = true;
			}
			else {
				if (assigned[v - m] == i) {
					std::cout << "-1";
					return 0;
				}

				if (assigned[v - m] != std::numeric_limits<uint>::max()) continue;

				assigned[v - m] = i;

				results[v - m] = false;
			}
		}
	}

	for (auto v: results)
		std::cout << (v? 'P': 'G');
}

