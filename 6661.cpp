#include "modules/Vec2.h"
#include "modules/SegmentTree.h"
#include "modules/InputIterator.h"
#include "modules/Math.h"
#include "modules/FastIO.h"
#include <cmath>

struct Crane {
	Crane():
		rotation(0), total_rotation(0), offset(0) {}
	Crane(int length):
		rotation(0), total_rotation(0), offset(length) {}
	Crane(double rotation, double total_rotation, double offset):
		rotation(rotation), total_rotation(total_rotation), offset(offset) {}
	double rotation;
	double total_rotation;
	double offset;
	Crane operator+(const Crane& other) const {
		Double2 total = Double2(offset, 0).rotate(rotation)
			+ Double2(other.offset, 0).rotate(total_rotation + other.rotation);
		return Crane(total.theta(),
				total_rotation + other.total_rotation,
				total.length());
	}
};

int main() {
	FastIO();
	bool first = true;
	while (true) {
		int n, c;
		std::cin >> n >> c;
		if (std::cin.eof()) break;

		if (!first) printf("\n");
		first = false;

		SegmentTree<Crane> crane(n, InputIterator<int>());

		for (int i = 0; i < c; i++) {
			int s, a;
			std::cin >> s >> a;

			crane.update(s, [a](Crane& arm) {
					arm.rotation = (a - 180) * Math::pi / 180;
					arm.total_rotation = (a - 180) * Math::pi / 180;
					});

			Double2 result = Double2(0, crane.root().offset).rotate(crane.root().rotation);

			printf("%.2f %.2f\n", result.x, result.y);
		}
	}
}
