#include <algorithm>
#include <limits>

#include "modules/SegmentTree.h"

struct Data {
	int min;
	int max;
	int a;
	int b;
	int c;

	Data operator+(const Data& other) const {
		return {
			.min = std::min(this->min, other.min),
			.max = std::max(this->max, other.max),
		};
	}
};
