#pragma once

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

	constexpr ModInt operator+(ModInt const& other) const noexcept {
		if (value + other.value >= Policy::mod())
			return ModInt(value + other.value - Policy::mod(), raw{});
		else
		 	return ModInt(value + other.value, raw{});
	}

	constexpr ModInt& operator+=(ModInt const& other) noexcept {
		*this = *this + other;
		return *this;
	}

	constexpr ModInt& operator++() noexcept {
		if (value == Policy::mod() - 1)
			value = 0;
		else
		 	++value;
		return *this;
	}

	constexpr ModInt operator*(ModInt const& other) const noexcept {
		return ModInt(static_cast<T2>(value) * other.value % Policy::mod(), raw{});
	}

	constexpr bool operator!=(T const& other) const noexcept {
		return value != other;
	}

	friend std::ostream& operator<<(std::ostream& os, ModInt const& val) {
		os << val.value;
		return os;
	}

	static void set_mod(T val) {
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
