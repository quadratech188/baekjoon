#include "../modules/Vec2.h"
#include "../modules/FastIO.h"
#include "../modules/ModInt.h"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <stack>
#include <vector>

void loop() {
	size_t n;
	std::cin >> n;

	std::vector<Int2> points(n);
	for (auto& point: points)
		std::cin >> point;

	/*
	size_t min_index = 0;

	for (size_t i = 1; i < n; i++) {
		if (points[i] < points[min_index])
			min_index = i;
	}

	std::swap(points[0], points[min_index]);
	*/

	std::sort(points.begin(), points.end());

	Int2 min = points[0];

	std::sort(std::next(points.begin()), points.end(), [min](Int2 const& l, Int2 const& r) {
			return (l - min).cross(r - min) > 0;
			});

	std::vector<Int2> shell;

	for (auto& point: points) {
		while (shell.size() > 1 && (shell[shell.size() - 1] - shell[shell.size() - 2])
					.cross(point - shell[shell.size() - 1]) <= 0) {
			shell.pop_back();
				}
		shell.push_back(point);
	}
	dm32<>::set_mod(shell.size());

	dm32<> a = shell.size() - 1;
	dm32<> b = 0;
	dm32<> c = 0;
	dm32<> d = 1;

	int64_t length_square = 0;

	size_t count = 0;
	while (count != shell.size()) {
		int64_t new_length_square = (shell[c.val()] - shell[a.val()]).size2();
		if (length_square < new_length_square) {
			length_square = new_length_square;
		}
		if ((shell[b.val()] - shell[a.val()]).cross(shell[d.val()] - shell[c.val()]) > 0) {
			++c;
			++d;
		}
		else {
			++a;
			++b;
			++count;
		}
	}
	std::cout << std::fixed << std::setprecision(6) << std::sqrt(length_square);
}

int main() {
	FastIO();
	/*
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
	*/
	loop();
}
