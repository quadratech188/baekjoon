#include <cstdint>
#include <iostream>
#include "../modules/SegmentTree.h"
#include "../modules/InputRange.h"
#include "../modules/FastIO2.h"

struct Data {
	long long int sum;
	long long int sum_;
	long long int _sum;
	long long int _sum_;

	Data(long long int value = 0):
		sum(value), sum_(0), _sum(0), _sum_(0) {}

	Data(int64_t sum, int64_t sum_, int64_t _sum, int64_t _sum_):
		sum(sum), sum_(sum_), _sum(_sum), _sum_(_sum_) {}

	Data operator+(const Data& other) const {
		return Data(
			std::max(this->sum + other._sum, this->sum_ + other.sum),
			std::max(this->sum + other._sum_, this->sum_ + other.sum_),
			std::max(this->_sum + other._sum, this->_sum_ + other.sum),
			std::max(this->_sum + other._sum_, this->_sum_ + other.sum_)
		);
	}

	void update(int value) {
		this->sum = value;
	}
};

int main() {
	int n, d;
	Fast::cin >> n >> d;

	SegmentTree<Data> machines(InputRange<int64_t, Fast::istream>(n, Fast::cin));

	long long int sum = 0;
	for (int _ = 0; _ < d; _++) {
		int i, m;
		Fast::cin >> i >> m;

		machines.update(i - 1, [m](Data& val) {
				val.update(m);
				});
		sum += machines.root().sum;
	}

	std::cout << sum;
}
