#include <iostream>
#include <array>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <map>

struct TrieNode {
	bool value;
	std::map<char, std::unique_ptr<TrieNode>> data;
};

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	int c, n;
	std::cin >> c >> n;
	TrieNode root;
	std::string temp;

	for (int i = 0; i < c; i++) {
		TrieNode* ref = &root;
		std::cin >> temp;
		for(char index: temp) {
			auto [it, success] = ref->data.try_emplace(index, std::make_unique<TrieNode>());
			ref = it->second.get();
		}
		ref->value = true;
	}

	std::unordered_set<std::string> names;
	for (int i = 0; i < n; i++) {
		std::cin >> temp;
		names.emplace(temp);
	}

	int q;
	std::cin >> q;
	std::string team;

	for (int i = 0; i < q; i++) {
		TrieNode* ref = &root;
		std::cin >> team;
		bool found = false;

		for (int j = 0; j < team.size() - 1; j++) {
			char index = team[j];
			auto it = ref->data.find(index);
			if (it == ref->data.end()) break;
			ref = it->second.get();

			if (ref->value) {
				if (names.find(team.substr(j + 1)) != names.end()) {
					found = true;
					break;
				}
			}
		}
		if (found) std::cout << "Yes\n";
		else std::cout << "No\n";
	}
}
