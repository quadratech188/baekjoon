#include <ranges>
#include <unistd.h>
#include <vector>

#ifndef FASTISTREAM_BUFFER_SIZE
#define FASTISTREAM_BUFFER_SIZE 1 << 20
#endif

#ifndef FASTOSTREAM_BUFFER_SIZE
#define FASTOSTREAM_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		inline char getchar() {
			static char buffer[FASTISTREAM_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (size <= 0) return EOF;
				ptr = buffer;
				end = buffer + size;
			}
			return *(ptr++);
		}
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar();
			} while (std::isspace(ch));

			// Optimized away for non-signed types
			bool negative = false;
			if constexpr (std::is_signed_v<T>) {
				if (ch == '-') {
					negative = true;
					ch = getchar();
				}
			}

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			if constexpr (std::is_signed_v<T>)
				if (negative) val = -val;

			return *this;
		}

		inline istream& operator>>(char& val) {
			do {
				val = getchar();
			} while (std::isspace(val));
			return *this;
		}

		template <typename T1, typename T2>
		inline istream& operator>>(std::pair<T1, T2>& pair) {
			(*this) >> pair.first >> pair.second;
			return *this;
		}

		template <typename... Ts>
		std::tuple<Ts...> get() {
			std::tuple<Ts...> result;

			std::apply([this](auto&... x) {(((*this) >> x), ...);}, result);

			return result;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}

		template <typename T>
		auto to_range(uint size) {
			return std::views::iota(0u, size)
				| std::views::transform([this](uint) {
						T temp;
						(*this) >> temp;
						return temp;
						});
		}
	};

	istream cin;
}
#include <iterator>
#include <sys/types.h>

template <typename T, typename Length = std::size_t>
class LazySum {
public:
	// Builder
	template <typename value>
	using with_length = LazySum<T, value>;

private:
	using size_t = Length;

public:
	using extracted_t = T;

	LazySum():
		value(), length(1), delta() {}
	LazySum(T const& value):
		value(value), length(1), delta() {}
	LazySum(T const& value, size_t length):
		value(value), length(length), delta() {}

private:
	T value;
	size_t length;
	T delta;

public:
	T extract() const {
		return value + delta * length;
	}

	void operator+=(const T& other) {
		delta += other;
	}

	LazySum operator+(const LazySum& other) const {
		return LazySum(extract() + other.extract(), length + other.length);
	}

	void propagate(LazySum& left, LazySum& right) {
		left += delta;
		right += delta;
	}

	void reinit(LazySum const& left, LazySum const& right) {
		value = left.extract() + right.extract();
		delta = 0;
	}

	void apply() {
		value = extract();
		delta = 0;
	}
};
#include <concepts>
#include <functional>
#include <locale>
#include <vector>

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
concept Lazy = requires(T t, T l, T r) {
	{l + r} -> std::same_as<T>;
	{t.reinit((T const&) l, (T const&) r)};
	{t.propagate((T&)l, (T&)r)};
	{t.apply()};
};

template <typename T>
class BasicLazy {
public:
	void propagate(T&, T&) {}
	void reinit(T const& l, T const& r) {
		static_cast<T&>(*this) = l + r;
	}
	void apply() {}
};

template<typename T> requires Lazy<T>
class LazySegmentTree {
public:
	using value_type = T;

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

	template <typename Extractor = std::identity>
	auto sum(Segment segment, Extractor extractor = {}) {
		return sum(segment, Segment(0, _size), 0, extractor);
	}

	template <typename Extractor = std::identity>
	auto sum(size_t start, size_t end, Extractor extractor = {}) {
		return sum(Segment(start, end), extractor);
	}

	template <typename Extractor = std::identity>
	auto at(size_t index, Extractor extractor = {}) {
		return sum(Segment(index, index + 1), extractor);
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

		size_t const left = index * 2 + 1;
		size_t const right = index * 2 + 2;

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

		_values[index].propagate(_values[left], _values[right]);
		_values[index].apply();

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right, extractor);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left, extractor);

		return sum(query, segment.left(), left, extractor)
		     + sum(query, segment.right(), right, extractor);
	}

	template <typename Callable>
	void update(Segment const index, size_t const value_index, Segment const segment, Callable func) {
		if (index.includes(segment)) {
			if constexpr (std::is_same_v<std::invoke_result_t<Callable, T&>, bool>) {
				if (std::invoke(func, _values[value_index])) {
					// Keep iterating
				}
				else
					return;
			}
			else {
				std::invoke(func, _values[value_index]);
				return;
			}
		}

		size_t const left = value_index * 2 + 1;
		size_t const right = value_index * 2 + 2;

		this->_values[value_index].propagate(this->_values[left], this->_values[right]);

		if (index.start < segment.center())
			update(index, left, segment.left(), func);

		if (segment.center() < index.end)
			update(index, right, segment.right(), func);

		_values[value_index].reinit(_values[left], _values[right]);
	}
};
#include <iostream>
#include <vector>

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<int64_t> values(n);

	int64_t prev = 0;
	for (uint i = 0; i < n; i++) {
		int a;
		Fast::cin >> a;
		values[i] = a - prev;
		prev = a;
	}

	LazySegmentTree<LazySum<int64_t>> tree(values);

	uint q;
	Fast::cin >> q;
	for (uint i = 0; i < q; i++) {
		char type;
		Fast::cin >> type;
		if (type == '1') {
			uint l, r;
			Fast::cin >> l >> r;
			l --;
			tree.update(l, r, [](LazySum<int64_t>& x) {
					x += 1;
					});
			if (r == n) continue;
			int len = r - l;
			tree.update(r, [len](LazySum<int64_t>& x) {
					x += - len;
					});
		}
		else {
			uint x;
			Fast::cin >> x;
			std::cout << tree.sum(0, x, [](LazySum<int64_t> const& x) {
					return x.extract();
					}) << '\n';
		}
	}
}
