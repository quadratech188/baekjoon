#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <vector>

template< typename T1, typename T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& data) {
	is >> data.first >> data.second;
	return is;
}

template <typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& data) {
	for (auto& element: data)
		is >> element;
	return is;
}

namespace Input {
	template<typename T>
	inline std::vector<T> toVec(size_t n) {
		std::vector<T> result(n);
		std::cin >> result;

		return result;
	}
}

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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
