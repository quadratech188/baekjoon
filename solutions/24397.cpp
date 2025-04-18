#include <iostream>
#include <memory>
#include <utility>
#include "modules/FastIO.h"
#include "modules/Input.h"

struct TrieNode {
	TrieNode(): count(0) {}
	int count;
	std::unique_ptr<TrieNode> on;
	std::unique_ptr<TrieNode> off;
	std::pair<TrieNode*, bool> different(int bit) {
		switch(bit) {
			case 0:
				return (on && on->count != 0)? std::make_pair(on.get(), true): std::make_pair(off.get(), false);
			case 1:
				return (off && off->count != 0)? std::make_pair(off.get(), true): std::make_pair(on.get(), false);
		}
		__builtin_unreachable();
	}
	std::pair<TrieNode*, bool> same(int bit) {
		switch(bit) {
			case 0:
				return (off && off->count != 0)? std::make_pair(off.get(), true): std::make_pair(on.get(), false);
			case 1:
				return (on && on->count != 0)? std::make_pair(on.get(), true): std::make_pair(off.get(), false);
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

bool add(TrieNode* root, int index, int height) {
	if (height < 0) {
		if (root->count != 0) return false;

		root->count++;
		return true;
	}
	bool success = add((*root)[index % 2], index / 2, height - 1);
	if (success)
		root->count ++;
	return success;
}

int find_max(TrieNode* root, int index, int height) {
	if (height < 0) return 0;
	auto [next, optimal] = root->different(index % 2);
	return find_max(next, index / 2, height - 1) * 2 + (optimal? 1: 0);
}

int find_min(TrieNode* root, int index, int height) {
	if (height < 0) return 0;
	auto [next, optimal] = root->same(index % 2);
	return find_min(next, index / 2, height - 1) * 2 + (optimal? 0: 1);
}

int remove_min(TrieNode* root, int height) {
	root->count --;
	if (height < 0) return 0;
	auto [next, optimal] = root->same(0);
	return remove_min(next, height - 1) * 2 + (optimal? 0: 1);
}

int remove_max(TrieNode* root, int height) {
	root->count --;
	if (height < 0) return 0;
	auto [next, optimal] = root->same(1);
	return remove_max(next, height - 1) * 2 + (optimal? 1: 0);
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

void loop() {
	FastIO();
	const int height = 31;
	int n, m;
	std::cin >> n >> m;
	TrieNode root;

	auto ns = Input::toVec<int>(n);
	
	for (int i = 0; i < m; i++) {
		int temp;
		std::cin >> temp;
		add(&root, reverse(temp, height), height);
	}

	for (int i = 0; i < q; i++) {
		char type;
		std::cin >> type;
		switch(type) {
			case '1': {
				int x;
				std::cin >> x;
				std::cout << reverse(find_min(&root, reverse(x, height), height), height) << '\n';
				break;
		    }
			case '2': {
				int x;
				std::cin >> x;
				std::cout << reverse(find_max(&root, reverse(x, height), height), height) << '\n';
				break;
			}
			case '3': {
				int x;
				std::cin >> x;
				add(&root, reverse(x, height), height);
				std::cout << root.count << '\n';
				break;
			}
			case '4': {
				std::cout << reverse(remove_min(&root, height), height) << '\n';
				break;
		  	}
			case '5': {
				std::cout << reverse(remove_max(&root, height), height) << '\n';
				break;
			}
		}
	}
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
