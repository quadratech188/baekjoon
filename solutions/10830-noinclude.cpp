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

#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>


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
		val %= Policy::mod();
		if (val < 0) val += Policy::mod();
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

	template <typename IS>
	friend IS& operator>>(IS& is, ModInt& x) {
		T v;
		is >> v;
		x = {v};
		return is;
	}

	inline friend std::ostream& operator<<(std::ostream& os, ModInt const& val) {
		os << val.value;
		return os;
	}

	static inline void set_mod(T val) {
		Policy::mod() = val;
	}
};

template <uint16_t MOD>
using sm16 = ModInt<uint16_t, uint32_t, StaticModPolicy<uint16_t, MOD>>;

template <uint32_t MOD>
using sm32 = ModInt<uint32_t, uint64_t, StaticModPolicy<uint32_t, MOD>>;

template <uint64_t MOD>
using sm64 = ModInt<uint64_t, uint64_t, StaticModPolicy<uint64_t, MOD>>;

using sm16_1e4_7 = sm16<10'007>;
using sm32_1e9_7 = sm32<1'000'000'007>;
using sm64_1e9_7 = sm64<1'000'000'007>;

template <typename tag = void>
using dm32 = ModInt<uint32_t, uint64_t, DynamicModPolicy<uint32_t, tag>>;

template <typename tag = void>
using dm64 = ModInt<uint64_t, uint64_t, DynamicModPolicy<uint64_t, tag>>;
#include <iostream>

using sm32_1e3 = sm32<1000>;

int main() {
	int n;
	int64_t b;
	Fast::cin >> n >> b;

	Matrix<sm32_1e3> m(n, n, Fast::cin.to_range<sm32_1e3>(n * n));

	std::cout << Math::power(m, b, m.identity());
}
