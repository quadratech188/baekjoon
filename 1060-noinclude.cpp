#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

namespace Input {
	template<typename T>
	inline std::vector<T> inlineToVec(size_t n) {
		std::vector<T> result;
		result.reserve(n);
		for (int i = 0; i < n; i++) {
			T value;
			std::cin >> value;
			result.push_back(value);
		}
		return result;
	}
}

int main() {
	int l;
	std::cin >> l;

	std::vector<int> values = Input::inlineToVec<int>(l);

	int n;
	std::cin >> n;

	std::priority_queue<std::pair<long long int, int>> queue;

	for (int j: values)
		queue.emplace(0, -j);

	values.push_back(0);

	std::sort(values.begin(), values.end());

	for (int i = 0; i < values.size() - 1; i++) {
		int left = values[i];
		int right = values[i + 1];

		if (right - left <= 2 * n + 1) {
			for (long long int j = left + 1; j < right; j++) {
				long long int k = (j - left) * (right - j) - 1;
				queue.emplace(-k, -j);
			}
		}
		else {
			for (long long int j = left + 1; j <= left + n; j++) {
				long long int k = (j - left) * (right - j) - 1;
				queue.emplace(-k, -j);
			}
			
			for (long long int j = right - n; j < right; j++) {
				long long int k = (j - left) * (right - j) - 1;
				queue.emplace(-k, -j);
			}
		}
	}

	for (int j = values[values.size() - 1] + 1; j <= values[values.size() - 1] + n; j++) {
		queue.emplace(std::numeric_limits<long long int>::min(), -j);
	}

	for (int i = 0; i < n; i++) {
		std::pair<int, int> temp = queue.top();
		queue.pop();

		std::cout << - temp.second << ' ';
	}
}
