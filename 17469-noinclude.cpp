#include <algorithm>
#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <ranges>
#include <set>
#include <unordered_set>
#include <vector>

class UnionFind {
public:
	UnionFind(int size) {
		this->size = size;
		data.reserve(size);
		for (int i = 0; i < size; i++)
			data.push_back(i);
	}

	int find(int x) {
		if (data[x] == x)
			return x;
		data[x] = find(data[x]);
		return data[x];
	}

	void connect(int x, int y) {
		data[find(x)] = find(y);
	}

	bool connected(int x, int y) {
		return find(x) == find(y);
	}

private:
	std::vector<int> data;
	int size;
};

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

struct None {};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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

	std::vector<std::unordered_set<int>> colors(n);

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
