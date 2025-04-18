#include <iostream>
#include <map>
#include <memory>
#include <queue>

struct TrieNode {
	bool needs_removing;
	bool file;

	bool recursive_removable;
	std::map<char, std::unique_ptr<TrieNode>> children;

	TrieNode* operator[](char index) {
		return children.try_emplace(index, std::make_unique<TrieNode>()).first->second.get();
	}
};

void loop() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int n1;
	std::cin >> n1;
	std::cin.ignore(1);

	TrieNode root;

	for (int i = 0; i < n1; i++) {
		TrieNode* ref = &root;
		while (true) {
			ref->recursive_removable = true;
			char index;
			std::cin.get(index);
			if (index == '\n') break;

			ref = (*ref)[index];
		}
		ref->needs_removing = true;
		ref->file = true;
	}

	int n2;
	std::cin >> n2;
	std::cin.ignore(1);

	for (int i = 0; i < n2; i++) {
		TrieNode* ref = &root;
		while (true) {
			ref->recursive_removable = false;
			char index;
			std::cin.get(index);
			if (index == '\n') break;

			ref = (*ref)[index];
		}
		ref->needs_removing = false;
		ref->file = true;
	}

	if (root.recursive_removable) {
		std::cout << "1\n";
		return;
	}

	std::queue<TrieNode*> queue;
	queue.push(&root);
	int count = 0;

	while (!queue.empty()) {
		TrieNode* parent = queue.front();
		queue.pop();

		for (auto& [index, child]: parent->children) {
			if (child->recursive_removable) {
				count++;
				continue;
			}

			if (child->file && child->needs_removing) count++;

			queue.push(child.get());
		}
	}
	std::cout << count << '\n';
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
