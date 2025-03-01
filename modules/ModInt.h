#pragma once

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

	ModInt operator*(const ModInt& other) const {
		return ModInt(_val * other._val);
	}

	void operator=(const ModInt& other) {
		_val = other._val;
	}

	bool operator==(const ModInt& other) {
		return _val == other._val;
	}

	bool operator!=(const ModInt& other) {
		return _val != other._val;
	}

private:
	T _val;
	static T _mod;
};

template <typename T>
T ModInt<T>::_mod = 0;

using mInt = ModInt<int>;
