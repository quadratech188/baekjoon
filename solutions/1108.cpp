#include <iostream>
#include "FastIO.h"
#include "Compress.h"
#include "ListGraph2.h"
#include <queue>
#include <string>
#include <variant>
#include "SSC.h"
#include "Bins.h"

template <typename G>
int64_t solve(G& graph, IdBins const& bins, uint parent) {
	int64_t& result = graph[parent];
	if (result != -1)
		return result;

	result = 1;
	for (auto child: graph.children(parent)) {
		if (bins[child] == bins[parent]) continue;
		result += solve(graph, bins, child);
	}
	return result;
}

int main() {
	FastIO();
	uint n;
	std::cin >> n;

	ListGraph<int64_t, std::monostate> graph;
	Compress<std::string> compress;

	for (uint i = 0; i < n; i++) {
		std::string name;
		std::cin >> name;
		size_t index = compress.add(name);
		graph.safe_at(index) = -1;

		uint k;
		std::cin >> k;
		for (uint j = 0; j < k; j++) {
			std::string referrer;
			std::cin >> referrer;
			size_t idx = compress.add(referrer);
			graph.safe_at(idx, -1);
			// Reverse
			graph.connect(index, idx);
		}
	}
	IdBins bins(graph.size());
	SSC(graph, bins)();

	std::string query;
	std::cin >> query;
	std::cout << solve(graph, bins, compress.compress(query));
}
