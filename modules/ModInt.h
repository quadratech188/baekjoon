#pragma once

#include <istream>
#include <ostream>
template <typename T, typename T2>
class ModInt {
public:
	static void setMod(T mod) {
		_mod = mod;
	}

	ModInt(T2 val) {
		_val = val % _mod;
		if (_val < 0) _val += _mod;
	}
	ModInt():
		_val(0) {}

	ModInt operator+(T other) {
		return ModInt((T2)_val + other);
	}

	ModInt& operator+=(const ModInt& other) {
		*this = ModInt((T2)_val + other._val);
		return *this;
	}

	ModInt& operator++(int) {
		_val = (_val + 1) % _mod;
		return *this;
	}

	ModInt operator*(const ModInt& other) const {
		return ModInt((T2)_val * other._val);
	}

	ModInt& operator*=(const ModInt& other) {
		*this = ModInt((T2)_val * other._val);
		return *this;
	}

	void operator=(const ModInt& other) {
		_val = other._val;
	}

	bool operator==(const ModInt& other) const {
		return _val == other._val;
	}

	bool operator!=(const ModInt& other) const {
		return _val != other._val;
	}

	bool operator<=(const ModInt& other) const {
		return _val <= other._val;
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

private:
	T _val;
	static T _mod;
};

template <typename T, typename T2>
T ModInt<T, T2>::_mod = 0;

using mInt = ModInt<unsigned int, unsigned long long int>;
using mLL = ModInt<unsigned long long int, unsigned long long int>;
