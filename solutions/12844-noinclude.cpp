#include <cstddef>
#include <cstdio>
#include <iostream>
#include <locale>
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

template<typename T>
class LazySegmentTree {
public:
	LazySegmentTree(const size_t size, const T& val = T()):
	_size(size), _values(4 * size) {
		DummyIterator<T> iter(val);
		init(Segment(0, _size), 0, iter);
	}

	template <typename Iter>
	LazySegmentTree(const size_t size, Iter iterator):
		_size(size), _values(4 * size) {
		init(Segment(0, _size), 0, iterator);
	}

	template <std::ranges::range R>
	LazySegmentTree(const R& range):
		_size(std::ranges::size(range)), _values(4 * _size) {
		auto it = std::ranges::begin(range);
		init(Segment(0, _size), 0, it);
	}

	T sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	T sum(size_t start, size_t end) {
		return sum(Segment(start, end));
	}

	T at(size_t index) {
		return sum(Segment(index, index + 1));
	}

	template <typename Callable>
	void update(Segment segment, Callable func) {
		return update(segment, 0, Segment(0, _size), func);
	}

	template <typename Callable>
	void update(size_t start, size_t end, Callable func) {
		return update(Segment(start, end), func);
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(Segment(index, index + 1), func);
	}

	size_t size() {
		return _size;
	}

	inline T root() {
		return this->_values[0];
	}

private:
	const size_t _size;
	std::vector<T> _values;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter& iterator) {
		if (segment.size() == 1) {
			this->_values[index] = *iterator;
			++iterator;
			return;
		}

		size_t left = index * 2 + 1;
		size_t right = index * 2 + 2;

		init(segment.left(), left, iterator);
		init(segment.right(), right, iterator);

		_values[index] = _values[left] + _values[right];
	}

	T sum(Segment query, Segment segment, size_t index) {
		if (query.includes(segment))
			return _values[index];

		size_t left = index * 2 + 1;
		size_t right = index * 2 + 2;

		_values[index].resolve(_values[left], _values[right]);

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left);

		return sum(query, segment.left(), left)
		     + sum(query, segment.right(), right);
	}

	template <typename Callable>
	void update(Segment index, size_t value_index, Segment segment, Callable func) {
		if (index.includes(segment)) {
			func(_values[value_index]);
			return;
		}

		size_t left = value_index * 2 + 1;
		size_t right = value_index * 2 + 2;

		this->_values[value_index].resolve(this->_values[left], this->_values[right]);

		if (index.start < segment.center())
			update(index, left, segment.left(), func);

		if (segment.center() < index.end)
			update(index, right, segment.right(), func);

		_values[value_index] = _values[left] + _values[right];
	}

};

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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Data {
	Data():
		_value(0), length(1), delta(0) {}
	Data(int value):
		_value(value), length(1), delta(0) {}
	Data(int value, int length):
		_value(value), length(length), delta(0) {}

	int _value;
	int length;
	int delta;

	void apply(int k) {
		delta ^= k;
	}

	int value() const {
		return length % 2 == 0? _value: _value ^ delta;
	}

	void resolve(Data& left, Data& right) {
		left.apply(delta);
		right.apply(delta);
		_value = value();
		delta = 0;
	}

	Data operator+(const Data& other) const {
		return Data(value() ^ other.value(), length + other.length);
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	LazySegmentTree<Data> tree(n, InputIterator<int>());

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;
		if (a == 1) {
			int k;
			std::cin >> k;
			tree.update(b, c + 1, [k](Data& data) {
					data.apply(k);
					});
		}
		else {
			std::cout << tree.sum(b, c + 1).value() << '\n';
		}
	}
}
