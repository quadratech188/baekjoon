#include <algorithm>

#include "../modules/Vec2.h"
#include "../modules/FastIO2.h"
#include "../modules/UnionFind.h"

bool check_frac(int64_t numerator, int64_t denominator) {
	if (denominator == 0) return true;

	if (denominator < 0)
		return denominator <= numerator && numerator <= 0;
	else
	 	return 0 <= numerator && numerator <= denominator;
}

bool intersects(Int2 a1, Int2 a2, Int2 b1, Int2 b2) {

	bool result = true;

	// a1 + (a2 - a1) x = b1 + (b2 - b1) y
	
	// (a2 - a1) cross (b2 - b1) x = (b1 - a1) cross (b2 - b1)
	
	result &= check_frac((b1 - a1).cross(b2 - b1), (a2 - a1).cross(b2 - b1));

	// (a1 - b1) cross (a2 - a1) = (b2 - b1) cross (a2 - a1) y
	result &= check_frac((a1 - b1).cross(a2 - a1), (b2 - b1).cross(a2 - a1));

	if ((a2 - a1).cross(b2 - b1) == 0) {
		result &= (b1 - a1).cross(a2 - a1) == 0;

		if (a1.x > a2.x) std::swap(a1, a2);
		if (b1.x > b2.x) std::swap(b1, b2);
		result &= a1.x <= b2.x && b1.x <= a2.x;

		if (a1.y > a2.y) std::swap(a1, a2);
		if (b1.y > b2.y) std::swap(b1, b2);
		result &= a1.y <= b2.y && b1.y <= a2.y;
	}

	return result;
}

int main() {
	uint n;
	Fast::cin >> n;

	auto lines = Fast::cin.to_vec<std::pair<Int2, Int2>>(n);
	UnionFind uf(n);

	for (uint i = 0; i < n; i++) {
		for (uint j = i + 1; j < n; j++) {
			if (intersects(lines[i].first, lines[i].second, lines[j].first, lines[j].second))
				uf.connect(i, j);
		}
	}
	std::vector<uint> counts(n);
	for (uint i = 0; i < n; i++)
		counts[uf.find(i)] ++;

	std::cout << std::ranges::count_if(counts, [](uint val) {return val != 0;})
		<< '\n' << std::ranges::max(counts);
}
