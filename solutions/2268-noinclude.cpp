#include <stdio.h>
#include <utility>
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

std::vector<long long int> values;

long long int combine_func(long long int val1, long long int val2) {
	return val1 + val2;
}

long long int value_func(int index) {
	return values[index];
}

int main() {

	int n, m;

	scanf("%d %d", &n, &m);

	values = std::vector<long long int>(n, 0);
	
	auto root = SegmentNode<int, long long int>({0, n}, combine_func, value_func);

	for (int i = 0; i < m; i++) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c);

		if (a == 0) {
			if (b > c) {
				std::swap(b, c);
			}

			long long int sum = root.query({b - 1, c});

			printf("%lld", sum);

		}
		else {
			values[b - 1] = c;
			root.update(b - 1);
		}
	}

	return 0;
}
