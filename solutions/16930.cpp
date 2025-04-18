#include "modules/Matrix.h"
#include "modules/FastIO.h"
#include <limits>
#include <queue>
#include <utility>

int main() {
	FastIO();
	int n, m, k;
	std::cin >> n >> m >> k;

	Matrix<char> room(m + 2, n + 2, '#');
	Matrix<int> time(m + 2, n + 2, std::numeric_limits<int>::max());
	for (Int2 index: Range2(Int2(1, 1), Int2(m + 1, n + 1)))
		std::cin >> room[index];
	
	Int2 start, end;
	std::cin >> start >> end;
	std::swap(start.x, start.y);
	std::swap(end.x, end.y);

	std::queue<Int2> queue;
	queue.push(start);
	time[start] = 0;

	int answer = -1;

	while (!queue.empty()) {
		Int2 pos = queue.front();
		int iterations = time[pos];

		queue.pop();

		if (pos == end) {
			answer = iterations;
			break;
		}

		static std::array<Int2, 4> steps {
			Int2(0, 1),
			Int2(1, 0),
			Int2(0, -1),
			Int2(-1, 0)
		};

		for (Int2 step: steps) {
			for (int i = 1; i <= k; i++) {
				Int2 new_pos = pos + i * step;
				if (room[new_pos] == '#') break;
				if (time[new_pos] <= iterations) break;
				if (time[new_pos] == iterations + 1) continue;

				time[new_pos] = iterations + 1;
				queue.push(new_pos);
			}
		}
	}
	std::cout << answer << '\n';
}
