#include <complex>
#include <valarray>

#include "Math.h"

class FFT {
private:
	typedef std::complex<double> Complex;

public:
	typedef std::valarray<Complex> Result;

	FFT(int n): _original(n), _n(Math::powerCeil(2, n)), _roots(_n) {

		double step = Math::tau / _n;

		for (int i = 0; i < n; i++) {
			_roots[i] = std::polar(1.0, step * i);
		}
	}

	template <typename Iterable>
	Result operator()(Iterable values) {
		std::valarray<Complex> temp(_n);
		for (int i = 0; i < values.size(); i++) {
			temp[i] = values[i];
		}

		return fft(temp, 0, 1);
	}

	std::valarray<double> inverse(Result input) {
		Result temp = fft(input, 0, 1);
		std::valarray<double> result;
		for (int i = 0; i < _original; i++)
			result[i] = temp[i].real();

		return result;
	}

private:
	Result fft(std::valarray<Complex>& coefficients, int offset, int step) {
		int length = coefficients.size() / step;
		if (length <= 1) return {coefficients[offset]};

		Result even = fft(coefficients, offset, 2 * step);
		Result odd = fft(coefficients, offset + step, 2 * step);

		Result result(length);

		for (int i = 0; i < length / 2; i++) {
			Complex temp = _roots[i * step] * odd[i];
			result[i] = even[i] + temp;
			result[i + length / 2] = even[i] - temp;
		}

		return result;
	}

	int _n;
	int _original;
	std::valarray<Complex> _roots;
};
