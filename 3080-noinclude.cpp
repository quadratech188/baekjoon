#include <cstdio>
#include <iostream>
#include <memory>
#include <queue>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct TrieNode {
	TrieNode(): end(false) {}
	bool end;
	std::vector<std::pair<char, std::unique_ptr<TrieNode>>> children;

	TrieNode* operator[](char index) {
		for (auto& it: children) {
			if (index == it.first) return it.second.get();
		}
		children.emplace_back(index, std::make_unique<TrieNode>());
		return children[children.size() - 1].second.get();
	}
};

long long int fact(int n) {
	long long int result = 1;
	for (int i = 1; i <= n; i++)
		result = (result * i) % 1'000'000'007;
	return result;
}

long long int permutation(TrieNode* root) {
	long long int result = fact(root->children.size());
	if (root->end) result = result * (root->children.size() + 1) % 1'000'000'007;

	for (auto& [index, child]: root->children) {
		result = result * permutation(child.get()) % 1'000'000'007;
	}
	return result;
}

int main() {
	FastIO();
	int n;
	std::cin >> n;
	std::cin.ignore(1);

	TrieNode root;
	std::string temp;

	for (int i = 0; i < n; i++) {
		TrieNode* ref = &root;
		std::cin >> temp;
		for (char index: temp)
			ref = (*ref)[index];

		ref->end = true;
	}

	std::cout << permutation(&root) << '\n';
}
