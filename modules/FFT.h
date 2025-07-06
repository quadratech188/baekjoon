#include <cmath>
#include <complex>
#include <ranges>
#include <vector>

#include "Math.h"

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
