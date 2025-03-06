#include <iostream>
#include <map>
#include <memory>
#include <string_view>

struct TrieNode {
	TrieNode(): end(false) {}
	bool end;
	std::map<char, std::unique_ptr<TrieNode>> children;

	TrieNode* operator[](char index) {
		return children.try_emplace(index, std::make_unique<TrieNode>()).first->second.get();
	}
};

void print(TrieNode const& root, std::string& operations, std::string::iterator it, bool max) {
	if (root.end) {
		operations += 'P';
	}
	for (auto& [index, child]: root.children) {
		if (index == *it && max) continue;
		operations += index;
		print(*child.get(), operations, std::next(it), false);
		operations += '-';
	}
	if (root.children.size() != 0 && max) {
		operations += *it;
		print(*root.children.at(*it).get(), operations, std::next(it), true);
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int n;
	std::cin >> n;

	TrieNode root;
	std::string max;

	std::string temp;
	for (int i = 0; i < n; i++) {
		std::cin >> temp;

		TrieNode* ref = &root;

		for (char index: temp) {
			ref = (*ref)[index];
		}
		ref->end = true;

		if (temp.size() > max.size()) max = temp;
	}

	std::string result;
	print(root, result, max.begin(), true);
	int length = result.find_last_not_of('-') + 1;
	std::cout << length << '\n';
	for (int i = 0; i < length; i++) {
		std::cout << result[i] << '\n';
	}
}
