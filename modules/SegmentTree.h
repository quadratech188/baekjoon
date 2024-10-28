#include <vector>

#include "Segment.h"

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
	inline VAL query(Segment segment) {
		return query(segment, {0, this->length}, 0);
	}

	inline void update(size_t index, ACTION action) {
		update(index, action, {0, this->length}, 0);
	}
};
