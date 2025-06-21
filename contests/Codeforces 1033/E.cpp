#include "../../modules/Input.h"
#include "../../modules/FastIO.h"
#include <algorithm>
#include <cstdint>

void loop() {
	uint32_t n;
	int k;
	std::cin >> n >> k;

	auto cars = Input::toVec<int>(n);

	int64_t delta = 0;
	while (true) {
		auto min = std::min_element(cars.begin(), cars.end());
		auto max = std::max_element(cars.begin(), cars.end());

		if (min == max) break;

		if (-(*max) + (*min + 1) + k < 0) {
			(*min)++;
			(*max)--;
			delta += k;
		}
		else {
			break;
		}
	}

	int64_t result = 0;

	for (int length: cars)
		result += (int64_t)length * (length + 1) / 2;

	std::cout << result + delta << '\n';
}

int main() {
	FastIO();
	uint32_t t;
	std::cin >> t;
	for (uint32_t i = 0; i < t; ++i) loop();
}
