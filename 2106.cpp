#include <iostream>
#include <vector>
#include "modules/Bounds2.h"
#include "modules/Input.h"
#include "modules/Compress.h"
#include "modules/LazySegmentTree.h"

Compress<int> x, y;

struct Data {
	Data(int count, int length):
		count(count), length(length) {}

	int count;
	int length;

	Data operator+(const Data& other) {
		if (count > other.count)
			return Data(count, length);
		else if (count == other.count)
			return Data(count, length + other.length);
		else
		 	return Data(other.count, other.length);
	}
};

int main() {
	int n;
	std::cin >> n;

	std::vector<Range2> fences = Input::toVec<Range2>(n);

	std::vector<int> x_vec;
	x_vec.reserve(2 * n);

	std::vector<int> y_vec;
	y_vec.reserve(2 * n);

	for (Range2& r: fences) {
		x_vec.push_back(r.x1());
		x_vec.push_back(r.x2());
		y_vec.push_back(r.y1());
		y_vec.push_back(r.y2());
	}
	std::sort(x_vec.begin(), x_vec.end());
	std::sort(y_vec.begin(), y_vec.end());
}
