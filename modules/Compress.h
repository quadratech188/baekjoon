#include <algorithm>
#include <unordered_map>

template <typename T>
class Compress {
public:
	Compress(std::vector<T>& values) {
		std::vector<T> copy(values);
		std::sort(copy.begin(), copy.end());

		size_t index = 0;

		for (T& value: copy) {
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

	size_t operator()(const T& val) {
		return _inverse[val];
	}

	size_t size() const {
		return _size;
	}

private:
	size_t _size;
	std::vector<T> _values;
	std::unordered_map<T, size_t> _inverse;
};
