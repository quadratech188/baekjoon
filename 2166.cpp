#include "modules/Vec2.h"
#include "modules/FastIO.h"

int main() {
	FastIO();
	int n;
	std::cin >> n;

	Int2 first, second;
	std::cin >> first >> second;

	long long int sum = 0;

	for (int i = 2; i < n; i++) {
		Int2 third;
		std::cin >> third;

		sum += (second - first).cross(third - first);

		second = third;
	}

	printf("%.1f", std::abs(sum) / 2.0);
}
