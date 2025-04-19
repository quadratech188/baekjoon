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

	ModInt(T2 val) noexcept {
		_val = val % Policy::mod();
		if (_val < 0) _val += Policy::mod();
	}
	ModInt() noexcept:
		_val(0) {}

	ModInt operator+(T const& other) const noexcept {
		return ModInt(_val + other);
	}

	ModInt operator+(ModInt const& other) const noexcept {
		return ModInt(_val + other._val);
	}

	ModInt& operator+=(const ModInt& other) noexcept {
		_val += other._val;
		if (_val >= Policy::mod()) _val -= Policy::mod();
		return *this;
	}

	ModInt& operator++(int) noexcept {
		_val = (_val + 1) % Policy::mod();
		return *this;
	}

	template <typename O>
	ModInt operator*(const O& other) const noexcept {
		return ModInt(static_cast<T2>(_val) * other);
	}

	ModInt operator*(const ModInt& other) const noexcept {
		return ModInt(static_cast<T2>(_val) * other._val);
	}

	ModInt& operator*=(const ModInt& other) noexcept {
		*this = ModInt(static_cast<T2>(_val) * other._val);
		return *this;
	}

	void operator=(const ModInt& other) {
		_val = other._val;
	}

	bool operator==(const ModInt& other) const noexcept {
		return _val == other._val;
	}

	bool operator!=(const ModInt& other) const noexcept {
		return _val != other._val;
	}

	bool operator<=(const ModInt& other) const noexcept {
		return _val <= other._val;
	}

	operator T() const noexcept {
		return _val;
	}

	friend std::ostream& operator<<(std::ostream& os, ModInt const& data) {
		os << data._val;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, ModInt& data) {
		T2 temp;
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
