#include "FastIO2.h"
#include "ListGraph2.h"
#include <limits>
#include <variant>

// https://en.wikipedia.org/wiki/Biconnected_component#Linear_time_depth-first_search

ListGraph<std::monostate, std::monostate> graph;

std::vector<uint> depths;

std::vector<std::pair<uint, uint>> results;

uint solve(uint before_parent, uint parent, uint depth) {
	uint lowpoint = depth;
	depths[parent] = depth;

	for (auto child: graph.children(parent)) {
		if (child == before_parent) continue;

		if (depths[child] != std::numeric_limits<uint>::max()) {
			lowpoint = std::min(lowpoint, depths[child]);
			continue;
		}

		uint child_lowpoint = solve(parent, child, depth + 1);

		if (child_lowpoint > depth)
			results.emplace_back(parent, child);

		lowpoint = std::min(lowpoint, child_lowpoint);
	}

	return lowpoint;
} 

int main() {
	uint v, e;
	Fast::cin >> v >> e;

	graph.resize(v);
	depths.resize(v, std::numeric_limits<uint>::max());

	for (uint i = 0; i < e; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		graph.connect(a - 1, b - 1);
		graph.connect(b - 1, a - 1);
	}

	for (uint i = 0; i < v; i++) {
		if (depths[i] == std::numeric_limits<uint>::max()) {
			solve(i, i, 0);
		}
	}

	for (auto& result: results) {
		if (result.second < result.first)
			std::swap(result.second, result.first);
	}

	std::ranges::sort(results);

	std::cout << results.size() << '\n';
	for (auto& result: results)
		std::cout << result.first + 1 << ' ' << result.second + 1 << '\n';
}
