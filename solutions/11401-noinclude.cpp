#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>

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
		for (T i = 1; i < n; ++i) result *= i;
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

int main() {
	uint n_raw, k_raw;
	std::cin >> n_raw >> k_raw;

	if (2 * k_raw > n_raw)
		k_raw = n_raw - k_raw;

	sm32_1e9_7 n(n_raw), k(k_raw);

	std::cout << Math::C(n, k);
}
