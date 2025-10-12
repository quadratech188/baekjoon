#include <cstdint>
using uint = unsigned int;
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
		inline istream& operator>>(std::tuple<Ts...>& tuple){
			std::apply([this](auto&... x) {(((*this) >> x), ...);}, tuple);
			return *this;
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
#include <ranges>
#include <vector>

#include <cmath>

template<typename T, typename T2 = T>
struct Vec2 {
	using type = T;
	
	T x, y;

	constexpr Vec2(T x, T y) noexcept: x(x), y(y) {}
	constexpr Vec2() noexcept : x(T()), y(T()) {}

	constexpr Vec2 operator+(const Vec2& other) const noexcept {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	constexpr Vec2& operator+=(Vec2 const& other) noexcept {
		x += other.x;
		y += other.y;
		return *this;
	}

	constexpr Vec2 operator-(const Vec2& other) const noexcept {
		return Vec2(this->x - other.x, this->y - other.y);
	}

	constexpr Vec2& operator-=(Vec2 const& other) noexcept {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename D>
	constexpr Vec2<D> operator/(const D other) const noexcept {
		return Vec2<D>(this->x / other, this->y / other);
	}

	constexpr Vec2 operator*(T const& other) const noexcept {
		return {x * other, y * other};
	}

	constexpr friend Vec2 operator*(T const& l, Vec2<T> const& r) noexcept {
		return {l * r.x, l * r.y};
	}

	constexpr bool operator<(const Vec2& other) const noexcept {
		if (this->x != other.x) return this->x < other.x;
		return this->y < other.y;
	}

	constexpr bool operator==(const Vec2& other) const noexcept {
		return this->x == other.x && this->y == other.y;
	}

	constexpr bool operator!=(const Vec2& other) const noexcept {
		return this->x != other.x || this->y != other.y;
	}

	constexpr Vec2 rotate(const double angle) const noexcept {
		return Vec2(
				x * std::cos(angle) - y * std::sin(angle),
				x * std::sin(angle) + y * std::cos(angle)
				);
	}

	constexpr T2 dot(const Vec2& other) const noexcept {
		return static_cast<T2>(this->x) * other.x
			+ static_cast<T2>(this->y) * other.y;
	}

	constexpr T2 cross(const Vec2& other) const noexcept {
		return static_cast<T2>(this->x) * other.y
			- static_cast<T2>(this->y) * other.x;
	}

	constexpr T2 size2() const noexcept {
		return static_cast<T2>(this->x) * this->x
			+ static_cast<T2>(this->y) * this->y;
	}

	constexpr auto length() const noexcept {
		return std::sqrt(size2());
	}

	constexpr auto theta() const noexcept {
		return std::atan2(y, x);
	}

	constexpr T taxi_distance(const Vec2& other) const noexcept {
		return std::abs(x - other.x) + std::abs(y - other.y);
	}

	constexpr static Vec2 zero() noexcept {
		return Vec2(0, 0);
	}

	constexpr static Vec2 one() noexcept {
		return Vec2(1, 1);
	}

	constexpr static Vec2 i() noexcept {
		return Vec2(1, 0);
	}

	constexpr static Vec2 j() noexcept {
		return Vec2(0, 1);
	}
};

template <typename IS, typename T, typename T2>
IS& operator>>(IS& is, Vec2<T, T2>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

template <typename OS, typename T, typename T2>
OS& operator<<(OS& os, Vec2<T, T2>& vec2) {
	os << vec2.x << ' ' << vec2.y;
	return os;
}

typedef Vec2<int32_t, int64_t> Int2;
typedef Vec2<double, double> Double2;


template <typename T>
class Matrix {
public:
	Matrix(int i, int j, T const& default_value = {}):
		Matrix({i, j}, default_value) {}

	Matrix(Int2 size, T const& default_value = {}):
		_size(size), _values(size.x * size.y, default_value) {}

	template <std::ranges::sized_range R>
	Matrix(int i, int j, R const& range):
		Matrix({i, j}, range) {}

	template <std::ranges::sized_range R>
	Matrix(Int2 size, R const& range):
		_size(size), _values(range.begin(), range.end()) {}
private:
	Int2 _size;
	std::vector<T> _values;

public:
	Int2 size() {
		return _size;
	}

	T& operator()(int i, int j) {
		return _values[_size.y * i + j];
	}
	T const& operator()(int i, int j) const {
		return _values[_size.y * i + j];
	}

	Matrix operator+(Matrix const& other) {
		Matrix result(_size);

		for (uint i = 0; i < _values.size(); i++) {
			result._values[i] = _values[i] + other._values[i];
		}
		return result;
	}

	Matrix operator*(Matrix const& other) {
		int depth = _size.y = other._size.x;

		Matrix result(_size.x, other._size.y, 0);

		for (int i = 0; i < result._size.x; i++) {
			for (int j = 0; j < result._size.y; j++) {
				for (int k = 0; k < depth; k++) {
					result(i, j) += (*this)(i, k) * other(k, j);
				}
			}
		}
		return result;
	}

	Matrix identity() {
		int size = _size.x = _size.y;
		Matrix result(size, size, 0);
		for (int i = 0; i < size; i++) {
			result(i, i) = 1;
		}
		return result;
	}

	template <typename IS>
	friend IS& operator>>(IS& is, Matrix& matrix) {
		for (auto& val: matrix._values)
			is >> val;
		return is;
	}

	template <typename OS>
	friend OS& operator<<(OS& os, Matrix const& matrix) {
		for (int i = 0; i < matrix._size.x; i++) {
			for (int j = 0; j < matrix._size.y; j++) {
				os << matrix(i, j) << ' ';
			}
			os << '\n';
		}
		return os;
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
#include <algorithm>
#include <iostream>
#include <limits>

struct Element {
	int value;
	int lazy;

	Element(int x = std::numeric_limits<int>::max()):
		value(x), lazy(std::numeric_limits<int>::max()) {}

	int get() const {
		return std::min(value, lazy);
	}

	void apply() {
		value = get();
	}

	Element operator+(Element const& other) const {
		return Element(std::min(get(), other.get()));
	}

	void propagate(Element& l, Element& r) {
		l.lazy = std::min(l.lazy, lazy);
		r.lazy = std::min(r.lazy, lazy);
	}
	
	void reinit(Element const& l, Element const& r) {
		value = std::min(l.value, r.value);
		lazy = std::numeric_limits<int>::max();
	}
};

/*
 * rows [a, b]:
 * we only care about sequential edges
 *
 * S = (b - a) * (col[i] - col[i - 1])
 *
 * scan?
 * figure out next col: total m
 * fill in: total m log n (lazy seg)
 *
 * total: n * n * m log n
 *
 * maybe???
 */

void loop() {
	int n, m;
	Fast::cin >> n >> m;

	Matrix<char> grid(0, 0);

	bool rotated = n > m;
	if (rotated) {
		std::swap(n, m);

		grid = Matrix<char>(n, m);
		for (int j = 0; j < m; j++) {
			for (int i = 0; i < n; i++) {
				Fast::cin >> grid(i, j);
			}
		}
	}
	else {
		grid = Matrix<char>(n, m, Fast::cin.to_range<char>(n * m));
	}

	Matrix<int> last_edge(n, n, -1);

	std::vector<LazySegmentTree<Element>> trees(m, LazySegmentTree<Element>(n));

	for (int col = 0; col < m; col++) {
		for (int l = 0; l < n; l++) {
			for (int r = l + 1; r < n; r++) {
				if (grid(l, col) == '0' || grid(r, col) == '0') continue;

				int prev_col = last_edge(l, r);
				last_edge(l, r) = col;
				
				if (prev_col == -1) continue;

				int size = (r - l + 1) * (col - prev_col + 1);

				for (int i = prev_col; i <= col; i++) {
					trees[i].update(l, r + 1, [size](Element& e) {
							e.lazy = std::min(e.lazy, size);
							});
				}
			}
		}
	}
	Matrix<int> result(n, m);
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			int x = trees[j].at(i).get();
			result(i, j) = x == std::numeric_limits<int>::max()? 0: x;
		}
	}
	if (rotated) {
		for (int j = 0; j < m; j++) {
			for (int i = 0; i < n; i++) {
				std::cout << result(i, j) << ' ';
			}
			std::cout << '\n';
		}
	}
	else {
		std::cout << result;
	}
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
