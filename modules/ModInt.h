#pragma once

#include <cstdint>
#include <istream>
#include <ostream>

template <typename T, T MOD>
struct StaticModPolicy {
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

template <typename tag = void>
using dm32 = ModInt<uint32_t, uint64_t, DynamicModPolicy<uint32_t, tag>>;

template <typename tag = void>
using dm64 = ModInt<uint64_t, uint64_t, DynamicModPolicy<uint64_t, tag>>;
