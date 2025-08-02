#include <iostream>
#include <map>
#include <vector>
#include <ranges>

void dfs(std::vector<std::map<std::string, uint>>& trie, uint parent, uint depth) {
	for (auto& child: trie[parent]) {
		for (uint i = 0; i < depth; i++)
			std::cout << "--";

		std::cout << child.first << '\n';

		dfs(trie, child.second, depth + 1);
	}
}

int main() {
	uint n;
	std::cin >> n;

	std::vector<std::vector<std::string>> information(n);

	for (auto& sequence: information) {
		uint k;
		std::cin >> k;

		sequence.resize(k);
		for (uint j = 0; j < k; j++)
			std::cin >> sequence[j];
	}

	std::vector<std::map<std::string, uint>> trie(1);

	for (auto& sequence: information) {
		uint ptr = 0;

		for (std::string& next: sequence) {

			auto next_idx = trie[ptr].find(next);

			if (next_idx != trie[ptr].end()) {
				ptr = next_idx->second;
				continue;
			}

			ptr = trie[ptr][next] = trie.size();
			trie.emplace_back();
		}
	}
	dfs(trie, 0, 0);
}
