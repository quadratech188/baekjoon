#include "../../modules/Vec2.h"
#include "../../modules/ModInt.h"
#include "../../modules/Input.h"
#include "../../modules/Matrix.h"
#include <array>
#include <iostream>
#include <optional>
#include <variant>

int k;
std::vector<Int2> points;
Matrix<int> usage;
Matrix<int> state;

sm32_1e9_7 solution(int index) {
	if (index == k + 1) return 1;

	if (points[index].x == points[index - 1].x || points[index].y == points[index - 1].y)
		return solution(index + 1);

	std::array<Int2, 2> candidates {
		Int2(points[index].x, points[index - 1].y),
		Int2(points[index - 1].x, points[index].y)
	};


	if (usage[candidates[0]] == 1 && usage[candidates[1]] == 1) {
		return 2 * solution(index + 1);
	}

	sm32_1e9_7 result = 0;
	for (Int2 candidate: candidates) {
		state[candidate]++;
		result += solution(index + 1);
		state[candidate]--;
	}

	return result;
}

void loop() {
	int n, m;
	std::cin >> n >> m >> k;

	points = Input::toVec<Int2>(k + 1);

	for (Int2& point: points)
		point -= Int2(1, 1);

	usage = Matrix<int>(n, m);

	for (int i = 1; i < k + 1; i++) {
		if (points[i].x == points[i - 1].x || points[i].y == points[i - 1].y) {
			usage[(points[i] + points[i - 1]) / 2] ++;
			continue;
		}

		usage(points[i - 1].x, points[i].y) ++;
		usage(points[i].x, points[i - 1].y) ++;
	}

	state = Matrix<int>(n, m);

	std::cout << solution(1) << '\n';
}

 int main() {
	 int t;
	 std::cin >> t;
	 for (int i = 0; i < t; i++) loop();
 }
