#include "modules/Vec2.h"
#include "modules/FastIO.h"
#include "modules/InputIterator.h"
#include "modules/LazySegmentTree.h"
#include "modules/Math.h"
#include <iomanip>
#include <ios>

struct Data {
	Data():
		angle_delta(0), offset(0, 0) {}
	Data(int length):
		angle_delta(Math::pi / 2), offset(length, 0) {}
	Data(double angle_delta, Double2 offset):
		angle_delta(angle_delta), offset(offset) {}
	double angle_delta;
	Double2 offset;

	Data operator+(const Data& other) const {
		return Data(0, value() + other.value());
	}

	void resolve(Data& left, Data& right) {
		left.angle_delta += angle_delta;
		right.angle_delta += angle_delta;

		offset = value();
		angle_delta = 0;
	}

	Double2 value() const {
		return offset.rotate(angle_delta);
	}
};

int main() {
	FastIO();
	std::cout << std::fixed << std::setprecision(2);

	int n, c;

	while (std::cin >> n >> c) {
		LazySegmentTree<Data> crane(n, InputIterator<int>());

		for (int i = 0; i < c; i++) {
			int s, a;
			std::cin >> s >> a;

			double radians = (a - 180) * Math::pi / 180;
			double delta = radians - crane.at(s).angle_delta;

			crane.update(s, n, [delta](Data& arm) {
					arm.angle_delta += delta;
					});
			Double2 result = crane.root().value();

			std::cout << result.x << ' ' << result.y << '\n';
		}
		std::cout << '\n';
	}
}
