#include <iostream>
#include <queue>
#include "modules/Tensor.h"
#include "modules/FastIO.h"

int main() {
	FastIO();
	size_t n, m;
	std::cin >> n >> m;
	int maxHeight = 0;

	Tensor<2, int> landscape({m + 2, n + 2}, [n, m, &maxHeight](auto index, int& value) {
			if (index[0] == 0 || index[0] == m + 1) {
				value = 0;
				return;
			}
			if (index[1] == 0 || index[1] == n + 1) {
				value = 0;
				return;
			}

			char temp;
			std::cin >> temp;
			value = temp - '0';
			maxHeight = std::max(value, maxHeight);
			});

	Tensor<2, int> waterHeights({m + 2, n + 2}, [maxHeight](auto index, int& value) {
			value = maxHeight;
			});

	Tensor<2, int> visited({m + 2, n + 2}, [](auto index, int& value) {
			value = false;
			});

	std::queue<std::array<size_t, 2>> queue;

	for (int waterHeight = maxHeight; waterHeight >= 0; waterHeight --) {
		visited.forEachRow([](auto index, int& value) {value = false;});
		queue.push({0, 0});

		while (!queue.empty()) {
			auto pos = queue.front();
			queue.pop();

			waterHeights[pos] = waterHeight;

			waterHeights.forEachNeighbor(pos, [&landscape, &waterHeight, &queue, &visited](auto index, int& height) {
					if (landscape[index] <= waterHeight && !visited[index]) {
						visited[index] = true;
						queue.push(index);
					}
					});
		}
	}

	int sum = 0;

	waterHeights.forEachRow([&sum, &landscape](auto index, int& value) {sum += value - landscape[index];});

	std::cout << sum;
}
