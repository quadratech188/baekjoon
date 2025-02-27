#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <locale>
#include <vector>

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
			this->_values[index] = T(*iterator);
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
			std::invoke(func, _values[value_index]);
			return;
		}

		size_t left = value_index * 2 + 1;
		size_t right = value_index * 2 + 2;

		// Does the function mutate values?
		if constexpr (!std::invocable<Callable, const T&>)
			this->_values[value_index].resolve(this->_values[left], this->_values[right]);

		if (index.start < segment.center())
			update(index, left, segment.left(), func);

		if (segment.center() < index.end)
			update(index, right, segment.right(), func);

		if constexpr (!std::invocable<Callable, const T&>)
			_values[value_index] = _values[left] + _values[right];
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Data {
	Data():
		length(0) {}
	Data(int value):
		length(1) {
		values.push_back(value);
	}

	size_t length;
	std::vector<int> values;

	Data operator+(const Data& other) {
		Data result(length + other.length);
		result.values.reserve(result.length);

		std::merge(values.begin(), values.end(),
				other.values.begin(), other.values.end(),
				std::back_inserter(result.values));

		return result;
	}

	void resolve(Data& left, Data& right) {}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	LazySegmentTree<Data> tree(n, InputIterator<int>());

	int m;
	std::cin >> m;

	for (int _ = 0; _ < m; _++) {
		int i, j, k;
		std::cin >> i >> j >> k;

		int total = 0;
		tree.update(i - 1, j, [&total, k](const Data& data){
				total += data.values.end() - std::upper_bound(data.values.begin(), data.values.end(), k);
				});
		std::cout << total << '\n';
	}
}
