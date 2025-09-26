#include <set>
#include <sys/types.h>
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

struct IdBins {
public:
	IdBins(uint size):
		group(size, -1), bin_counter(-1) {}

	void new_bin() {
		bin_counter ++;
	}
	void insert(uint val) {
		group[val] = bin_counter;
	}

	int operator[](uint i) const {
		return group[i];
	}

	uint size() {
		return bin_counter;
	}

	std::vector<int> group;
	int bin_counter;
};
