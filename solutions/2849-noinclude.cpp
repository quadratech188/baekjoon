#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <ranges>
#include <type_traits>
#include <unistd.h>

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
concept SegmentTreeElement = requires(T x, T l, T r) {
	{l + r} -> std::same_as<T>;
	{x.reinit((T const&)l, (T const&)r)};
};

template <typename T> requires SegmentTreeElement<T>
class SegmentTree {
public:
	using value_type = T;

	struct combine_context {
		Segment left, right, current;
	};
	inline static combine_context combine_ctx;

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

		combine_ctx = {segment.left(), segment.right(), segment};
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

		combine_ctx = {segment.left(), segment.right(), segment};
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

		combine_ctx = {segment.left(), segment.right(), segment};
		_values[value_index].reinit(_values[left], _values[right]);
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
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

enum Type {
	L,
	R
};

struct Element {

	Type left;
	Type right;

	uint _max_, _max, max_;

	Element():
		left(L), right(L), _max_(1), _max(1), max_(1) {}

	void reinit(Element const& l, Element const& r) {
		left = l.left;
		right = r.right;

		if (l.right == r.left) {
			_max_ = std::max(l._max_, r._max_);
			_max = r._max;
			max_ = l.max_;
		}
		else {
			_max_ = std::max({l._max_, l._max + r.max_, r._max_});
			
			if (r.max_ == SegmentTree<Element>::combine_ctx.right.size())
				_max = l._max + SegmentTree<Element>::combine_ctx.right.size();
			else
			 	_max = r._max;

			if (l.max_ == SegmentTree<Element>::combine_ctx.left.size())
				max_ = SegmentTree<Element>::combine_ctx.left.size() + r.max_;
			else
			 	max_ = l.max_;
		}
	}

	Element operator+(Element const& other) {
		Element result;
		result.reinit(*this, other);
		return result;
	}

	void toggle() {
		if (left == L)
			left = right = R;
		else
		 	left = right = L;
	}
};

int main() {
	FastIO();
	uint N, Q;
	Fast::cin >> N >> Q;

	SegmentTree<Element> tree(N);

	for (uint i = 0; i < Q; i++) {
		uint index;
		Fast::cin >> index;

		tree.update(index - 1, &Element::toggle);

		std::cout << tree.root()._max_ << '\n';
	}
}
