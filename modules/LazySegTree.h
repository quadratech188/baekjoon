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

template<typename VAL, typename ACTION>
class LazySegNode {
	Segment segment;
	std::function<VAL(VAL, Segment, VAL, Segment)> combine_func;
	std::function<VAL(size_t)> value_func;
	VAL value;

	LazySegNode<VAL, ACTION>* left;
	LazySegNode<VAL, ACTION>* right;

public:

	LazySegNode(Segment segment, std::function<VAL(VAL, Segment, VAL, Segment)> combine_func, std::function<VAL(size_t)> value_func) {

		this->segment = segment;
		this->combine_func = combine_func;
		
		if (this->segment.size() == 1) {
			this->value = value_func(this->segment.start);
			return;
		}

		this->left = new LazySegNode<VAL, ACTION>({segment.start, segment.center()}, combine_func, value_func);
		this->right = new LazySegNode<VAL, ACTION>({segment.center(), segment.end}, combine_func, value_func);

		this->value = combine_func(this->left->value, this->left->segment, this->right->value, this->right->segment);
	}

	VAL query(Segment segment) {
		if (segment.start <= this->segment.start && this->segment.end <= segment.end) {
			return this->value.resolved(this->segment);
		}
		this->value.resolve(this->left->value, this->right->value, this->segment);

		if (this->segment.center() <= segment.start) {
			return this->right->query(segment);
		}
		if (segment.end <= this->segment.center()) {
			return this->left->query(segment);
		}

		return this->combine_func(this->left->query(segment), this->left->segment, this->right->query(segment), this->right->segment);
	}

	void update(Segment segment, ACTION action) {
		if (segment.start <= this->segment.start && this->segment.end <= segment.end) {
			this->value.update(action);
			return;
		}

		this->value.resolve(this->left->value, this->right->value, this->segment);

		if (segment.start < this->segment.center()) {
			this->left->update(segment, action);
		}
		if (this->segment.center() < segment.end) {
			this->right->update(segment, action);
		}

		this->value = combine_func(this->left->value, this->left->segment, this->right->value, this->right->segment);
	}
};
