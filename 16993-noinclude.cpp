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
