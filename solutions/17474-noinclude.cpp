#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <locale>
#include <ranges>
#include <type_traits>
#include <unistd.h>
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
	{t.reinit(l, r)};
	{t.apply()};

	typename T::extracted_t;
	{t.extract()} -> std::same_as<typename T::extracted_t>;
};

template <typename T>
class BasicLazy {
public:
	void propagate(T&, T&) {}
	void reinit(T const& l, T const& r) {
		static_cast<T&>(*this) = l + r;
	}
	void apply() {}

	using extracted_t = T;
	extracted_t extract() {
		return static_cast<T const&>(*this);
	}
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

	T::extracted_t sum(Segment segment) {
		return sum(segment, Segment(0, _size), 0);
	}

	T::extracted_t sum(size_t start, size_t end) {
		return sum(Segment(start, end));
	}

	T::extracted_t at(size_t index) {
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

	T::extracted_t sum(Segment const query, Segment const segment, size_t const index) {
		if (query.includes(segment))
			return _values[index].extract();

		size_t const left = index * 2 + 1;
		size_t const right = index * 2 + 2;

		_values[index].propagate(_values[left], _values[right]);
		_values[index].apply();

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left);

		return sum(query, segment.left(), left)
		     + sum(query, segment.right(), right);
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

template <typename T, typename Input = std::istream>
inline auto InputRange(size_t n, Input& is = std::cin) {
	return std::views::iota(static_cast<size_t>(0), n)
		| std::views::transform([&is](size_t) {
				T temp;
				is >> temp;
				return temp;
				});
}

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
			} while (isspace(ch));

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
			} while (isspace(val));
			return *this;
		}
	};

	istream cin;

	/*
	class ostream {
		private:
			inline void putchar(char const& ch) {
				static char buffer[FASTOSTREAM_BUFFER_SIZE];
				static char* ptr = buffer;
				static char* end = buffer + (FASTOSTREAM_BUFFER_SIZE);

				if (ptr == end) {
					write(STDOUT_FILENO, buffer, FASTOSTREAM_BUFFER_SIZE);
					ptr = buffer;
				}
				*(ptr++) = ch;
			}
		public:
			template <typename T>
				inline ostream& operator<<(T& val)
				requires std::is_integral_v<T> {
					if (val < 0)
						putchar('-');
				}
	};
	*/
}

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Element: public BasicLazy<Element> {
	int max;
	int max_cnt;
	int max2;
	int64_t sum;

	Element() {}

	Element(long long int value):
		max(value),
		max_cnt(1),
		max2(std::numeric_limits<int>::min()),
		sum(value) {}

	Element(int max, int max_cnt, int max2, int64_t sum):
		max(max),
		max_cnt(max_cnt),
		max2(max2),
		sum(sum) {}

	Element operator+(Element const& other) const {
		if (max < other.max)
			return Element(
					other.max,
					other.max_cnt,
					std::max(max, other.max2),
					sum + other.sum
					);

		else if (max == other.max)
			return Element(
					max,
					max_cnt + other.max_cnt,
					std::max(max2, other.max2),
					sum + other.sum
					);

		else
			return Element(
					max,
					max_cnt,
					std::max(max2, other.max),
					sum + other.sum
					);
	}

	void propagate(Element& l, Element& r) {
		l.update(max);
		r.update(max);
	}

	bool update(int new_max) {
		if (new_max >= max) return false;
		if (new_max < max2) return true;

		sum += static_cast<int64_t>(new_max - max) * max_cnt;
		max = new_max;
		return false;
	}

	struct extracted {
		int max;
		int64_t sum;

		extracted operator+(extracted const& other) {
			return extracted {
				std::max(max, other.max),
				sum + other.sum
			};
		}
	};

	using extracted_t = extracted;

	extracted extract() {
		return extracted {
			max,
			sum
		};
	}
};

int main() {
	size_t n;
	Fast::cin >> n;

	LazySegmentTree<Element> tree(InputRange<int, Fast::istream>(n, Fast::cin));

	uint m;
	Fast::cin >> m;

	for (uint i = 0; i < m; i++) {
		char type;
		size_t l, r;
		Fast::cin >> type >> l >> r;

		switch(type) {
			case '1':
				int x;
				Fast::cin >> x;
				tree.update(l - 1, r, [x](Element& val) {
						return val.update(x);
						});
				break;
			case '2':
				std::cout << tree.sum(l - 1, r).max << '\n';
				break;
			case '3':
				std::cout << tree.sum(l - 1, r).sum << '\n';
		}
	}
}
