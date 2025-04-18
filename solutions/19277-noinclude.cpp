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

struct Action {
	int type;
	long long int value;
};

struct Data {
	bool reset;
	long long int value;
	long long int add;
	long long int divide;

	void toZero() {
		this->reset = true;
		this->value = 0;
		this->add = 0;
		this->divide = 1;
	}

	void resolve(Data& child1, Data& child2) {
		if (this->reset) {
			child1.toZero();
			child2.toZero();
		}

		child1.update({0, this->add});
		child1.update({1, this->divide});

		child2.update({0, this->add});
		child2.update({1, this->divide});

		this->reset = false;
		this->value = this->calculate();
		this->add = 0;
		this->divide = 1;
	}

	void update(Action action) {
		switch(action.type) {
			case 0:
				this->add += this->divide * action.value;
				break;

			case 1:
				if (this->calculate() < action.value) {
					this->toZero();
					break;
				}
				divide *= action.value;
		}
	}
	long long int calculate() const {
		return (this->value + this->add) / this->divide;
	}

	Data operator+(const Data& other) const {
		return {
			.reset = false,
			.value = std::max(this->calculate(), other.calculate()),
			.add = 0,
			.divide = 1
		};
	}
};

int main() {
	int n, q;
	scanf("%d %d", &n, &q);

	std::vector<Data> values;
	values.reserve(n);

	for (int i = 0; i < n; i++) {
		int value;
		scanf("%d", &value);
		values.push_back({
				.value = value,
				.add = 0, 
				.divide = 1,
				});
	}

	LazySegmentTree<Data, Action> segTree(values);

	for (int i = 0; i < q; i++) {
		int t, l, r, x;
		scanf("%d %d %d %d", &t, &l, &r, &x);

		if (t == 2) {
			printf("%lld\n", segTree.query({l, r + 1}).calculate());
		}
		else {
			segTree.update({l, r + 1}, {
					.type = t,
					.value = x
					});
		}
	}
}
