#include <array>
#include <cstdio>
#include <iostream>

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

namespace Math {
	template<typename T>
	T power(T data, long long int exponent) {
		if (exponent == 1) return data;

		T half = power(data, exponent / 2);

		if (exponent % 2 == 0)
			return half * half;
		else
			return half * half * data;
	}

	template <typename T>
	T powerCeil(T power, T n) {
		T result = 1;

		while (result < n) result *= power;
		return result;
	}

	constexpr double pi = 3.1415926535897932384626;
	constexpr double tau = 2 * pi;
}

namespace Math {
template <typename T>
	bool prime(T p) {
		T d = p - 1;
		int s = 0;

		while (d % 2 == 0) {
			s ++;
			d /= 2;
		}

		static std::array<int, 12> bases = {
			2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
		};

		ModInt<T>::setMod(p);

		for (T a: bases) {
			if (a >= p - 1) break;

			ModInt<T> x = power(ModInt<T>(a), d);
			ModInt<T> y = 0;

			for (int i = 0; i < s; i++) {
				y = x * x;
				if (y == 1 && x != 1 && x != p - 1) return false;

				x = y;
			}
			if (y != 1) return false;
		}

		return true;
	}
}

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	int count = 0;

	for (int i = 0; i < n; i++) {
		unsigned long long int s;
		std::cin >> s;
		s = 2 * s + 1;
		if (Math::prime(s)) count++;	
	}
	std::cout << count;
}
