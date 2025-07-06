#include "../modules/FFT.h"
#include "../modules/FastIO.h"
#include <iostream>
#include <string>
#include <vector>
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
