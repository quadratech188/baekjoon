#include "../modules/FastIO2.h"
#include "../modules/Vec2.h"
#include <algorithm>
#include <vector>

int main() {
	int n;
	Fast::cin >> n;

	std::vector<Int2> buildings(n);

	for (int i = 0; i < n; i++) {
		buildings[i].x = i;
		Fast::cin >> buildings[i].y;
	}

	int max = 0;

	for (int i = 0; i < n; i++) {
		int cnt = 0;
		for (int j = 0; j < i; j++) {
			bool flag = true;
			for (int k = j + 1; k < i; k++) {
				if ((buildings[j] - buildings[i]).cross(buildings[k] - buildings[i]) <= 0) {
					flag = false;
					break;
				}
			}
			if (flag) cnt++;
		}
		for (int j = i + 1; j < n; j++) {
			bool flag = true;
			for (int k = i + 1; k < j; k++) {
				if ((buildings[j] - buildings[i]).cross(buildings[k] - buildings[i]) >= 0) {
					flag = false;
					break;
				}
			}
			if (flag) cnt++;
		}

		max = std::max(max, cnt);
	}

	std::cout << max;
}
