#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>

struct Segment {
	Segment(): start(0), end(0) {}
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
		assert(Segment(0, _size).includes(segment));
		assert(segment.start < segment.end);
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

private:
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
class InputRange: public std::ranges::view_interface<InputRange<T>> {
public:
	InputRange(size_t size, std::istream& is = std::cin):
		is(&is), _size(size) {}

	InputRange():
		_size(0), is(nullptr) {}

private:
	std::istream* is;
	size_t _size;

public:
	class iterator {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;

		iterator(std::istream* is, size_t index):
			is(is), index(index) {
			++(*this);
		}

		iterator():
			is(nullptr), index(0) {}

	private:
		std::istream* is;
		size_t index;

	public:
		T operator*() const {
			T value;
			(*is) >> value;
			return value;
		}

		iterator& operator++() {
			return *this;
		}

		iterator operator++(int) {
			iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator!=(iterator const& other) const {
			return index != other.index;
		}

		bool operator==(iterator const& other) const {
			return index == other.index;
		}
	};

	iterator begin() const {
		return iterator(is, 0);
	}

	iterator end() const {
		return iterator(is, _size);
	}

	size_t size() const noexcept {
		return _size;
	}
};

template <typename T>
class Min {
public:
	T operator()(const T& l, const T& r) const {
		return std::min(l, r);
	}
};

template <typename T>
class Max {
public:
	T operator()(const T& l, const T& r) const {
		return std::max(l, r);
	}
};

template <typename T>
class Mul {
public:
	T operator()(const T& l, const T& r) const {
		return l * r;
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

bool loop() {
	int n, k;
	std::cin >> n >> k;

	if (std::cin.eof()) return false;

	SegmentTree<int, Mul<int>> tree{InputRange<int>(n) | std::views::transform([](int val) {
			if (val < 0) return -1;
			if (val == 0) return 0;
			return 1;
			})};

	for (int _ = 0; _ < k; _++) {
		char type;
		std::cin >> type;
		switch(type) {
			case 'C': {
				int i, V;
				std::cin >> i >> V;

				if (V < 0)
					tree.update(i - 1, [](int& val) {val = -1;});
				else if (V == 0)
					tree.update(i - 1, [](int& val) {val = 0;});
				else
				 	tree.update(i - 1, [](int& val) {val = 1;});
				break;
			}
			case 'P': {
				int i, j;
				std::cin >> i >> j;

				int result = tree.sum(i - 1, j);

				if (result < 0) std::cout << '-';
				else if (result == 0) std::cout << '0';
				else std::cout << '+';
			}
		}
	}

	std::cout << '\n';
	return true;
}

int main() {
	FastIO();
	while (loop()) {}
}
