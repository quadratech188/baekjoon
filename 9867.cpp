#include <iostream>
#include "modules/SegmentTree.h"
#include "modules/FastIO.h"

struct Data {
	long long int sum;
	long long int sum_;
	long long int _sum;
	long long int _sum_;

	Data operator+(const Data& other) const {
		return {
			.sum = std::max(this->sum + other._sum, this->sum_ + other.sum),
			.sum_ = std::max(this->sum + other._sum_, this->sum_ + other.sum_),
			._sum = std::max(this->_sum + other._sum, this->_sum_ + other.sum),
			._sum_ = std::max(this->_sum + other._sum_, this->_sum_ + other.sum_)
		};
	}

	void update(int value) {
		this->sum = value;
	}
};

int main() {
	FastIO();

	int n, d;
	std::cin >> n >> d;

	std::vector<Data> values(n);

	for (size_t i = 0; i < n; i++) {
		int value;
		std::cin >> value;
		values[i] = {
			.sum = value,
			.sum_ = 0,
			._sum = 0,
			._sum_ = 0
		};
	}

	SegmentTree<Data, int> machines(values);

	long long int sum = 0;
	for (int _ = 0; _ < d; _++) {
		int i, m;
		std::cin >> i >> m;

		machines.update(i - 1, m);
		sum += machines.root().sum;
	}

	std::cout << sum;
}
