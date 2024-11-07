#include "modules/Tensor.h"
#include "modules/FastIO.h"
#include <iostream>
#include <utility>
#include <vector>
#include <queue>

enum TomatoState {
	NONE,
	NOT_RIPE,
	RIPE
};

struct Data {
	int time;
	std::array<size_t, 11> index;
};

int main() {
	FastIO();

	std::array<size_t, 11> size;

	for (int i = 0; i < 11; i++) {
		std::cin >> size[i];
	}

	std::queue<Data> queue;

	int notRipeCount = 0;

	Tensor<11, TomatoState> tomatoes(size, [&notRipeCount, &queue](auto index, TomatoState& value) {
			int type;
			std::cin >> type;
			if (type == 1) {
				value = RIPE;
				queue.push({.time = 0, .index = index});
			}
			else if (type == 0) {
				value = NOT_RIPE;
				notRipeCount ++;
			}
			else {
				value = NONE;
			}
			}, true);

	int time;

	while (!queue.empty()) {
		Data value = queue.front();
		queue.pop();

		time = value.time;

		tomatoes.forEachNeighbor(value.index, [&notRipeCount, &value, &queue](auto index, TomatoState& state) {
				if (state == NOT_RIPE) {
					state = RIPE;
					queue.push({.time = value.time + 1, .index = index});
					notRipeCount --;
				}
				});
	}
		if (notRipeCount != 0) {
			std::cout << -1;
		}
		else {
			std::cout << time;
		}
}
