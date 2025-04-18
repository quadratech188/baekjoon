#include <algorithm>
#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <numeric>
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

int main() {
	FastIO();
	long long int n, b, c;
	std::cin >> n >> b >> c;

	auto values = Input::toVec<long long int>(n);
	values.push_back(0);
	values.push_back(0);

	if (b <= c) {
		std::cout << b * std::accumulate(values.begin(), values.end(), 0);
		return 0;
	}

	long long int result = 0;
	for (int i = 0; i < n; i++) {
		if (values[i + 1] > values[i + 2]) {
			long long int delta = std::min(values[i], values[i + 1] - values[i + 2]);
			result += (b + c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;

			delta = std::min(values[i], std::min(values[i + 1], values[i + 2]));
			result += (long long int)(b + 2 * c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;
			values[i + 2] -= delta;
		}
		else {
			long long int delta = std::min(values[i], std::min(values[i + 1], values[i + 2]));
			result += (b + 2 * c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;
			values[i + 2] -= delta;

			delta = std::min(values[i], values[i + 1]);
			result += (long long int)(b + c) * delta;

			values[i] -= delta;
			values[i + 1] -= delta;
		}
		result += b * values[i];
		values[i] = 0;
	}

	std::cout << result;
}
