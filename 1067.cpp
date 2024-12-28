#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <iostream>
#include <vector>

typedef std::complex<double> Complex;

typedef std::vector<Complex> Polynomial;

void fft(Polynomial& coefficients, Polynomial& roots) {
	int length = coefficients.size();
	if (length <= 1) return;

	Polynomial even(length / 2);
	Polynomial odd(length / 2);
	Polynomial half_roots(length / 2);

	for (int i = 0; i < length / 2; i++) {
		even[i] = coefficients[2 * i];
		odd[i] = coefficients[2 * i + 1];
		half_roots[i] = roots[2 * i];
	}

	fft(even, half_roots);
	fft(odd, half_roots);

	for (int i = 0; i < length / 2; i++) {
		Complex temp = odd[i] * roots[i];
		coefficients[i] = even[i] + temp;
		coefficients[i + length / 2] = even[i] - temp;
	}
}

Polynomial roots(int n) {
	Polynomial result;
	result.reserve(n);

	double temp = Math::tau / n;

	for (int i = 0; i < n; i++) {
		result.push_back(std::polar(1.0, i * temp));
	}

	return result;
}

int main() {
	int n;
	std::cin >> n;
	
	int length = Math::powerCeil(2, 2 * n);

	std::vector<Complex> a(length), b(length);

	for (int i = 0; i < n; i++) {
		int temp;
		std::cin >> temp;
		a[i] = temp;
		a[i + n] = temp;
	}

	for (int i = n - 1; i >= 0; i--) {
		int temp;
		std::cin >> temp;
		b[i] = temp;
	}

	Polynomial r = roots(length);

	fft(a, r);
	fft(b, r);

	for (int i = 0; i < length; i++) {
		a[i] *= b[i];
	}

	fft(a, r); // reverse
	
	int max = 0;
	for (int i = 0; i < length; i++) {
		max = std::max(max, (int)std::round(a[i].real() / length));
	}
	std::cout << max;
}
