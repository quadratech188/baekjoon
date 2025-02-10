#include <algorithm>

template <typename T>
class Min {
public:
	T operator()(const T& l, const T& r) const {
		return std::min(l, r);
	}
};

template <typename T>
class Max {
public:
	T operator()(const T& l, const T& r) const {
		return std::max(l, r);
	}
};
