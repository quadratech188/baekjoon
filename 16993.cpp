#include <iostream>
#include "modules/SegmentTree.h"
#include "modules/FastIO.h"

struct Data {
	int sum;
	int _sum;
	int sum_;
	int _sum_;

	Data operator+(const Data& other) const {
		Data result;
		result.sum = this->sum + other.sum;
		result._sum = std::max(other._sum, this->_sum + other.sum);
		result.sum_ = std::max(this->sum_, this->sum + other.sum_);
		result._sum_ = std::max(std::max(this->_sum_, other._sum_), this->_sum + other.sum_);
		return result;
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	std::vector<Data> values;
	values.reserve(n);

	for (int i = 0; i < n; i++) {
		int value;
		std::cin >> value;
		values.push_back({
				.sum = value,
				._sum = value,
				.sum_ = value,
				._sum_ = value,
				});
	}

	SegmentTree<Data, int> tree(values);

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		size_t a, b;
		std::cin >> a >> b;

		std::cout << tree.query({a - 1, b})._sum_ << std::endl;
	}
}
