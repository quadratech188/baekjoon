#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <concepts>
#include <functional>
#include <ranges>

#include <cstddef>

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
class SegmentTree {
public:
	using value_type = T;

	template <std::ranges::range R>
	SegmentTree(R&& range):
		_size(std::ranges::size(range)), _values(4 * _size) {
		auto it = std::ranges::begin(range);
		init(Segment(0, _size), 0, it);
	}

	SegmentTree(size_t size, T&& default_value = T()):
		_size(size), _values(4 * size) {
		auto it = DummyIterator<T>(default_value);
		init(Segment(0, _size), 0, it);
	}

	template <typename Extractor = std::identity>
	auto sum(Segment segment, Extractor extractor = {}) {
		return sum(segment, Segment(0, _size), 0, extractor);
	}

	template <typename Extractor = std::identity>
	auto sum(size_t start, size_t end, Extractor extractor = {}) {
		return sum(Segment(start, end), extractor);
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(index, 0, Segment(0, _size), func);
	}

	constexpr size_t size() const noexcept {
		return _size;
	}

	constexpr T const& root() const noexcept {
		return _values[0];
	}

private:
	size_t const _size;
	std::vector<T> _values;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter& iterator) {
		if (segment.size() == 1) {
			_values[index] = T(*iterator);
			++iterator;
			return;
		}

		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;

		init(segment.left(), left, iterator);
		init(segment.right(), right, iterator);

		_values[index] = _values[left] + _values[right];
	}

	template <typename Extractor>
	auto sum(Segment const query, Segment const segment, size_t const index, Extractor extractor) {
		if (query.includes(segment))
			return std::invoke(extractor, static_cast<T const&>(_values[index]));

		size_t const left = index * 2 + 1;
		size_t const right = index * 2 + 2;

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right, extractor);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left, extractor);

		return sum(query, segment.left(), left, extractor)
		     + sum(query, segment.right(), right, extractor);
	}

	template <typename Callable>
	void update(size_t const index, size_t const value_index, Segment const segment, Callable func) {
		if (segment.size() == 1) {
			std::invoke(func, _values[value_index]);
			return;
		}

		size_t const left = value_index * 2 + 1;
		size_t const right = value_index * 2 + 2;

		if (index < segment.center())
			update(index, left, segment.left(), func);
		else
			update(index, right, segment.right(), func);

		if constexpr (requires (T x, T const& y, T const& z) {x.reinit(y, z);})
			_values[value_index].reinit(_values[left], _values[right]);
		else
		 	_values[value_index] = _values[left] + _values[right];
	}
};
#include <algorithm>

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

template <typename T, typename Operator>
class OverloadedPlus {
public:
	using value_type = T;

	OverloadedPlus() {}

	OverloadedPlus(T const& value, Operator op = Operator()):
		value(value), op(op) {}
private:
	T value;
	[[no_unique_address]]
	Operator op;
public:
	explicit operator T const&() {
		return value;
	}

	T const& val() const {
		return value;
	}

	OverloadedPlus operator+(OverloadedPlus const& other) {
		return op(value, other.value);
	}

	OverloadedPlus operator+(T const& other) {
		return op(value, other);
	}

	void reinit(OverloadedPlus const& l, OverloadedPlus const& r) {
		value = op(l.value, r.value);
	}
};

template <typename T>
using maxxer = OverloadedPlus<T, Max<T>>;

template <typename T>
using minner = OverloadedPlus<T, Min<T>>;
#include <iostream>
#include <cstdio>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}
int main() {
	FastIO();
	int n, d;
	std::cin >> n >> d;

	// time, points
	std::vector<std::tuple<int, int, int>> dishes;

	for (int i = 0; i < n; i++) {
		int t, a, b;
		std::cin >> t >> a >> b;
		dishes.emplace_back(t, a, b);
	}

	int score = 0;

	// 1개 만드는 경우
	
	for (auto [t, a, b]: dishes) {
		score = std::max(score, a + b);
	}

	// 2개 만드는 경우
	
	std::sort(dishes.begin(), dishes.end());

	SegmentTree<maxxer<int>> tree(n);
	for (int i = 0; i < n; i++) {
		auto [t, a, b] = dishes[i];
		tree.update(i, [b](maxxer<int>& x) {
				x = b;
				});
	}

	std::vector<int> times(n);
	for (int i = 0; i < n; i++) {
		auto [t, a, b] = dishes[i];
		times[i] = t;
	}

	for (int i = 0; i < n; i++) {
		auto [t, a, b] = dishes[i];
		int time_left = d - t;
		auto ind = std::ranges::upper_bound(times, time_left) - times.begin();
		if (ind == 0) break;

		score = std::max(score, a + tree.sum(0, ind).val());
	}

	std::cout << score << '\n';
}
