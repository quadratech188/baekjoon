#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include "../modules/Input.h"
#include "../modules/Vec2.h"
#include <iomanip>
#include <limits>

void loop() {
	uint n;
	Fast::cin >> n;
	std::vector<Int2> points(n);

	for (auto& point: points)
		Fast::cin >> point;

	double min_length = std::numeric_limits<double>::max();

	for (uint i = 0; i < (uint(1) << n); i++) {
		Int2 sum = Int2::zero();
		uint count_0 = 0;
		uint count_1 = 0;

		for (uint j = 0; j < n; j++) {
			if (i & (1 << j)) {
				sum += points[j];
				++count_1;
			}
			else {
				sum -= points[j];
				++count_0;
			}
		}

		if (count_0 != count_1) continue;

		min_length = std::min(min_length, sum.length());
	}

	std::cout << min_length << '\n';
}

int main() {
	FastIO();
	uint t;
	Fast::cin >> t;

	std::cout << std::fixed << std::setprecision(6);
	for (uint i = 0; i < t; i++) loop();
}
