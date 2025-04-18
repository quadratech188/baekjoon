#include "modules/Bounds2.h"
#include "modules/Matrix.h"
#include "modules/Math.h"
#include "modules/FastIO.h"
#include <array>
#include <iostream>

static std::array<Int2, 4> directions = {
	Int2(0, -1),
	Int2(0, 1),
	Int2(1, 0),
	Int2(-1, 0)
};

struct Shark {
	Shark(Int2 velocity, int size):
		velocity(velocity), size(size) {}
	Shark():
		velocity(0, 0), size(0) {}
	Int2 velocity;
	int size;
};

std::pair<Int2, Shark> bounce(Int2 pos, int x, int y, Shark shark) {
	auto [quotient_x, remainder_x] = Math::quotient_remainder(pos.x + shark.velocity.x, x);
	auto [quotient_y, remainder_y] = Math::quotient_remainder(pos.y + shark.velocity.y, y);

	Int2 new_pos;
	Shark result;
	result.size = shark.size;

	if (quotient_x % 2 == 0) {
		new_pos.x = remainder_x;
		result.velocity.x = shark.velocity.x;
	}
	else {
		new_pos.x = x - remainder_x;
		result.velocity.x = - shark.velocity.x;
	}

	if (quotient_y % 2 == 0) {
		new_pos.y = remainder_y;
		result.velocity.y = shark.velocity.y;
	}
	else {
		new_pos.y = y - remainder_y;
		result.velocity.y = - shark.velocity.y;
	}

	return std::make_pair(new_pos, result);
}

int main() {
	FastIO();
	int r, c, m;
	std::cin >> r >> c >> m;
	Matrix<Shark> sharks1(c, r);
	Matrix<Shark> sharks2(c, r);

	for (int i = 0; i < m; i++) {
		int r, c, s, d, z;
		std::cin >> r >> c >> s >> d >> z;
		sharks1(c - 1, r - 1) = Shark(directions[d - 1] * s, z);
	}

	decltype(sharks1)& primary = sharks1;
	decltype(sharks2)& secondary = sharks2;

	Range2 bounds(Int2(c - 1, r - 1));

	int sum = 0;

	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			if (primary(i, j).size != 0) {
				sum += primary(i, j).size;
				primary(i, j).size = 0;
				break;
			}
		}

		for (Int2 index: primary.bounds()) {
			if (primary[index].size == 0) continue;

			auto [new_pos, result] = bounce(index, c - 1, r - 1, primary[index]);

			Shark& target = secondary[new_pos];
			if (result.size > target.size)
				target = result;

			primary[index].size = 0;
		}

		std::swap(primary, secondary);
	}
	std::cout << sum;
}
