#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& values) {
	if (values.size() == 0) return os;

	for (size_t i = 0; i < values.size() - 1; i++) {
		std::cout << values[i] << ' ';
	}
	std::cout << values[values.size() - 1];

	return os;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& pair) {
	std::cout << pair.first << ' ' << pair.second;

	return os;
}
