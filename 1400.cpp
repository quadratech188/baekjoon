#include "modules/Matrix.h"
#include "modules/FastIO.h"
#include "modules/Input.h"
#include "modules/Vec2.h"
#include <algorithm>
#include <queue>
#include <array>

enum Direction {
	EW,
	NS
};

struct Light {
	int ew, ns;
	Direction initial;

	Direction state(int time) {
		time = (time - 1) % (ew + ns);

		if (initial == EW) {
			if (time < ew)
				return EW;
			else
			 	return NS;
		}
		else {
			if (time < ns)
				return NS;
			else
			 	return EW;
		}
	}
};

std::istream& operator>>(std::istream& is, Light& light) {
	char a;
	int i;

	std::cin >> i >> a >> light.ew >> light.ns;

	if (a == '-')
		light.initial = EW;
	else
	 	light.initial = NS;

	return is;
}

int main() {
	// FastIO();
	
	std::array<Int2, 4> directions {
		Int2(0, 1),
		Int2(1, 0),
		Int2(0, -1),
		Int2(-1, 0)
	};

	while (true) {
		int m, n;
		std::cin >> m >> n;

		if (m == 0 && n == 0) break;

		Matrix<char> grid(n, m);
		std::cin >> grid;

		Matrix<char> visited(n, m, false);

		std::queue<std::pair<Int2, int>> queue;

		int lightCount = 0;

		for (Int2 index: grid.bounds()) {
			switch(grid[index]) {
				case '#':
				case '.':
				case 'B':
					break;
				case 'A':
					visited[index] = true;
					queue.emplace(index, 0);
					break;
				default:
					lightCount = std::max(lightCount, grid[index] - '0' + 1);
			}
		}

		std::vector<Light> lights = Input::inlineToVec<Light>(lightCount);

		int result = 0;

		while (!queue.empty() && result == 0) {
			Int2 pos = queue.front().first;
			int time = queue.front().second;
			queue.pop();
			bool enQueued = false;

			for (Int2 direction: directions) {
				Int2 newPos = pos + direction;

				if (!grid.bounds().contains(newPos)) continue;
				if (visited[newPos]) continue;

				if (grid[newPos] == '.') continue;

				if (grid[newPos] == 'B') {
					result = time + 1;
					break;
				}

				if (grid[newPos] == '#') {
					visited[newPos] = true;
					queue.emplace(newPos, time + 1);
					continue;
				}

				Light& light = lights[grid[newPos] - '0'];

				Direction state = light.state(time + 1);

				if ((state == EW && direction.y == 0) || (state == NS && direction.x == 0)) {
					visited[newPos] = true;
					queue.emplace(newPos, time + 1);
				}
				else {
					if (!enQueued) {
						queue.emplace(pos, time + 1);
						enQueued = true;
					}
				}
			}
		}
		if (result == 0)
			std::cout << "impossible" << '\n';
		else
		 	std::cout << result << '\n';;
	}

	return 0;
}
