#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <locale>
#include <ostream>
#include <ranges>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

template <typename T>
inline auto InputRange(size_t n, std::istream& is = std::cin) {
	return std::views::iota(static_cast<size_t>(0), n)
		| std::views::transform([&is](size_t) {
				T temp;
				std::cin >> temp;
				return temp;
				});
}

/*
template <typename T>
class InputRange: public std::ranges::view_interface<InputRange<T>> {
public:
	InputRange(size_t size, std::istream& is = std::cin):
		is(&is), _size(size) {}

	InputRange():
		_size(0), is(nullptr) {}

private:
	std::istream* is;
	size_t _size;

public:
	class iterator {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;

		iterator(std::istream* is, size_t index):
			is(is), index(index) {
			++(*this);
		}

		iterator():
			is(nullptr), index(0) {}

	private:
		std::istream* is;
		size_t index;

	public:
		T operator*() const {
			T value;
			(*is) >> value;
			return value;
		}

		iterator& operator++() {
			return *this;
		}

		iterator operator++(int) {
			iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator!=(iterator const& other) const {
			return index != other.index;
		}

		bool operator==(iterator const& other) const {
			return index == other.index;
		}
	};

	iterator begin() const {
		return iterator(is, 0);
	}

	iterator end() const {
		return iterator(is, _size);
	}

	size_t size() const noexcept {
		return _size;
	}
};
*/

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

	T sum(Segment const query, Segment const segment, size_t const index) {
		if (query.includes(segment))
			return _values[index];

		size_t const left = index * 2 + 1;
		size_t const right = index * 2 + 2;

		_values[index].resolve(_values[left], _values[right]);

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
			if constexpr (std::same_as<std::invoke_result_t<decltype(func), decltype(_values[value_index])>,
					bool>) {
				if (!std::invoke(func, _values[value_index]) || segment.size() == 1)
					return;
			} else {
				std::invoke(func, _values[value_index]);
				return;
			}
		}

		size_t const left = value_index * 2 + 1;
		size_t const right = value_index * 2 + 2;

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

template <typename T, T MOD>
struct StaticModPolicy {
	static constexpr T mod() {
		return MOD;
	}
};

template <typename T, int tag>
struct DynamicModPolicy {
	static T& mod() {
		static T value = 0;
		return value;
	}
};

template <typename T, typename T2, typename Policy>
class ModInt {
public:

	constexpr ModInt(T2 val) noexcept {
		_val = val % Policy::mod();
		if (_val < 0) _val += Policy::mod();
	}
	constexpr ModInt() noexcept:
		_val(0) {}

	constexpr ModInt operator+(T const& other) const noexcept {
		return ModInt(_val + other);
	}

	constexpr ModInt operator+(ModInt const& other) const noexcept {
		return ModInt(_val + other._val);
	}

	constexpr ModInt& operator+=(const ModInt& other) noexcept {
		_val += other._val;
		if (_val >= Policy::mod()) _val -= Policy::mod();
		return *this;
	}

	constexpr ModInt& operator++(int) noexcept {
		_val = (_val + 1) % Policy::mod();
		return *this;
	}

	template <typename O>
	constexpr ModInt operator*(const O& other) const noexcept {
		return ModInt(static_cast<T2>(_val) * other);
	}

	constexpr ModInt operator*(const ModInt& other) const noexcept {
		return ModInt(static_cast<T2>(_val) * other._val);
	}

	constexpr ModInt& operator*=(const ModInt& other) noexcept {
		*this = ModInt(static_cast<T2>(_val) * other._val);
		return *this;
	}

	constexpr ModInt& operator=(const ModInt& other) = default;

	constexpr bool operator==(const ModInt& other) const noexcept {
		return _val == other._val;
	}

	constexpr bool operator!=(const ModInt& other) const noexcept {
		return _val != other._val;
	}

	constexpr bool operator<=(const ModInt& other) const noexcept {
		return _val <= other._val;
	}

	constexpr operator T() const noexcept {
		return _val;
	}

	friend std::ostream& operator<<(std::ostream& os, ModInt const& data) {
		os << data._val;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, ModInt& data) {
		T temp;
		is >> temp;
		data = ModInt(temp);
		return is;
	}

	static void setMod(T mod) {
		Policy::mod() = mod;
	}

private:
	T _val;
};

template <uint32_t MOD>
using sm32 = ModInt<uint32_t, uint64_t, StaticModPolicy<uint32_t, MOD>>;

template <uint64_t MOD>
using sm64 = ModInt<uint64_t, uint64_t, StaticModPolicy<uint64_t, MOD>>;

using sm32_1e9_7 = sm32<1'000'000'007>;
using sm64_1e9_7 = sm64<1'000'000'007>;

template <int tag = 0>
using dm32 = ModInt<uint32_t, uint64_t, DynamicModPolicy<uint32_t, tag>>;

template <int tag = 0>
using dm64 = ModInt<uint64_t, uint64_t, DynamicModPolicy<uint64_t, tag>>;

struct Update {
	sm32_1e9_7 a, b;
};

struct Data {
	sm32_1e9_7 a;
	sm32_1e9_7 b;
	sm32_1e9_7 _value;
	size_t length;

	Data(sm32_1e9_7 value = 0, size_t length = 1):
		a(1), b(0), _value(value), length(length) {}
	
	constexpr sm32_1e9_7 value() const {
		return this->a * this->_value + this->b * this->length;
	}

	Data operator+(const Data& other) const {
		return Data(
				value() + other.value(),
				length + other.length
				);
	}

	inline void update(Update update) {
		a *= update.a;
		b *= update.a;
		b += update.b;
	}

	inline void resolve(Data& child1, Data& child2) {

		child1.a *= a;
		
		child1.b *= a;
		child1.b += b;

		child2.a *= a;
		
		child2.b *= a;
		child2.b += b;

		_value = value();
		a = 1;
		b = 0;
	}
};

int main() {
	FastIO();
	int n;
	std::cin >> n;

	LazySegmentTree<Data> root{InputRange<int>(n)};

	int m;
	std::cin >> m;

	for (int i = 0; i < m; i++) {
		int type;
		size_t x, y;
		std::cin >> type >> x >> y;
		if (type != 4) {
			Update update;
			int v;
			std::cin >> v;
			switch(type) {
				case 1:
					update = {1, v}; break;
				case 2:
					update = {v, 0}; break;
				case 3:
					update = {0, v}; break;
			}
			root.update(x - 1, y, [update](Data& val) {
					val.update(update);
					});
		}
		else {
			std::cout << root.sum(x - 1, y).value() << '\n';
		}
	}
}
