#include "modules/InputIterator.h"
#include "modules/LazySegmentTree.h"
#include "modules/FastIO.h"
#include <algorithm>
#include <iostream>

struct Data {
	Data():
		length(0) {}
	Data(int value):
		length(1) {
		values.push_back(value);
	}

	size_t length;
	std::vector<int> values;

	Data operator+(const Data& other) {
		Data result(length + other.length);
		result.values.reserve(result.length);

		std::merge(values.begin(), values.end(),
				other.values.begin(), other.values.end(),
				std::back_inserter(result.values));

		return result;
	}

	void resolve(Data& left, Data& right) {}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	LazySegmentTree<Data> tree(n, InputIterator<int>());

	int m;
	std::cin >> m;

	int total = 0;

	for (int _ = 0; _ < m; _++) {
		int i, j, k;
		std::cin >> i >> j >> k;

		i = i ^ total;
		j = j ^ total;
		k = k ^ total;

		total = 0;
		tree.update(i - 1, j, [&total, k](const Data& data){
				total += data.values.end() - std::upper_bound(data.values.begin(), data.values.end(), k);
				});
		std::cout << total << '\n';
	}
}
