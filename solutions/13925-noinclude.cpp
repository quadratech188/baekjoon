#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <locale>
#include <ostream>
#include <ranges>
#include <type_traits>
#include <unistd.h>
#include <vector>

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

template <typename T, typename Input = std::istream>
inline auto InputRange(size_t n, Input& is = std::cin) {
	return std::views::iota(static_cast<size_t>(0), n)
		| std::views::transform([&is](size_t) {
				T temp;
				is >> temp;
				return temp;
				});
}

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
	{t.propagate(l, r)};
	{t.reinit(l, r)};
	{t.apply()};

	typename T::extracted_t;
	{t.extract()} -> std::same_as<typename T::extracted_t>;
};

template <typename Callable, typename Arg>
concept const_invocable = requires(Callable callable, Arg const& arg) {
	callable(arg);
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

	// Non-const

	template <typename Callable>
	requires (!std::invocable<Callable, T const&>)
	void update(Segment const index, size_t const value_index, Segment const segment, Callable func) {
		if (index.includes(segment)) {
			std::invoke(func, _values[value_index]);
			return;
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

	// Const
	
	template <typename Callable>
	requires std::invocable<Callable, T const&>
	void update(Segment const index, size_t const value_index, Segment const segment, Callable func) {
		if (index.includes(segment)) {
			std::invoke(func, _values[value_index]);
			return;
		}

		size_t const left = value_index * 2 + 1;
		size_t const right = value_index * 2 + 2;

		if (index.start < segment.center())
			update(index, left, segment.left(), func);

		if (segment.center() < index.end)
			update(index, right, segment.right(), func);
	}
};

template <typename T, T MOD>
struct StaticModPolicy {
	static_assert(MOD < std::numeric_limits<T>::max() / 2);
	static constexpr T mod() {
		return MOD;
	}
};

template <typename T, typename tag = void>
struct DynamicModPolicy {
	static T& mod() {
		static T value = 0;
		return value;
	}
};

template <typename T, typename T2, typename Policy>
class ModInt {
public:
	constexpr ModInt(T val) noexcept {
		if (val < 0) val += Policy::mod();
		val %= Policy::mod();
		value = val;
	}

	constexpr ModInt() noexcept:
		value(0) {}

private:
	T value;

	struct raw {};
	constexpr ModInt(T val, raw) noexcept:
		value(val) {}
public:
	constexpr static ModInt verified(T val) noexcept {
		return ModInt(val, raw{});
	}

	constexpr T val() const noexcept {
		return value;
	}

	constexpr explicit operator T() const noexcept {
		return value;
	}

	constexpr inline ModInt operator+(ModInt const& other) const noexcept {
		T sum = value + other.value;
		if (sum >= Policy::mod()) sum -= Policy::mod();
		return ModInt(sum, raw{});
	}

	constexpr inline ModInt& operator+=(ModInt const& other) noexcept {
		value += other.value;
		if (value >= Policy::mod()) value -= Policy::mod();
		return *this;
	}

	constexpr inline ModInt operator-(ModInt const& other) const noexcept {
		if (value < other.value)
			return ModInt(value + Policy::mod() - other.value);
		else
		 	return ModInt(value - other.value);
	}

	constexpr inline ModInt& operator-=(ModInt const& other) noexcept {
		if (value < other.value)
			value += (Policy::mod() - other.value);
		else
			value -= other.value;

		return *this;
	}

	constexpr inline ModInt& operator++() noexcept {
		if (++value == Policy::mod()) value = 0;
		return *this;
	}

	constexpr inline ModInt operator*(ModInt const& other) const noexcept {
		return ModInt(static_cast<T2>(value) * other.value % Policy::mod(), raw{});
	}

	constexpr inline ModInt& operator*=(ModInt const& other) noexcept {
		value = static_cast<T2>(value) * other.value % Policy::mod();
		return *this;
	}

	constexpr inline bool operator!=(T const& other) const noexcept {
		return value != other;
	}
	constexpr inline bool operator==(T const& other) const noexcept {
		return value == other;
	}

	inline friend std::ostream& operator<<(std::ostream& os, ModInt const& val) {
		os << val.value;
		return os;
	}

	static inline void set_mod(T val) {
		Policy::mod() = val;
	}
};

template <uint32_t MOD>
using sm32 = ModInt<uint32_t, uint64_t, StaticModPolicy<uint32_t, MOD>>;

template <uint64_t MOD>
using sm64 = ModInt<uint64_t, uint64_t, StaticModPolicy<uint64_t, MOD>>;

using sm32_1e9_7 = sm32<1'000'000'007>;
using sm64_1e9_7 = sm64<1'000'000'007>;

template <typename tag = void>
using dm32 = ModInt<uint32_t, uint64_t, DynamicModPolicy<uint32_t, tag>>;

template <typename tag = void>
using dm64 = ModInt<uint64_t, uint64_t, DynamicModPolicy<uint64_t, tag>>;

struct Update {
	sm32_1e9_7 a, b;
};

struct Data {
	using extracted_t = sm32_1e9_7;

	sm32_1e9_7 a;
	sm32_1e9_7 b;
	sm32_1e9_7 sum;
	int32_t length;

	Data() {}
	Data(sm32_1e9_7 value, int32_t length = 1):
		a(1), b(0), sum(value), length(length) {}

	void update(sm32_1e9_7 c, sm32_1e9_7 d) noexcept {
		a *= c;
		b *= c;
		b += d;
	}
	
	sm32_1e9_7 extract() const noexcept {
		return this->a * this->sum + this->b * this->length;
	}

	Data operator+(const Data& other) const noexcept {
		return Data(
				extract() + other.extract(),
				length + other.length
				);
	}

	void propagate(Data& child1, Data& child2) const noexcept {
		child1.update(a, b);
		child2.update(a, b);
	}

	void reinit(Data const& child1, Data const& child2) noexcept {
		sum = child1.extract() + child2.extract();
		a = 1;
		b = 0;
	}

	void apply() noexcept {
		sum = extract();
		a = 1;
		b = 0;
	}
};

int main() {
	FastIO();
	size_t n;
	Fast::cin >> n;

	LazySegmentTree<Data> tree(InputRange<uint, Fast::istream>(n, Fast::cin)
			| std::views::transform([](int const& val) {return sm32_1e9_7::verified(val);}));

	uint m;
	Fast::cin >> m;

	for (uint i = 0; i < m; i++) {
		char type;
		size_t x, y;
		Fast::cin >> type >> x >> y;

		uint v;
		if (type != '4')
			Fast::cin >> v;

		switch(type) {
			case '1':
				tree.update(x - 1, y, [v](Data& val) {
						val.update(1, sm32_1e9_7::verified(v));
						});
				break;
			case '2':
				tree.update(x - 1, y, [v](Data& val) {
						val.update(sm32_1e9_7::verified(v), 0);
						});
				break;
			case '3':
				tree.update(x - 1, y, [v](Data& val) {
						val.update(0, sm32_1e9_7::verified(v));
						});
				break;
			case '4':
				std::cout << tree.sum(x - 1, y) << '\n';
		}
	}

	std::cout << std::flush;
	_exit(0);
}
