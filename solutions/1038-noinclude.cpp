#include <cstdint>
#include <iostream>
#include <queue>
int main() {
	int n;
	std::cin >> n;

	std::queue<int64_t> queue;
	for (int i = 0; i < 10; i++)
		queue.push(i);

	int cnt = 0;
	while (!queue.empty()) {
		int64_t fr = queue.front();
		queue.pop();

		if (cnt == n) {
			std::cout << fr;
			return 0;
		}
		cnt++;

		for (int i = 0; i < fr % 10; i++)
			queue.push(10 * fr + i);
	}
	std::cout << -1;
}
