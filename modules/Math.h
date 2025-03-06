#include <array>

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

	template <typename T>
	T factorial(T n) {
		T result = 1;
		for (T i = 1; i <= n; i++) result *= i;
		return result;
	}

	constexpr double pi = 3.1415926535897932384626;
	constexpr double tau = 2 * pi;
}
