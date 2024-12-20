#include <istream>
#include <vector>
#include <iostream>

template<typename T1, typename T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& data) {
	is >> data.first >> data.second;
	return is;
}

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

