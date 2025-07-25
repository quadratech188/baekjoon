#include "../modules/FastIO2.h"
#include "../modules/Vec2.h"
#include <algorithm>
#include <cstdint>

int main() {
	uint n;
	Fast::cin >> n;

	auto points = Fast::cin.to_vec<Int2>(n);

	auto min = std::min_element(points.begin(), points.end());

	std::swap(points[0], *min);

	Int2 base = points[0];

	std::sort(points.begin() + 1, points.end(), [base](Int2 a, Int2 b) -> bool{
			int64_t temp = (a - base).cross(b - base);

			if (temp != 0) return temp > 0;

			return (a - base).size2() < (b - base).size2();
			});

	std::vector<Int2> stack;

	for (auto point: points) {
		while (stack.size() > 1 && (stack[stack.size() - 1] - stack[stack.size() - 2]).cross(
					point - stack[stack.size() - 1]
					) <= 0)
			stack.pop_back();

		stack.push_back(point);
	}

	std::cout << stack.size();
}
