#include <cassert>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

struct Segment {
	constexpr Segment(): start(0), end(0) {}
	constexpr Segment(size_t start, size_t end): start(start), end(end) {}

	size_t start;
	size_t end;
	constexpr size_t size() const {
		return end - start;
	}
	constexpr size_t center() const {
		return (start + end) / 2;
	}
	constexpr Segment left() const {
		return Segment(start, center());
	}
	constexpr Segment right() const {
		return Segment(center(), end);
	}

	constexpr bool includes(const Segment& other) const {
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
		_values(4 * size), _size(size), _operator(op) {
		init(Segment(0, _size), 0, DummyIterator<T>(val));
	}

	template <typename Iter>
	SegmentTree(const size_t size, Iter iterator, Operator op = Operator()):
		_size(size), _values(4 * size), _operator(op) {
		init(Segment(0, _size), 0, iterator);
	}

	template <std::ranges::range R>
	SegmentTree(R&& range, Operator op = Operator()):
		_size(std::ranges::distance(range)), _values(4 * _size), _operator(op) {
		init(Segment(0, _size), 0, std::ranges::begin(range)); 
	}

	inline T sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	inline T sum(size_t start, size_t end) {
		return sum(Segment(start, end));
	}

	inline T root() {
		return sum(0, _size);
	}

	inline T at(size_t index) {
		return sum(Segment(index, index + 1));
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(index, 0, Segment(0, _size), func);
	}

	inline size_t size() {
		return _size;
	}

#ifndef HACKABLE
private:
#endif
	const size_t _size;
	std::vector<T> _values;
	const Operator _operator;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter&& iterator) {
		if (segment.size() == 1) {
			_values[index] = T(*iterator);
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
			std::invoke(func, _values[value_index]);
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Data {
	int delta;
	int minimum;

	Data(char type) {
		switch(type) {
			case '(':
				delta = 1;
				minimum = 0;
				break;
			case ')':
				delta = -1;
				minimum = -1;
		}
	}

	void toggle() {
		if (delta == 1) {
			delta = -1;
			minimum = -1;
		}
		else {
			delta = 1;
			minimum = 0;
		}
	}

	Data(int delta = 0, int minimum = 0):
		delta(delta), minimum(minimum) {}

	Data operator+(Data const& other) const {
		return Data(
				delta + other.delta,
				std::min(minimum, delta + other.minimum)
				);
	}
};

int main() {
	FastIO();
	std::string str;
	std::cin >> str;

	SegmentTree<Data> tree(str);

	int m;
	std::cin >> m;

	int count = 0;

	for (int i = 0; i < m; i++) {
		int index;
		std::cin >> index;

		tree.update(index - 1, &Data::toggle);

		Data result = tree.root();

		if (result.delta == 0 && result.minimum >= 0)
			count++;
	}

	std::cout << count;
}
