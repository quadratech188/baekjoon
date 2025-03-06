#include <cstdio>
#include <iostream>
#include <memory>
#include <utility>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct TrieNode {
	TrieNode(): count(0) {}
	int count;
	std::unique_ptr<TrieNode> on;
	std::unique_ptr<TrieNode> off;
	std::pair<TrieNode*, bool> max(int bit) {
		switch(bit) {
			case 0:
				return on != nullptr && on->count != 0? std::make_pair(on.get(), true): std::make_pair(off.get(), false);
			case 1:
				return off != nullptr && off->count != 0? std::make_pair(off.get(), true): std::make_pair(on.get(), false);
		}
		__builtin_unreachable();
	}
	TrieNode* operator[](int bit) {
		switch(bit) {
			case 0:
				if (off == nullptr) off = std::make_unique<TrieNode>();
				return off.get();
			case 1:
				if (on == nullptr) on = std::make_unique<TrieNode>();
				return on.get();
		}
		__builtin_unreachable();
	}
};

void add(TrieNode* root, int index, int height) {
	if (height < 0) return;
	root->count ++;
	add((*root)[index % 2], index / 2, height - 1);
}

void remove(TrieNode* root, int index, int height) {
	if (height < 0) return;
	root->count --;
	remove((*root)[index % 2], index / 2, height - 1);
}

int max(TrieNode* root, int index, int height) {
	if (height < 0) return 0;
	auto [next, optimal] = root->max(index % 2);
	return max(next, index / 2, height - 1) * 2 + (optimal? 1: 0);
}

int reverse(int value, int length) {
	int result = 0;
	for (int i = 0; i < length; i++) {
		result <<= 1;
		result += value % 2;
		value >>= 1;
	}
	return result;
}

int main() {
	FastIO();
	const int height = 31;
	int m;
	std::cin >> m;
	TrieNode root;

	add(&root, reverse(0, height), height);

	for (int i = 0; i < m; i++) {
		char type;
		int x;
		std::cin >> type >> x;
		switch(type) {
			case '1':
				add(&root, reverse(x, height), height);
				break;
			case '2':
				remove(&root, reverse(x, height), height);
				break;
			case '3':
				std::cout << reverse(max(&root, reverse(x, height), height), height) << '\n';
		}
	}
}
