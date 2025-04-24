#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <locale>
#include <unordered_map>
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

	constexpr size_t size() const {
		return _size;
	}

	constexpr T const& root() {
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
			if constexpr (std::same_as<std::invoke_result_t<decltype(func), decltype(_values[value_index])>,
					bool>) {
				if (!std::invoke(func, _values[value_index]) || segment.size() == 1)
					return;
			} else {
				std::invoke(func, _values[value_index]);
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

struct Data {
	int layers;
	int delta;
	bool equals;

	Data(int layers = 0, int delta = 0, int equals = true):
		layers(layers), delta(delta), equals(equals) {}

	Data operator+(Data const& other) const {
		return Data(
				value(),
				0,
				equals && other.equals && value() == other.value()
				);
	}

	int value() const {
		return layers + delta;
	}

	void resolve(Data& l, Data& r) {
		l.delta += delta;
		r.delta += delta;
		delta = 0;
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

	LazySegmentTree<Data> tree(2 * n);

	std::sort(data.begin(), data.end());

	for (Boundary& boundary: data) {
		Segment segment(
				compressed_rows.compress(boundary.row_begin),
				compressed_rows.compress(boundary.row_end)
				);

		if (!tree.sum(segment).equals) {
			std::cout << '1';
			return 0;
		}

		if (boundary.type == BEGIN)
			tree.update(segment, [](Data& val) {val.delta++;});
		else
			tree.update(segment, [](Data& val) {val.delta--;});
	}

	std::cout << '0';
}
