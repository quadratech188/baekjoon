#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stack>
#include <vector>
#include "modules/Vec2.h"
#include "modules/Input.h"

int count = 1;

bool loop() {
	int n;
	std::cin >> n;

	if (n == 0) return false;

	std::vector<Int2> points = Input::toVec<Int2>(n);

	auto min = std::min_element(points.begin(), points.end());
	Int2 min_p = *min;

	std::swap(points[0], *min);
	std::sort(points.begin() + 1, points.end(), [&min_p](Int2& lhs, Int2& rhs) {
			int cross = (lhs - min_p).cross(rhs - min_p);
			if (cross != 0)
				return cross > 0;

			if (lhs.x != rhs.x)
				return lhs.x < rhs.x;

			return lhs.y < rhs.y;
			});

	std::vector<Int2> shell;

	for (int i = 0; i < points.size(); i++) {
		while (shell.size() >= 2 &&
				(shell[shell.size() - 1] - shell[shell.size() - 2])
				.cross(points[i] - shell[shell.size() - 1])
				<= 0) {
			shell.pop_back();
		}
		shell.push_back(points[i]);
	}

	double diameter = std::numeric_limits<double>::max();

	for (int i = 0; i < shell.size(); i++) {
		Int2 direction = shell[(i + 1) % shell.size()] - shell[i];

		double max = 0;

		for (auto& point: shell) {
			max = std::max(max,
					direction.cross(point - shell[i]) / direction.length());
		}
		diameter = std::min(diameter, max);
	}

	std::cout << "Case " << count << ": " << std::ceil(diameter * 100) / 100 << '\n';
	count++;

	return true;
}

int main() {
	std::cout << std::fixed;
	std::cout.precision(2);

	while (loop()) {}
}
