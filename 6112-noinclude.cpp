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
class LazySegmentTree {
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

enum Paint {
	BLACK,
	WHITE,
	MIXED
};

struct Data {
	int wrongWhite;
	int rightWhite;
	int wrongBlack;
	int rightBlack;
	Paint color;

	void update(Paint color) {
		switch(color) {
			case WHITE:
				this->wrongWhite += this->rightBlack;
				this->rightWhite += this->wrongBlack;
				this->wrongBlack = 0;
				this->rightBlack = 0;
				this->color = WHITE;
				break;
			case BLACK:
				this->wrongBlack += this->rightWhite;
				this->rightBlack += this->wrongWhite;
				this->wrongWhite = 0;
				this->rightWhite = 0;
				this->color = BLACK;
				break;
		}
	}
	Data operator+(const Data other) const {
		return {
			.wrongWhite = this->wrongWhite + other.wrongWhite,
			.rightWhite = this->rightWhite + other.rightWhite,
			.wrongBlack = this->wrongBlack + other.wrongBlack,
			.rightBlack = this->rightBlack + other.rightBlack,
			.color = MIXED
		};
	}
	void resolve(Data& child1, Data& child2) {
		if (this->color == MIXED) return;
		child1.update(this->color);
		child2.update(this->color);
		this->color = MIXED;
	}
	int correct() {
		return this->rightWhite + this->rightBlack;
	}
};

int main() {
	int r, c, q;
	scanf("%d %d %d", &r, &c, &q);
	
	std::vector<std::vector<Data>> data(c);

	for (auto& column: data) {
		column.reserve(r);
	}
	
	getchar();

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			char value;
			scanf("%c", &value);

			data[j].push_back({
					.wrongWhite = 0,
					.rightWhite = 0,
					.wrongBlack = value == '1'? 1: 0,
					.rightBlack = value == '0'? 1: 0
					});
		}
		getchar();
	}
/*
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			printf("%d", data[j][i].wrongBlack);
		}
		printf("\n");
	}
*/
	std::vector<LazySegmentTree<Data, Paint>> segData;

	segData.reserve(c);

	for (auto& column: data) {
		segData.emplace_back(column);
	}

	for (int i = 0; i < q; i++) {
		int r1, r2, c1, c2, x;
		scanf("%d %d %d %d %d", &r1, &r2, &c1, &c2, &x);
		for (int column = c1 - 1; column <= c2 - 1; column++) {
			segData[column].update({r1 - 1, r2}, x == 1? WHITE: BLACK);
		}

		int sum = 0;

		for (auto& column: segData) {
			sum += column.root().correct();
		}
		printf("%d\n", sum);
	}
}
