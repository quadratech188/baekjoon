#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>

template <typename T, typename I = size_t>
class Compress {
public:
	// Builder
	template <typename Index>
	using with_index = Compress<T, Index>;
private:
	std::map<T, I> _compress;
	std::vector<T> _decompress;

public:
	I add(T const& val) {
		auto it = _compress.find(val);
		if (it != _compress.end()) {
			return it->second;
		}
		
		I result = _compress.size();
		_compress[val] = result;
		_decompress.push_back(val);
		return result;
	}

	I compress(T const& val) {
		return _compress[val];
	}

	T const& decompress(I index) {
		return _decompress[index];
	}
};
