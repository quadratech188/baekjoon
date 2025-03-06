#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

template< typename T1, typename T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& data) {
	is >> data.first >> data.second;
	return is;
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& data) {
	for (auto& element: data)
		is >> element;
	return is;
}

namespace Input {
	template<typename T>
	inline std::vector<T> toVec(size_t n) {
		std::vector<T> result(n);
		std::cin >> result;

		return result;
	}
}

struct TrieNode {
	std::unique_ptr<TrieNode> on;
	std::unique_ptr<TrieNode> off;
	std::pair<TrieNode*, bool> max(int bit) {
		switch(bit) {
			case 0:
				return on != nullptr? std::make_pair(on.get(), true): std::make_pair(off.get(), false);
			case 1:
				return off != nullptr? std::make_pair(off.get(), true): std::make_pair(on.get(), false);
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
	add((*root)[index % 2], index / 2, height - 1);
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
	int n;
	std::cin >> n;
	TrieNode root;

	std::vector<int> values = Input::toVec<int>(n);

	for (int value: values) add(&root, reverse(value, height), height);

	int maximum = 0;

	for (int value: values) maximum = std::max(maximum, reverse(max(&root, reverse(value, height), height), height));

	std::cout << maximum;
}
