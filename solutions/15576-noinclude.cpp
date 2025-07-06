#include <array>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

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

template <typename T = double>
class FFT {
public:
	using value_t = T;
	using complex_t = std::complex<T>;

	FFT(size_t n): _size(n), _roots(n + 1), a(n), b(n), primary(a), secondary(b) {
		complex_t unit(0, -Math::tau / n);

		for (size_t i = 0; i < n + 1; i++)
			_roots[i] = std::exp(unit * static_cast<double>(i));
	}

private:
	size_t _size;
	std::vector<complex_t> _roots;

	std::vector<complex_t> a;
	std::vector<complex_t> b;

	std::vector<complex_t>& primary;
	std::vector<complex_t>& secondary;

public:
	template <std::ranges::range R>
	std::vector<complex_t> fft(R&& values) {
		return fft(values, 1, 0);
	}

	template <std::ranges::range R>
	std::vector<complex_t> ifft(R&& values) {
		return ifft(values, 1, 0);
	}

private:
	template <std::ranges::range R>
	std::vector<complex_t> fft(R&& values, size_t step, size_t offset) {
		// https://en.wikipedia.org/wiki/Cooley-Tukey_FFT_algorithm
		
		if (step == _size) {
			return {values[offset]};
		}
		
		auto E_k = fft(values, 2 * step, offset);
		auto O_k = fft(values, 2 * step, offset + step);

		std::vector<complex_t> X_k(_size / step);

		for (size_t i = 0; i < X_k.size() / 2; i++) {
			X_k[i] = E_k[i] + _roots[step * i] * O_k[i];
			X_k[i + X_k.size() / 2] = E_k[i] - _roots[step * i] * O_k[i];
		}

		return X_k;
	}

	template <std::ranges::range R>
	std::vector<complex_t> ifft(R&& values, size_t step, size_t offset) {
		// https://en.wikipedia.org/wiki/Cooley-Tukey_FFT_algorithm
		
		if (step == _size) {
			return {values[offset] / T(_size)};
		}
		
		auto E_k = ifft(values, 2 * step, offset);
		auto O_k = ifft(values, 2 * step, offset + step);

		std::vector<complex_t> X_k(_size / step);

		for (size_t i = 0; i < X_k.size() / 2; i++) {
			X_k[i] = E_k[i] + _roots[_size - step * i] * O_k[i];
			X_k[i + X_k.size() / 2] = E_k[i] - _roots[_size - step * i] * O_k[i];
		}

		return X_k;
	}
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	std::string a, b;
	std::cin >> a >> b;

	if (a == "0") {
		std::cout << 0;
		return 0;
	}
	if (b == "0") {
		std::cout << 0;
		return 0;
	}

	size_t length = a.size() + b.size();

	length = Math::powerCeil(size_t(2), length);

	FFT<double> fft(length);

	std::vector<double> a_vec(length);
	for (size_t i = 0; i < a.size(); i++)
		a_vec[a.size() - i - 1] = double(a[i] - '0');

	std::vector<double> b_vec(length);
	for (size_t i = 0; i < b.size(); i++)
		b_vec[a.size() - i - 1] = double(b[i] - '0');

	auto a_fft = fft.fft(a_vec);
	auto b_fft = fft.fft(b_vec);

	for (size_t i = 0; i < length; i++)
		a_fft[i] *= b_fft[i];

	auto result = fft.ifft(a_fft);

	std::vector<int> buffer(length);

	for (size_t i = 0; i < length; i++)
		buffer[i] = std::round(result[i].real());

	for (size_t i = 0; i < length - 1; i++) {
		buffer[i + 1] += buffer[i] / 10;
		buffer[i] %= 10;
	}

	bool leading_zeros = true;
	for (int i = length - 1; i >= 0; i--) {
		if (buffer[i] != 0) leading_zeros = false;

		if (leading_zeros) continue;

		std::cout << buffer[i];
	}
}
