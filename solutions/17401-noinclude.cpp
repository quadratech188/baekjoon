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

#include <functional>
#include <iostream>
#include <numeric>
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

#include <cmath>
#include <iterator>

template <typename V2>
class Bounds2 {

public:
	Bounds2(V2 start, V2 end): left(start), right(end) {}
	Bounds2(V2 end): left(V2::zero()), right(end) {}
	Bounds2(): left(V2::zero()), right(V2::zero()) {}

	struct iterator {
		using value_type = V2;
		using difference_type = std::ptrdiff_t;
		using pointer = const V2*;
		using reference = const V2&;
		using iterator_category = std::forward_iterator_tag;

		iterator(V2 pos, const Bounds2& bounds): pos(pos), bounds(bounds) {}
		V2 operator*() {
			return pos;
		}

		bool operator==(const iterator& other) {
			return pos == other.pos;
		}

		bool operator!=(const iterator& other) {
			return pos != other.pos;
		}

		iterator& operator++() {
			pos.x ++;
			if (pos.x == bounds.right.x) {
				pos.x = bounds.left.x;
				pos.y ++;
			}
			return *this;
		}
		
	private:
		V2 pos;
		const Bounds2& bounds;
	};

	iterator begin() const {
		return iterator(left, *this);
	}

	iterator end() const {
		return iterator({left.x, right.y}, *this); // so ugly
	}

	bool contains(V2 pos) const {
		return left.x <= pos.x && pos.x < right.x
			&& left.y <= pos.y && pos.y < right.y;
	}

	Bounds2 pad(V2 start, V2 end) {
		return Bounds2(left + start, right - end);
	}

	V2::type& x1() {
		return left.x;
	}
	V2::type& x2() {
		return right.x;
	}
	V2::type& y1() {
		return left.y;
	}
	V2::type& y2() {
		return right.y;
	}

	template <typename T>
	friend std::istream& operator>>(std::istream& is, Bounds2<T>& bounds2) {
		is >> bounds2.left >> bounds2.right;
		return is;
	}

private:
	V2 left, right;
};


typedef Bounds2<Int2> Range2;

template <typename T>
class Matrix {
public:
	using value_type = T;

	Matrix(int columns, int rows, T defaultValue = T()):
		_values(columns * rows, defaultValue), _size(columns, rows) {}

	Matrix(): _values(), _size(Int2::zero()) {}

	static Matrix identity(int n) {
		Matrix result(n, n, 0);
		for (int i = 0; i < n; i++)
			result(i, i) = 1;

		return result;
	}

	Matrix identity() const {
		return identity(this->_size.x);
	}

	T& operator()(int column, int row) {
		return this->_values[row * _size.x + column];
	}

	const T& operator()(int column, int row) const {
		return this->_values[row * _size.x + column];
	}

	T& operator[](Int2 index) {
		return this->_values[index.y * _size.x + index.x];
	}

	bool operator==(const Matrix<T>& other) const {
		return _size == other._size && _values == other._values;
	}
	
	bool operator<(const Matrix<T>& other) const {
		if (_size != other._size) return _size < other._size;

		return _values < other._values;
	}

	Matrix operator+(Matrix const& other) const {
		Matrix result(_size.x, _size.y);
		for (int i = 0; i < _values.size(); i++)
			result._values[i] = _values[i] + other._values[i];

		return result;
	}

	Matrix& operator+=(Matrix const& other) {
		for (int i = 0; i < _values.size(); i++)
			_values[i] += other._values[i];

		return *this;
	}

	Matrix& operator+=(T const& other) {
		for (auto& element: _values)
			element += other;

		return *this;
	}

	Matrix operator*(const Matrix& other) const {
		Matrix result(other._size.x, _size.y);

		for (Int2 index: result.bounds()) {
			T sum = 0;
			for (int depth = 0; depth < _size.x; depth++) {
				sum += (*this)(depth, index.y) * other(index.x, depth);
			}
			result[index] = sum;
		}

		return result;
	}

	Int2 size() const {
		return _size;
	}

	Range2 bounds() const {
		return Range2(Int2::zero(), _size);
	}

	int rawIndex(Int2 const index) const {
		return index.y * _size.x + index.x;
	}

