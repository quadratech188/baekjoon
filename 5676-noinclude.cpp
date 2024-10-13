#include <stdio.h>
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
	return val1 * val2;
}

int value_func(int index) {
	if (values[index] > 0) {
		return 1;
	}
	else if (values[index] == 0) {
		return 0;
	}
	else {
		return -1;
	}
}

int main() {
	while (true) {
		int n, k;

		int isEOF = scanf("%d %d", &n, &k);

		if (isEOF == -1) {
			break;
		}

		values = std::vector<int>(n);

		for (int i = 0; i < n; i++) {
			scanf("%d", &values[i]);
		}
		
		auto root = SegmentNode<int, int>({0, n}, combine_func, value_func);

		for (int i = 0; i < k; i++) {
			char c;
			int a, b;
			scanf(" %c %d %d", &c, &a, &b);

			if (c == 'P') {
				int result = root.query({a - 1, b});
					
				if (result == 1) {
					printf("+");
				}				
				else if (result == 0) {
					printf("0");
				}
				else {
					printf("-");
				}
			}
			else {
				values[a - 1] = b;
				root.update(a - 1);
			}
		}
		printf("\n");
	}
	return 0;
}
