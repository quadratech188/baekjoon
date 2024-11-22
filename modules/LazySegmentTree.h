#include <vector>

#include "Segment.h"

template<typename VAL, typename ACTION>
class LazySegmentTree {
	std::vector<VAL> values;
	size_t length;

	void init(Segment currentSegment, size_t currentIndex, VAL defaultValue) {
		if (currentSegment.size() == 1) {
			this->values[currentIndex] = defaultValue;
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		init(currentSegment.left(), leftIndex, values);
		init(currentSegment.right(), rightIndex, values);

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

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

	VAL query(Segment segment, Segment currentSegment, size_t currentIndex) {
		if (segment.start <= currentSegment.start && currentSegment.end <= segment.end) {
			return this->values[currentIndex];
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		this->values[currentIndex].resolve(this->values[leftIndex], this->values[rightIndex]);

		if (currentSegment.center() <= segment.start) {
			return query(segment, currentSegment.right(), rightIndex);
		}
		if (segment.end <= currentSegment.center()) {
			return query(segment, currentSegment.left(), leftIndex);
		}
		return query(segment, currentSegment.left(), leftIndex)
			+ query(segment, currentSegment.right(), rightIndex);
	}

	void update(Segment segment, ACTION action, Segment currentSegment, size_t currentIndex) {
		if (segment.start <= currentSegment.start && currentSegment.end <= segment.end) {
			this->values[currentIndex].update(action);
			return;
		}

		size_t leftIndex = currentIndex * 2 + 1;
		size_t rightIndex = currentIndex * 2 + 2;

		this->values[currentIndex].resolve(this->values[leftIndex], this->values[rightIndex]);

		if (segment.start < currentSegment.center()) {
			update(segment, action, currentSegment.left(), leftIndex);
		}
		if (currentSegment.center() < segment.end) {
			update(segment, action, currentSegment.right(), rightIndex);
		}

		this->values[currentIndex] = this->values[leftIndex] + this->values[rightIndex];
	}

public:
	LazySegmentTree(std::vector<VAL>& values) {
		this->length = values.size();
		this->values.resize(this->length * 4);
		init({0, this->length}, 0, values);
	}
	
	LazySegmentTree(size_t n, VAL defaultValue) {
		this->length = n;
		this->values.resize(this->length * 4);
		init({0, this->length}, 0, defaultValue);
	}

	inline VAL query(Segment segment) {
		return query(segment, {0, this->length}, 0);
	}

	inline void update(Segment segment, ACTION action) {
		update(segment, action, {0, this->length}, 0);
	}
	inline VAL root() {
		return this->values[0];
	}
};
