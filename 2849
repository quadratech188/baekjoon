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

enum Letter {
	L,
	R
};

struct Data {
	Letter left;
	Letter right;
	int length_full;
	int length_left;
	int length_right;
	int length_floating;

	Data operator+(const Data& other) const {
		Data result;
		result.left = this->left;
		result.right = other.right;
		if (this->right != other.left) {
			if (this->length_full != 0 && other.length_full != 0) {
				result.length_full = this->length_full + other.length_full;
			}
			else {
				result.length_full = 0;
			}

			if (this->length_full != 0)
				result.length_left = this->length_full + other.length_left;
			else
				result.length_left = this->length_left;

			if (other.length_full != 0)
				result.length_right = this->length_right + other.length_full;
			else
				result.length_right = other.length_right;

			result.length_floating = std::max(std::max(this->length_floating, other.length_floating), this->length_right + other.length_left);
		}
		else {
			result.length_full = 0;
			result.length_left = this->length_left;
			result.length_right = other.length_right;
			result.length_floating = std::max(this->length_floating, other.length_floating);
		}
		return result;
	}

	void update(bool action) {
		if (this->left == L) {
			this->left = R;
			this->right = R;
		}
		else {
			this->left = L;
			this->right = L;
		}
	}
};

int main() {
	int n, q;
	scanf("%d %d", &n, &q);
	
	SegmentTree<Data, bool> tree(n, {
			.left = L,
			.right = L,
			.length_full = 1,
			.length_left = 1,
			.length_right = 1,
			.length_floating = 1
			});

	for (int i = 0; i < q; i++) {
		size_t index;
		scanf("%zu", &index);
		tree.update(index - 1, true);
		printf("%d\n", tree.root().length_floating);
	}
}
