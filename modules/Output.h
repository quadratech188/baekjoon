#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& values) {
	if (values.size() == 0) return os;

	for (int i = 0; i < values.size() - 1; i++) {
		std::cout << values[i] << ' ';
	}
	std::cout << values[values.size() - 1];

	return os;
}
