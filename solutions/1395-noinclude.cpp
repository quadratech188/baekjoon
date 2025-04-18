#include <cstddef>
#include <cstdio>
#include <iostream>
#include <locale>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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
		init(Segment(0, _size), 0, DummyIterator<T>(val));
	}

	template <typename Iter>
	LazySegmentTree(const size_t size, Iter iterator):
		_size(size), _values(4 * size) {
		init(Segment(0, _size), 0, iterator);
	}

	T sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	T sum(size_t start, size_t end) {
		return sum(Segment(start, end));
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
	std::vector<T> _values;
	const size_t _size;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter iterator) {
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

struct Data {
	int length;
	int on;
	bool flipped;

	Data(): length(1), on(0), flipped(false) {}
	Data(int length, int on, int flipped):
		length(length), on(on), flipped(flipped) {}

	void flip() {
		flipped = !flipped;
		on = length - on;
	}

	Data operator+(const Data& other) const {
		return Data(length + other.length, on + other.on, false);
	}

	void resolve(Data& left, Data& right) {
		if (flipped) {
			left.flip();
			right.flip();
			flipped = false;
		}
	}
};

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	LazySegmentTree<Data> tree(n);

	for (int i = 0; i < m; i++) {
		int o, s, t;
		std::cin >> o >> s >> t;

		switch(o) {
			case 0:
			tree.update(s - 1, t, [](Data& data) {data.flip();});
			break;
			case 1:
			std::cout << tree.sum(s - 1, t).on << '\n';
		};
	}
}
