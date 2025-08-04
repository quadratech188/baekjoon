#include <set>
#include <vector>

template <typename T>
struct SetBins {
public:
	void new_bin() {
		bins.emplace_back();
	}
	void insert(T val) {
		bins.back().insert(val);
	}
	std::vector<std::set<T>> bins;
};
