#include <algorithm>
#include <cstdio>
#include <iostream>
#include <istream>
#include <unordered_map>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

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

int main() {
	int n;
	std::cin >> n;

	std::vector<int> values = Input::toVec<int>(n);

	Compress<int> compress(values);

	std::vector<bool> used(compress.size());

	int count = 0;

	for (int value: values) {
		int index = compress(value);

		if (used[index]) continue;

		if ((index > 0 && used[index - 1]) || (index < used.size() - 1 && used[index + 1])) {
			used[index] = true;
			continue;
		}

		count++;
		used[index] = true;
	}

	std::cout << count;
}
