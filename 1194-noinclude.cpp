#include <array>
#include <iostream>
#include <queue>
#include <vector>

struct Vec2 {
	int x, y;

	Vec2(int x, int y): x(x), y(y) {}
	Vec2(): x(0), y(0) {}

	Vec2 operator+(const Vec2& other) {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	bool operator==(const Vec2& other) {
		return this->x == other.x && this->y == other.y;
	}
};

template <typename T>
struct Matrix {
	std::vector<T> values;

	int sizeX, sizeY;

	Matrix(int x, int y, int defaultValue = T()) {
		values = std::vector<T>(x * y);
		sizeX = x;
		sizeY = y;
	}

	T& at(int x, int y) {
		return this->values[x * sizeY + y];
	}
	T& operator[](Vec2 index) {
		return this->values[index.x * sizeY + index.y];
	}

	Matrix(): sizeX(0), sizeY(0), values(std::vector<T>()) {}
};

template<typename T>
std::istream& operator>>(std::istream& input, Matrix<T>& matrix) {
	for (int j = 0; j < matrix.sizeY; j++) {
		for (int i = 0; i < matrix.sizeX; i++) {
			std::cin >> matrix.at(i, j);
		}
	}

	return input;
}

std::array<Vec2, 4> dirs = {
	Vec2(0, 1),
	Vec2(1, 0),
	Vec2(0, -1),
	Vec2(-1, 0)
};

struct Man {
	Vec2 pos;
	bool a, b, c, d, e, f;
	int count;
};

int main() {
	int m, n;
	std::cin >> n >> m;
	Matrix<char> grid(m, n);
	std::cin >> grid;

	Vec2 s, c;

	for (int x = 0; x < grid.sizeX; x++) {
		for (int y = 0; y < grid.sizeY; y++) {
			if (grid.at(x, y) == '0') {
				s = Vec2(x, y);
			}
			else if (grid.at(x, y) == '1') {
				c = Vec2(x, y);
			}
		}
	}

	std::queue<Man> queue;

	queue.push({
			.pos = s,
			});

	int count = 0;

	std::array<Matrix<char>, 64> visited;

	for (auto& matrix: visited) {
		matrix = Matrix<char>(m, n);
	}

	while (!queue.empty()) {
		Man man = queue.front();
		queue.pop();
		// std::cout << man.pos.x << man.pos.y << man.a << man.b << man.c << man.d << man.e << man.f << std::endl;

		for (auto& dir: dirs) {
			Vec2 newPos = man.pos + dir;

			if (newPos.x < 0 or newPos.x >= grid.sizeX) continue;
			if (newPos.y < 0 or newPos.y >= grid.sizeY) continue;

			if (grid[newPos] == '#') continue;

			Man result(man);

			result.pos = newPos;
			result.count ++;

			switch(grid[newPos]) {
				case 'a':
					result.a = true; break;
				case 'b':
					result.b = true; break;
				case 'c':
					result.c = true; break;
				case 'd':
					result.d = true; break;
				case 'e':
					result.e = true; break;
				case 'f':
					result.f = true; break;
				case 'A':
					if (!result.a) continue; break;
				case 'B':
					if (!result.b) continue; break;
				case 'C':
					if (!result.c) continue; break;
				case 'D':
					if (!result.d) continue; break;
				case 'E':
					if (!result.e) continue; break;
				case 'F':
					if (!result.f) continue; break;
			}

			if (visited[result.a + 2 * result.b + 4 * result.c + 8 * result.d + 16 * result.e + 32 * result.f][newPos]) continue;

			visited[result.a + 2 * result.b + 4 * result.c + 8 * result.d + 16 * result.e + 32 * result.f][newPos] = true;

			if (grid[newPos] == '1'){
				std::cout << result.count;
				return 0;
			}

			queue.push(result);
		}
	}
	std::cout << -1;
}
