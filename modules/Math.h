namespace Math {
	template<typename T>
	T power(T& data, int exponent) {
		if (exponent == 1) return data;

		T half = power(data, exponent / 2);

		if (exponent % 2 == 0)
			return half * half;
		else
			return half * half * data;
	}
}
