#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <vector>

struct Segment {
	Segment(size_t start, size_t end): start(start), end(end) {}

	size_t start;
	size_t end;
	size_t size() {
		return end - start;
	}
	inline size_t center() {
		return (start + end) / 2;
	}
	inline Segment left() {
		return Segment(start, center());
	}
	inline Segment right() {
		return Segment(center(), end);
	}

	bool includes(const Segment& other) {
		return start <= other.start && other.end <= end;
	}
};

template <typename T>
class DummyIterator {
public:
	DummyIterator(const T& val): _val(val) {}

	const T& operator*() const {
		return _val;
	}

	DummyIterator& operator++() {
		return *this;
	}
	void operator++(int) {}

private:
	const T& _val;
};

template <typename T, typename Operator = std::plus<T>>
class SegmentTree {
public:
	SegmentTree(const size_t size, const T& val = T(), Operator op = Operator()):
		_size(size), _values(4 * size), _operator(op) {
		init(Segment(0, _size), 0, DummyIterator<T>(val));
	}

	template <typename Iter>
	SegmentTree(const size_t size, Iter iterator, Operator op = Operator()):
		_size(size), _values(4 * size), _operator(op) {
		init(Segment(0, _size), 0, iterator);
	}

	T sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	T sum(size_t start, size_t end) {
		return sum(Segment(start, end));
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(index, 0, Segment(0, _size), func);
	}

	size_t size() {
		return _size;
	}

private:
	std::vector<T> _values;
	const size_t _size;
	const Operator _operator;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter iterator) {
		if (segment.size() == 1) {
			_values[index] = *iterator;
			++iterator;
			return;
		}

		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;
		init(segment.left(), left, iterator);
		init(segment.right(), right, iterator);

		_values[index] = _operator(_values[left], _values[right]);
	}

	T sum(Segment query, Segment segment, size_t index) {
		if (query.includes(segment))
			return _values[index];

		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left);

		return _operator(sum(query, segment.left(), left)
		          ,sum(query, segment.right(), right));
	}

	template <typename Callable>
	void update(size_t index, size_t value_index, Segment segment, Callable func) {
		if (segment.size() == 1) {
			func(_values[value_index]);
			return;
		}

		size_t left = 2 * value_index + 1;
		size_t right = 2 * value_index + 2;
		
		if (index < segment.center())
			update(index, left, segment.left(), func);
		else
		 	update(index, right, segment.right(), func);

		_values[value_index] = _operator(_values[left], _values[right]);
	}
};

template <typename T>
class Min {
public:
	T operator()(const T& l, const T& r) const {
		return std::min(l, r);
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

template <typename T>
class InputIterator {
public:
	using iterator_category = std::input_iterator_tag;

	InputIterator(const std::istream& is = std::cin): _input(is) {
	}

	T operator*() {
		T temp;
		std::cin >> temp;
		return temp;
	}

	InputIterator& operator++() {
		return *this;
	}

	InputIterator operator++(int) {
		return *this;
	}
private:
	const std::istream& _input;
};

int main() {
	FastIO();

	int n;
	std::cin >> n;

	SegmentTree<int, Min<int>> tree(n, InputIterator<int>());

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;

		switch(a) {
			case 1:
				tree.update(b - 1, [&c](int& val){val = c;});
				break;
			case 2:
				std::cout << tree.sum(b - 1, c) << '\n';
		}
	}
}
