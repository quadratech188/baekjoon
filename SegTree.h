#include <functional>

struct Segment {
	size_t start;
	size_t end;
	size_t size() {
		return end - start;
	}
	size_t center() {
		return (start + end) / 2;
	}
};
template<typename VAL>
class SegmentNode {
	Segment segment;
	VAL value;
	VAL (*combine_func)(VAL, VAL);
	VAL (*value_func)(size_t);
	SegmentNode<VAL>* left;
	SegmentNode<VAL>* right;
public:
	SegmentNode<VAL>(Segment segment, VAL (*combine_func)(VAL, VAL), VAL(*value_func)(size_t)) {
		this->segment = segment;
		this->combine_func = combine_func;
		this->value_func = value_func;
		
		if (this->segment.size() == 1) {
			this->value = value_func(this->segment.start);
			return;
		}

		this->left = new SegmentNode<VAL>({segment.start, segment.center()}, combine_func, value_func);
		this->right = new SegmentNode<VAL>({segment.center(), segment.end}, combine_func, value_func);

		this->value = combine_func(this->left->value, this->right->value);
	}

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

		return this->combine_func(this->left->query(segment), this->right->query(segment));
	}
	void update(size_t index) {
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
