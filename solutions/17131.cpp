#include "../modules/SegmentTree.h"
#include "../modules/Vec2.h"
#include "../modules/Input.h"
#include "../modules/ModInt.h"
#include "../modules/FastIO.h"
#include <algorithm>

int main() {
	FastIO();
	int n;
	std::cin >> n;

	auto stars = Input::toVec<Int2>(n);

	std::sort(stars.begin(), stars.end(), [](Int2& l, Int2& r) {
			if (l.y != r.y) return l.y > r.y;

			return l.x < r.x;
			});

	SegmentTree<int> tree(400003);

	sm32_1e9_7 sum = 0;

	std::vector<int> row;

	int last_y = -200001;

	for (Int2 star: stars) {
		if (star.y != last_y) {
			for (int pos: row) {
				tree.update(pos + 200001, [](int& val) {val++;});
			}
			row.clear();
		}

		sum += sm32_1e9_7(tree.sum(0, star.x + 200001)) * tree.sum(star.x + 200002, 400003);

		row.push_back(star.x);

		last_y = star.y;
	}

	std::cout << sum;
}