	T sum() const {
		return std::accumulate(_values.begin(), _values.end(), T());
	}

private:
	std::vector<T> _values;
	Int2 _size;
};

template<typename CIN, typename T>
CIN& operator>>(CIN& input, Matrix<T>& matrix) {
	for (Int2 index: matrix.bounds())
		input >> matrix[index];

	return input;
}

template <typename T>
std::ostream& operator<<(std::ostream& output, Matrix<T> const & matrix) {
	for (int j = 0; j < matrix.size().y; j++) {
		output << matrix(0, j);
		for (int i = 1; i < matrix.size().x; i++)
			output << ' ' << matrix(i, j);
		output << '\n';
	}
	return output;
}

#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>


#include <array>
#include <cstdlib>

namespace Math {
	template<typename T>
	T power(T data, size_t exponent, T&& identity = 1) {
		T result = identity;

		while (exponent > 0) {
			if (exponent & 1)
				result = data * result;
			data = data * data;
			exponent /= 2;
		}
		return result;
	}

	template <typename T>
	T powerCeil(T power, T n) {
		T result = 1;

		while (result < n) result *= power;
		return result;
	}

	template <typename T>
	T factorial(T n) {
		T result = 1;
		for (T i = 1; i <= n; ++i) result *= i;
		return result;
	}

	template <typename T>
	T C(T n, T k) {
		T dividend = 1;
		T divisor = 1;
		for (T i = 0; i < k; ++i) {
			dividend *= (n - i);
			divisor *= (i + 1);
		}

		return dividend / divisor;
	}

	template <typename T>
	std::pair<T, T> quotient_remainder(T a, T b) {
		T quotient = a / b;
		T remainder = a % b;
		if (remainder < 0) {
			remainder += std::abs(b);
			quotient --;
		}
		return std::make_pair(quotient, remainder);
	}

	constexpr double pi = 3.1415926535897932384626;
	constexpr double tau = 2 * pi;
}

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
	template <typename U>
	constexpr ModInt(U val) noexcept {
		if (val < 0) val += Policy::mod();
		val %= Policy::mod();
		value = val;
	}

	constexpr ModInt() noexcept {}

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

	constexpr T2 big_val() const noexcept {
		return static_cast<T2>(value);
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
			return ModInt(value + Policy::mod() - other.value, raw{});
		else
		 	return ModInt(value - other.value, raw{});
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

	constexpr inline ModInt operator/(ModInt const& other) const noexcept {
		return *this * other.inverse();
	}

	constexpr inline ModInt& mul_add(ModInt const& a, ModInt const& b) noexcept {
		value = (static_cast<T2>(value) * a.value + b.value) % Policy::mod();
		return *this;
	}

	constexpr inline ModInt inverse() const noexcept {
		return Math::power<ModInt>(*this, Policy::mod() - 2);
	}

	constexpr inline bool operator!=(T const& other) const noexcept {
		return value != other;
	}
	constexpr inline bool operator!=(ModInt const& other) const noexcept {
		return value != other.value;
	}

	constexpr inline bool operator==(T const& other) const noexcept {
		return value == other;
	}
	
	constexpr inline bool operator<(ModInt const& other) const noexcept {
		return value < other.value;
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
#include <iostream>

int main() {
	uint t, n, d;
	Fast::cin >> t >> n >> d;

	std::vector<Matrix<sm32_1e9_7>> maps(t, Matrix<sm32_1e9_7>(n, n, 0));

	for (uint i = 0; i < t; i ++) {
		uint m;
		Fast::cin >> m;
		for (uint _ = 0; _ < m; _ ++) {
			uint a, b, c;
			Fast::cin >> a >> b >> c;
			maps[i](b - 1, a - 1) = c;
		}
	}
	
	auto remainder = Matrix<sm32_1e9_7>::identity(n);
	for (uint i = 0; i < d % t; i++)
		remainder = remainder * maps[i];

	auto total = remainder;
	for (uint i = d % t; i < t; i++)
		total = total * maps[i];

	auto result = Math::power(total, d / t, Matrix<sm32_1e9_7>::identity(n)) * remainder;

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < n; j++)
			std::cout << result(j, i) << ' ';
		std::cout << '\n';
	}
}
