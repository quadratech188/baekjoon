#include <cstddef>
#include <functional>
#include <stdio.h>

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
struct SegmentTree {
	Segment segment;
	VAL value;
	SegmentTree<VAL, ACTION>* left;
	SegmentTree<VAL, ACTION>* right;
	
	SegmentTree(Segment segment, std::vector<VAL>& values) {
		this->segment = segment;
		
		if (this->segment.size() == 1) {
			this->value = values[this->segment.start];
			return;
		}

		this->left = new SegmentTree<VAL, ACTION>({segment.start, segment.center()}, values);
		this->right = new SegmentTree<VAL, ACTION>({segment.center(), segment.end}, values);

		this->value = this->left->value + this->right->value;
	}
	SegmentTree(std::vector<VAL>& values): SegmentTree({0, values.size()}, values) {}

	VAL query(Segment segment) {
		if (segment.start <= this->segment.start && this->segment.end <= segment.end) {
			return this->value;
		}
		
		if (this->segment.center() <= segment.start) {
			return this->right->query(segment);
		}
		if (segment.end <= this->segment.center()) {
			return this->left->query(segment);
		}

		return this->left->query(segment) + this->right->query(segment);
	}
	void update(size_t index, ACTION action) {
		if (this->segment.size() == 1) {
			this->value.update(action);
			return;
		}
		
		if (index < this->segment.center()) {
			this->left->update(index, action);
		}
		else{
			this->right->update(index, action);
		}

		this->value = this->left->value + this->right->value;
	}
};

struct Min {
	int value;
	size_t index;
	inline Min operator+(const Min other) const {
		if (this->value <= other.value) {
			return *this;
		}
		else {
			return other;
		}
	}
	void update(int action) {
		this->value = action;
	}
};

int main() {
	int n;
	scanf("%d", &n);

	std::vector<Min> values(n);

	for (size_t i = 0; i < n; i++) {
		int value;
		scanf("%d", &value);
		values[i] = {
			.value = value,
			.index = i + 1
		};
	}

	SegmentTree<Min, int> tree(values);

	int m;
	scanf("%d", &m);

	for(int i = 0; i < m; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);

		switch(a) {
			case 1:
				tree.update(b - 1, c);
				break;
			case 2:
				printf("%zu\n", tree.query({b - 1, c}).index);
		}
	}
}
