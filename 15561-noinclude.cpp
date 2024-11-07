#include <cstddef>
#include <stdio.h>
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

struct Data {
	int max_full;
	int max_left;
	int max_right;
	int max_floating;

	Data operator+(const Data& other) const {
		Data result;
		result.max_full = this->max_full + other.max_full;

		result.max_left = std::max(this->max_left, this->max_full + other.max_left);

		result.max_right = std::max(other.max_right, this->max_right + other.max_full);

		result.max_floating = std::max(std::max(this->max_floating, other.max_floating), this->max_right + other.max_left);
		return result;
	}

	void update(int action) {
		this->max_full = action;
		this->max_left = action;
		this->max_right = action;
		this->max_floating = action;
	}
};

int main() {
	int n, q, u, v;
	scanf("%d %d %d %d", &n, &q, &u, &v);

	std::vector<Data> values(n);

	for (auto& value: values) {
		int a;
		scanf("%d", &a);
		value = {
			.max_full = u * a + v,
			.max_left = u * a + v,
			.max_right = u * a + v,
			.max_floating = u * a + v
		};
	}
	
	SegmentTree<Data, int> tree(values);

	for (int i = 0; i < q; i++) {
		int t, b;
		size_t a;
		scanf("%d %zu %d", &t, &a, &b);
		switch(t) {
			case 0:
				printf("%d\n", tree.query(a - 1, b).max_floating - v);
				break;
			case 1:
				tree.update(a - 1, u * b + v);
		}
	}
}
