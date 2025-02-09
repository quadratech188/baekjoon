#include <vector>
#include <algorithm>
#include <unordered_map>

template <typename T>
class Compress {
public:
	Compress() {}

	template <std::ranges::range R>
	Compress(R&& values) {
		size_t index = 0;

		for (T& value: values) {
			if (_inverse.find(value) != _inverse.end()) continue;
			_inverse[value] = index;
			index++;
		}

		_size = index;

		_values.resize(_size);

		for (auto& it: _inverse) {
			_values[it.second] = it.first;
		}
	}

	size_t compress(const T& val) {
		return _inverse.at(val);
	}

	T decompress(const size_t index) {
		return _values[index];
	}

	size_t size() const {
		return _size;
	}

private:
	size_t _size;
	std::vector<T> _values;
	std::unordered_map<T, size_t> _inverse;
};
