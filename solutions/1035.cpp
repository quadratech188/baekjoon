#include "modules/Matrix.h"
#include <algorithm>
#include <limits>
#include <queue>

Matrix<char> state(5, 5, '.');

static std::array<Int2, 4> deltas = {
	Int2(1, 0),
	Int2(0, 1),
	Int2(-1, 0),
	Int2(0, -1)
};

std::vector<Int2> points;

int min = std::numeric_limits<int>::max();

int dfs1(Int2 index, Matrix<char>& used) {
	used[index] = true;
	int sum = 1;
	for (Int2 delta: deltas) {
		if (!state.bounds().contains(index + delta)) continue;
		if (state[index + delta] != '*') continue;
		if (used[index + delta]) continue;

		sum += dfs1(index + delta, used);
	}
	return sum;
}

void dfs(int count, int length) {
	if (count == points.size()) {
		std::queue<int> queue;

		Matrix<char> used(5, 5);

		for (Int2 index: state.bounds()) {
			if (state[index] == '*') {
				if (dfs1(index, used) == points.size())
					min = std::min(min, length);
				return;
			}
		}
	}

	for (Int2 index: state.bounds()) {
		if (state[index] == '*') continue;

		state[index] = '*';

		dfs(count + 1, length + index.taxi_distance(points[count]));

		state[index] = '.';
	}
}

int main() {
	Matrix<char> grid(5, 5);
	std::cin >> grid;

	for (Int2 index: grid.bounds()) {
		if (grid[index] == '*')
			points.push_back(index);
	}
	dfs(0, 0);
	std::cout << min;
}
