#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <locale>
#include <ranges>
#include <unordered_map>
#include <vector>

template <typename T>
class Compress {
public:
	Compress() {}

	template <std::ranges::range R>
	Compress(R&& values) {
		size_t index = 0;

		for (T& value: values) {
			if (_inverse.find(value) != _inverse.end()) continue;
			_inverse[value] = index;
			index++;
		}

		_size = index;

		_values.resize(_size);

		for (auto& it: _inverse) {
			_values[it.second] = it.first;
		}
	}

	size_t compress(const T& val) {
		return _inverse.at(val);
	}

	T decompress(const size_t index) {
		return _values[index];
	}

	size_t size() const {
		return _size;
	}

private:
	size_t _size;
	std::vector<T> _values;
	std::unordered_map<T, size_t> _inverse;
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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

template <typename T>
concept Lazy = requires(T t, T l, T r) {
	{l + r} -> std::same_as<T>;
	{t.resolve(l, r)};
};

template<typename T> requires Lazy<T>
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
			auto result = std::invoke(func, _values[value_index]);

			if constexpr(std::convertible_to<decltype(result), bool>) {
				if (!result || segment.size() == 1) return;
			}
			else {
				return;
			}
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

enum Type {
	BEGIN,
	END
};

struct Boundary {
	Boundary(int column, int row_begin, int row_end, Type type):
		column(column), row_begin(row_begin), row_end(row_end), type(type) {}
	int column;
	int row_begin, row_end;
	Type type;

	bool operator<(const Boundary& other) {
		return column < other.column;
	}
};

struct LazyColor {
	int length;
	int count;
	int layers;

	LazyColor(int length = 0):
		length(length), count(0), layers(0) {}

	LazyColor(int length, int count, int layers):
		length(length), count(count), layers(layers) {}

	int value() {
		if (layers == 0) return count;
		return length;
	}

	LazyColor operator+(LazyColor& other)  {
		int delta = std::min(layers, other.layers);
		layers -= delta;
		other.layers -= delta;

		return LazyColor(
				length + other.length,
				value() + other.value(),
				delta
				);
	}

	bool update(int delta) {
		if (layers + delta >= 0) {
			// All good
			layers += delta;
			// Stop here
			return false;
		}
		// resolve() will update the children with layers, we don't do anything here
		return true;
	}

	void resolve(LazyColor& left, LazyColor& right) {
		left.update(layers);
		right.update(layers);
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;
	std::vector<Boundary> data;
	data.reserve(2 * n);

	std::vector<int> rows;
	rows.reserve(2 * n);

	for (int i = 0; i < n; i++) {
		int x1, x2, y1, y2;
		std::cin >> x1 >> y1 >> x2 >> y2;

		rows.push_back(y1);
		rows.push_back(y2);

		data.emplace_back(x1, y1, y2, BEGIN);
		data.emplace_back(x2, y1, y2, END);
	}

	std::sort(rows.begin(), rows.end());

	Compress<int> compressed_rows(rows);

	LazySegmentTree<LazyColor> tree(
			std::views::iota((size_t)0, compressed_rows.size() - 1)
			| std::views::transform([&compressed_rows](size_t index) {
				return compressed_rows.decompress(index + 1)
				- compressed_rows.decompress(index);
				})
			);

	std::sort(data.begin(), data.end());

	int prev_column = 0;
	long long int sum = 0;

	for (Boundary& boundary: data) {
		sum += static_cast<long long int>(boundary.column - prev_column) * tree.root().value();

		int delta = boundary.type == BEGIN? 1: -1;
		tree.update(
				compressed_rows.compress(boundary.row_begin),
				compressed_rows.compress(boundary.row_end),
				[delta](LazyColor& val) {
				return val.update(delta);
				});
		
		prev_column = boundary.column;
	}
	std::cout << sum;
}
