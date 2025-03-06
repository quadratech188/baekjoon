#pragma once

#include <ostream>
template <typename T>
class ModInt {
public:
	static void setMod(T mod) {
		_mod = mod;
	}

	ModInt(T val) {
		_val = val % _mod;
		if (_val < 0) _val += _mod;
	}

	ModInt operator+(T other) {
		return ModInt(_val + other);
	}

	ModInt& operator++(int) {
		_val = (_val + 1) % _mod;
		return *this;
	}

	ModInt operator*(const ModInt& other) const {
		return ModInt(_val * other._val);
	}

	ModInt& operator*=(const ModInt& other) {
		*this = ModInt(_val * other._val);
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

	friend std::ostream& operator<<(std::ostream& os, ModInt& data) {
		os << data._val;
		return os;
	}

private:
	T _val;
	static T _mod;
};

template <typename T>
T ModInt<T>::_mod = 0;

using mInt = ModInt<unsigned int>;
using mLL = ModInt<unsigned long long int>;
