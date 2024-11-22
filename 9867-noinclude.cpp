#include <cstddef>
#include <cstdio>
#include <iostream>
#include <vector>

struct Segment {
	size_t start;
	size_t end;
	size_t size() {
		return end - start;
	}
	inline size_t center() {
		return (start + end) / 2;
	}
	inline Segment left() {
		return {start, center()};
	}
	inline Segment right() {
		return {center(), end};
	}
};

template<typename VAL, typename ACTION>
class SegmentTree {
	std::vector<VAL> values;
	size_t length;

	void init(Segment currentSegment, size_t currentIndex, std::vector<VAL>& values) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex] = values[currentSegment.start];
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		init(currentSegment.left(), leftIndex, values);
		init(currentSegment.right(), rightIndex, values);

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}
	void init(Segment currentSegment, size_t currentIndex, VAL& defaultValue) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex] = defaultValue;
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		init(currentSegment.left(), leftIndex, defaultValue);
		init(currentSegment.right(), rightIndex, defaultValue);

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

	VAL query(Segment segment, Segment currentSegment, size_t currentIndex) {
		if (segment.start <= currentSegment.start && currentSegment.end <= segment.end) {
			return this->values[currentIndex];
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		if (currentSegment.center() <= segment.start) {
			return query(segment, currentSegment.right(), rightIndex);
		}
		if (segment.end <= currentSegment.center()) {
			return query(segment, currentSegment.left(), leftIndex);
		}
		return query(segment, currentSegment.left(), leftIndex) + query(segment, currentSegment.right(), rightIndex);
	}

	void update(size_t index, ACTION action, Segment currentSegment, size_t currentIndex) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex].update(action);
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		if (index < currentSegment.center()) {
			update(index, action, currentSegment.left(), leftIndex);
		}
		else {
			update(index, action, currentSegment.right(), rightIndex);
		}

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

public:
	SegmentTree(std::vector<VAL>& values) {
		this->length = values.size();
		this->values = std::vector<VAL>(4 * this->length);
		init({0, this->length}, 0, values);
	}
	SegmentTree(size_t size, VAL defaultValue) {
		this->length = size;
		this->values = std::vector<VAL>(4 * size);
		init({0, size}, 0, defaultValue);
	}
	inline VAL query(Segment segment) {
		return query(segment, {0, this->length}, 0);
	}
	inline void update(size_t index, ACTION action) {
		update(index, action, {0, this->length}, 0);
	}
	VAL root() {
		return this->values[0];
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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
