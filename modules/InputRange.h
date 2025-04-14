#include <iostream>
#include <iterator>
#include <ranges>

template <typename T>
class InputRange: public std::ranges::view_interface<InputRange<T>> {
public:
	InputRange(size_t size, std::istream& is = std::cin):
		is(&is), _size(size) {}

	InputRange():
		_size(0), is(nullptr) {}

private:
	std::istream* is;
	size_t _size;

public:
	class iterator {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;

		iterator(std::istream* is, size_t index):
			is(is), index(index) {
			++(*this);
		}

		iterator():
			is(nullptr), index(0) {}

	private:
		std::istream* is;
		size_t index;

	public:
		T operator*() const {
			T value;
			(*is) >> value;
			return value;
		}

		iterator& operator++() {
			return *this;
		}

		iterator operator++(int) {
			iterator temp = *this;
			++(*this);
			return temp;
		}

		bool operator!=(iterator const& other) const {
			return index != other.index;
		}

		bool operator==(iterator const& other) const {
			return index == other.index;
		}
	};

	iterator begin() const {
		return iterator(is, 0);
	}

	iterator end() const {
		return iterator(is, _size);
	}

	size_t size() const noexcept {
		return _size;
	}
};
