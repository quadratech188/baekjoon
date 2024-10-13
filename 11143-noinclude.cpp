#include <cstdio>
#include <vector>
typedef int INDEX;

template<typename IDX>
struct Segment {
	IDX start;
	IDX end;
	IDX size() {
		return end - start;
	}
	IDX center() {
		return (start + end) / 2;
	}
};
template<typename IDX, typename VAL>
class SegmentNode {
	Segment<IDX> segment;
	VAL value;
	VAL (*combine_func)(VAL, VAL);
	VAL (*value_func)(INDEX);
	SegmentNode<IDX, VAL>* left;
	SegmentNode<IDX, VAL>* right;
public:
	SegmentNode<IDX, VAL>(Segment<IDX> segment, VAL (*combine_func)(VAL, VAL), VAL(*value_func)(INDEX)) {
		this->segment = segment;
		this->combine_func = combine_func;
		this->value_func = value_func;
		
		if (this->segment.size() == 1) {
			this->value = value_func(this->segment.start);
			return;
		}

		this->left = new SegmentNode<IDX, VAL>({segment.start, segment.center()}, combine_func, value_func);
		this->right = new SegmentNode<IDX, VAL>({segment.center(), segment.end}, combine_func, value_func);

		this->value = combine_func(this->left->value, this->right->value);
	}

	VAL query(Segment<IDX> segment) {
		if (segment.start <= this->segment.start && this->segment.end <= segment.end) {
			return this->value;
		}
		
		if (this->segment.center() <= segment.start) {
			return this->right->query(segment);
		}
		if (segment.end <= this->segment.center()) {
			return this->left->query(segment);
		}

		return this->combine_func(this->left->query(segment), this->right->query(segment));
	}
	void update(INDEX index) {
		if (this->segment.size() == 1) {
			this->value = this->value_func(index);
			return;
		}
		
		if (index < this->segment.center()) {
			this->left->update(index);
		}
		else{
			this->right->update(index);
		}

		this->value = combine_func(this->left->value, this->right->value);
	}
};

std::vector<int> values;

int combine_func(int val1, int val2) {
	return val1 + val2;
}

int value_func(int index) {
	return values[index];
}

void loop() {
	int b, p, q;
	scanf("%d %d %d", &b, &p, &q);

	values = std::vector<int>(b, 0);

	auto root = SegmentNode<int, int>({0, b}, combine_func, value_func);

	for (int i = 0; i < p + q; i++) {
		char x;
		int y, z;
		scanf(" %c %d %d", &x, &y, &z);

		if (x == 'P') {
			values[y - 1] += z;
			root.update(y - 1);
		}
		else if (x == 'Q') {
			printf("%d\n", root.query({y - 1, z}));
		}
	}
}

int main() {
	int t;
	scanf("%d", &t);
	for (int i = 0; i < t; i++) {
		loop();
	}
}
