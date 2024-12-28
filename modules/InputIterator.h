#include <iostream>

template <typename T>
class InputIterator {
public:
	using iterator_category = std::input_iterator_tag;

	InputIterator(const std::istream& is = std::cin): _input(is) {
	}

	T operator*() {
		T temp;
		std::cin >> temp;
		return temp;
	}

	InputIterator& operator++() {
		return *this;
	}

	InputIterator operator++(int) {
		return *this;
	}
private:
	const std::istream& _input;
};
