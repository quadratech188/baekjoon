#include "FastIO.h"
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

struct Trie {
	struct TrieNode {
		uint use_count;
		std::array<uint, 26> nexts = {};
		bool end_of_word;
	};

	uint next(uint i, uint next) {
		if (nodes[i].nexts[next] == 0) {
			nodes[i].nexts[next] = nodes.size();
			nodes.emplace_back();
		}
		return nodes[i].nexts[next];
	}

	std::vector<TrieNode> nodes;
};


bool loop() {
	FastIO();
	uint n;
	std::cin >> n;

	if (std::cin.eof()) return false;

	std::vector<std::string> words(n);
	for (auto& word: words)
		std::cin >> word;

	Trie trie;
	trie.nodes.emplace_back();

	for (auto& word: words) {
		uint ref = 0;
		for (char ch: word) {
			trie.nodes[ref].use_count ++;
			ref = trie.next(ref, ch - 'a');
		}
		trie.nodes[ref].end_of_word = true;
	}

	int64_t count = 0;
	for (uint i = 1; i < trie.nodes.size(); i++) {
		auto& node = trie.nodes[i];
		if (std::ranges::count_if(node.nexts, [](uint x) {return x != 0;}) + node.end_of_word > 1)
			count += node.use_count;
	}
	count += trie.nodes[0].use_count;

	std::cout << (double)count / n << '\n';

	return true;
}

int main() {
	std::cout << std::fixed << std::setprecision(2);
	while (loop()) {}
}
