#include "modules/UnionFind.h"
#include "modules/Input.h"
#include "modules/Types.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <set>
#include <ranges>

int main() {
	FastIO();
	int n, q;
	std::cin >> n >> q;

	std::vector<int> parents(n);
	parents[0] = 0;

	for (int i = 1; i < n; i++) {
		std::cin >> parents[i];
		parents[i] --;
	}

	std::vector<std::set<int>> colors(n);

	for (int i = 0; i < n; i++) {
		int temp;
		std::cin >> temp;
		colors[i].insert(temp);
	}

	UnionFind uf(n);

	std::vector<std::pair<int, int>> queries = Input::toVec<std::pair<int, int>>(n + q - 1);
	std::vector<int> answers;
	answers.reserve(q);

	for (auto& it: queries | std::views::reverse) {
		auto [type, a] = it;

		switch(type) {
			case 1: {
				int root_a = uf.find(a - 1);
				int root_b = uf.find(parents[a - 1]);

				if (colors[root_a].size() < colors[root_b].size()) {
					uf.connect(root_a, root_b);
					colors[root_b].merge(colors[root_a]);
				}
				else {
					uf.connect(root_b, root_a);
					colors[root_a].merge(colors[root_b]);
				}
				break;
			}
			case 2:
				answers.push_back(colors[uf.find(a - 1)].size());
		}
	}
	for (int i: answers | std::views::reverse) std::cout << i << '\n';
}
