#include <cstddef>
#include <cstdio>
#include <iostream>
#include <limits>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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

template <typename T>
struct Min {
	T value;
	Min operator+(const Min& other) const {
		return {
			.value = std::max(this->value, other.value)
		};
	}
};

int main() {
	FastIO();
	while (true) {
		int n;
		std::cin >> n;
		
		if (n == 0) break;

		std::vector<int> values(2 * n);
		std::vector<Min<int>> minValues(2 * n);

		for (int i = 0; i < n; i++) {
			std::cin >> values[i];
			values[i + n] = values[i];
			minValues[i] = {values[i]};
			minValues[i + n] = {values[i + n]};
		}

		SegmentTree<int, bool> sumSeg(values);
		SegmentTree<Min<int>, bool> maxSeg(minValues);

		int min = std::numeric_limits<int>::max();

		for (int i = 0; i < n; i++) {
			min = std::min(min, sumSeg.query({i, n / 2 + i}) - maxSeg.query({i, n / 2 + i}).value);
		}

		std::cout << (min + sumSeg.root() / 2) << std::endl;
	}
}
