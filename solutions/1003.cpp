#include "../modules/FastIO2.h"
#include "../modules/FastIO.h"
#include "../modules/Vec2.h"

Int2 cache[40];
int calculated_until = 1;

void loop() {
	int n;
	Fast::cin >> n;

	for (int k = calculated_until + 1; k <= n; k++) {
		cache[k] = cache[k - 1] + cache[k - 2];
	}

	std::cout << cache[n] << '\n';
}

int main() {
	FastIO();
	cache[0] = {1, 0};
	cache[1] = {0, 1};

	int t;
	Fast::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
