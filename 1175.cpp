#include <array>
#include <queue>

#include "modules/Matrix.h"
#include "modules/Vec2.h"

std::array<Vec2, 4> dirs = {
	Vec2(0, 1),
	Vec2(1, 0),
	Vec2(0, -1),
	Vec2(-1, 0)
};

struct Man {
	Vec2 pos;
	Vec2 prevDir;
	bool c1;
	bool c2;
	int count;
};

int main() {
	int m, n;
	std::cin >> n >> m;
	Matrix<char> grid(m, n);
	std::cin >> grid;

	Vec2 s, c1, c2;
	int temp = 0;

	for (int x = 0; x < grid.sizeX; x++) {
		for (int y = 0; y < grid.sizeY; y++) {
			if (grid.at(x, y) == 'S') {
				s = Vec2(x, y);
			}
			else if (grid.at(x, y) == 'C') {
				if (temp == 0) {
					c1 = Vec2(x, y);
					temp = 1;
				}
				else {
					c2 = Vec2(x, y);
				}
			}
		}
	}

	std::queue<Man> queue;

	queue.push({
			.pos = s,
			});

	int count = 0;

	std::array<Matrix<char>, 20> visited;

	for (auto& matrix: visited) {
		matrix = Matrix<char>(m, n);
	}

	while (!queue.empty()) {
		Man man = queue.front();
		queue.pop();
		// std::cout << man.pos.x << man.pos.y << man.prevDir.x << man.prevDir.y << man.c1 << man.c2 << man.count << std::endl;

		for (auto& dir: dirs) {
			if (dir == man.prevDir) continue;

			Vec2 newPos = man.pos + dir;

			if (newPos.x < 0 or newPos.x >= grid.sizeX) continue;
			if (newPos.y < 0 or newPos.y >= grid.sizeY) continue;

			if (grid[newPos] == '#') continue;

			Man result(man);

			result.pos = newPos;
			result.prevDir = dir;
			result.count ++;

			if (newPos == c1) result.c1 = true;
			if (newPos == c2) result.c2 = true;

			if (visited[10 * result.c1 + 5 * result.c2 + 2 + 2 * dir.x + dir.y][newPos]) continue;

			visited[10 * result.c1 + 5 * result.c2 + 2 + 2 * dir.x + dir.y][newPos] = true;

			if (result.c1 && result.c2){
				std::cout << result.count;
				return 0;
			}

			queue.push(result);
		}
	}
	std::cout << -1;
}
