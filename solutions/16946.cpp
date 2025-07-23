#include "../modules/GridGraph.h"
#include "../modules/FastIO2.h"
#include "../modules/ModInt.h"
#include <cstdint>
#include <queue>
#include <set>

using sm_ch = ModInt<uint8_t, uint8_t, StaticModPolicy<uint8_t, 10>>;

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	Matrix<char> walls(m, n);
	Fast::cin >> walls;

	GridGraph graph(walls);


	Matrix<uint> ids(m, n);
	std::vector<sm_ch> cnts(1);

	uint id_counter = 1;
	for (Int2 parent: walls.bounds()) {
		if (ids[parent] != 0) continue;
		if (walls[parent] == '1') continue;

		std::queue<Int2> queue;

		sm_ch cnt = 0;

		++cnt;
		ids[parent] = id_counter;
		queue.push(parent);

		while (!queue.empty()) {
			Int2 parent = queue.front();
			queue.pop();

			for (auto child: graph.children(parent)) {
				if (ids[child] != 0) continue;
				if (walls[child] == '1') continue;

				++cnt;
				ids[child] = id_counter;
				queue.push(child);
			}
		}

		cnts.push_back(cnt);
		id_counter ++;
	}

	for (Int2 index(0, 0); index.y < n; index.y ++) {
		for (index.x = 0; index.x < m; index.x ++) {
			if (walls[index] == '0') {
				std::cout << '0';
				continue;
			}
			sm_ch result = 1;
			std::set<uint> id_set;
			for (auto child: graph.children(index)) {
				if (id_set.find(ids[child]) != id_set.end()) continue;

				result += cnts[ids[child]];
				id_set.insert(ids[child]);
			}

			std::cout << int(result.val());
		}
		std::cout << '\n';
	}
}
