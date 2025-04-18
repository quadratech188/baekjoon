#include "modules/GridGraph.h"
#include "modules/FastIO.h"
#include <iostream>
#include <queue>

void loop() {
	FastIO();
	int h, w;
	std::cin >> h >> w;

	Matrix<char> grid(w + 2, h + 2, '.');
	for (Int2 index: Range2(Int2::one(), grid.size() - Int2::one()))
		std::cin >> grid[index];
	// std::cout << grid;

	GridGraph<decltype(grid)> graph(grid);

	std::array<bool, 26> keys = {false};

	std::string keys_string;
	std::cin >> keys_string;
	if (keys_string != "0") {
		for (char ch: keys_string) keys[ch - 'a'] = true;
	}

	Matrix<char> visited(w + 2, h + 2);

	std::queue<Int2> doors;
	std::queue<Int2> queue;
	queue.push(Int2::zero());
	visited(0, 0) = true;

	bool progressed;
	int count = 0;

	do {
		progressed = false;

		for (int i = 0; i < doors.size(); i++) {
			Int2 door = doors.front();
			doors.pop();
			if (keys[graph[door] - 'A'])
				queue.push(door);
			
			else doors.push(door);
		}

		while (!queue.empty()) {
			Int2 parent = queue.front();
			queue.pop();
			
			for (auto child: graph.children(parent)) {
				if (visited[child]) continue;
				char type = child.value();
				if (type == '.') {}
				else if (type == '*')
					continue;
				else if (type == '$') 
					count++;
				else if ('A' <= type && type <= 'Z') {
					if (!keys[type - 'A']) {
						doors.push(child);
						visited[child] = true;
						continue;
					}
				}
				else 
					keys[type - 'a'] = true;

				progressed = true;
				visited[child] = true;
				queue.push(child);
			}
		}

	} while (progressed);

	std::cout << count << '\n';
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
