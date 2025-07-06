#include "../modules/FFT.h"
#include "../modules/Input.h"
#include "../modules/Output.h"

int main() {
	size_t n;
	std::cin >> n;

	size_t length = Math::powerCeil(size_t(2), 2 * n);

	std::vector<double> x(length);

	for (size_t i = 0; i < n; i++) {
		std::cin >> x[i];
		x[i + n] = x[i];
	}

	std::vector<double> y(length);

	for (int i = n - 1; i >= 0; i--) {
		std::cin >> y[i];
	}

	FFT<double> fft(length);

	auto x_fft = fft.fft(x);
	auto y_fft = fft.fft(y);

	std::cout << x_fft << '\n' << y_fft << '\n';

	for (size_t i = 0; i < length; i++)
		x_fft[i] *= y_fft[i];

	std::cout << x_fft << '\n';

	auto product = fft.ifft(x_fft);

	std::cout << product << '\n';
}
