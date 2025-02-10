#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <ios>
#include <iostream>
#include <locale>
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

class LazyFill {
	enum FillType {
		PARTIAL,
		FULL
	};
	int length;
	int filled;
	FillType type;

	LazyFill(int length, int filled, FillType type):
		length(length), filled(filled), type(type) {}

public:
	LazyFill():
		length(1), filled(0), type(PARTIAL) {}

	LazyFill operator+(const LazyFill& other) const {
		if (this->type == FULL && other.type == FULL)
			return LazyFill(length + other.length, filled + other.filled, FULL);
		else
			return LazyFill(length + other.length, filled + other.filled, PARTIAL);
	}

	void fill() {
		filled = length;
		type = FULL;
	}

	int sum() {
		return filled;
	}

	void resolve(LazyFill& left, LazyFill& right) {
		if (type == FULL) {
			left.fill();
			right.fill();
		}
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

bool loop() {
	int n;
	std::cin >> n;
	if (n == 0) return false;

	std::vector<LazySegmentTree<LazyFill>> trees(2001, LazySegmentTree<LazyFill>(2001));

	for (int i = 0; i < n; i++) {
		int x1, y1, x2, y2;
		std::cin >> x1 >> y1 >> x2 >> y2;

		for (int y = y1; y < y2; y++) {
			trees[y + 1000].update(x1 + 1000, x2 + 1000, [](LazyFill& val) {
					val.fill();
					});
		}
	}
	int sum = 0;
	for (auto& tree: trees)
		sum += tree.root().sum();

	std::cout << sum << '\n';

	return true;
}

int main() {
	FastIO();
	std::cout << std::fixed << std::setprecision(2);
	while (loop()) {}
}
