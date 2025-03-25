#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
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
	long long int n, m;
	std::cin >> n >> m;

	std::vector<long long int> periods = Input::toVec<long long int>(m);

	if (n < m) {
		std::cout << n;
		return 0;
	}

	long long int left = 0;
	long long int right = std::numeric_limits<long long int>::max();

	long long int result;
	while (left <= right) {
		long long int children = 0;
		long long int center = (left + right) / 2;

		long long int sum = 0;
		for (auto period: periods)
			sum += 1 + (center / period);

		if (sum >= n) {
			right = center - 1;
			result = center;
		}
		else
		 	left = center + 1;
	}
	long long int sum = m;
	for (auto period: periods)
		sum += (result - 1) / period;

	for (long long int i = 0; i < m; i++) {
		if (left % periods[i] == 0)
			sum++;

		if (sum == n) {
			std::cout << i + 1;
			return 0;
		}
	}
}
