#include "FastIO2.h"
#include "SegmentTree2.h"
#include <algorithm>
#include <iostream>
#include <numeric>

int main() {
	uint n;
	Fast::cin >> n;

	SegmentTree<int64_t> tree(Fast::cin.to_range<int>(n));


	std::vector<std::tuple<uint, int>> first_queries;
	std::vector<std::tuple<uint, uint, uint>> second_queries;

	uint m;
	Fast::cin >> m;

	for (uint i = 0; i < m; i++) {
		char type;
		Fast::cin >> type;

		if (type == '1')
			first_queries.push_back(Fast::cin.get<uint, int>());
		else
		 	second_queries.push_back(Fast::cin.get<uint, uint, uint>());
	}

	std::vector<uint> offline_indices(second_queries.size());
	std::iota(offline_indices.begin(), offline_indices.end(), 0);

	std::ranges::sort(offline_indices, [&second_queries](uint a, uint b) {
			return second_queries[a] < second_queries[b];
			});

	uint ptr = 0;
	std::vector<int64_t> results(second_queries.size());

	for (uint index: offline_indices) {
		auto [k, i, j] = second_queries[index];
		for (; ptr < k; ptr ++) {
			auto [i, v] = first_queries[ptr];
			tree.update(i - 1, [v](int64_t& x) {x = v;});
		}
		results[index] = tree.sum(i - 1, j);
	}

	for (auto result: results)
		std::cout << result << '\n';
}
