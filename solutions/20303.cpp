#include "../modules/FastIO2.h"
#include "../modules/UnionFind.h"

int main() {
	uint n, m, k;
	Fast::cin >> n >> m >> k;

	auto candies = Fast::cin.to_vec<int>(n);

	UnionFind uf(n);

	for (uint i = 0; i < m; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		uf.connect(a - 1, b - 1);
	}
	uf.flatten();

	std::vector<uint> child_count(n, 1);

	for (uint i = 0; i < n; i++) {
		if (uf.at(i) == i) continue;

		candies[uf.at(i)] += candies[i];
		child_count[uf.at(i)] ++;
	}

	std::vector<int> max_candies(k);

	for (uint i = 0; i < n; i++) {
		if (uf.at(i) != i) continue;

		for (uint children_used = k - 1; child_count[i] <= children_used; children_used --) {
			max_candies[children_used] = std::max(
					max_candies[children_used],
					max_candies[children_used - child_count[i]] + candies[i]
					);
		}
	}

	std::cout << max_candies[k - 1];
}
