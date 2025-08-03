#include "../modules/ListGraph2.h"
#include "../modules/Graph.h"
#include "../modules/FastIO2.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <set>
#include <stack>
#include <variant>

uint ssc_counter = 0;

ListGraph<uint, std::monostate> graph;

std::vector<uint> indices, lowlinks;
std::vector<bool> on_stack;

uint index;
std::stack<uint> S;

void strongconnect(uint v) {
	indices[v] = index;
	lowlinks[v] = index;
	index ++;
	S.push(v);
	on_stack[v] = true;

	for (auto& w: graph.children(v)) {
		if (indices[w] == std::numeric_limits<uint>::max()) {
			strongconnect(w);
			lowlinks[v] = std::min(lowlinks[v], lowlinks[w]);
		}
		else if (on_stack[w]) {
			lowlinks[v] = std::min(lowlinks[v], indices[w]);
		}
	}

	if (indices[v] == lowlinks[v]) {
		uint w;
		do {
			w = S.top();
			S.pop();
			on_stack[w] = false;
			graph[w] = ssc_counter;
		} while (w != v);
		ssc_counter ++;
	}
}

int main() {
	uint v, e;
	Fast::cin >> v >> e;

	graph.resize(v);

	for (uint i = 0; i < e; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	indices.resize(v, std::numeric_limits<uint>::max());
	lowlinks.resize(v, std::numeric_limits<uint>::max());
	on_stack.resize(v, false);

	for (uint i = 0; i < v; i++) {
		if (indices[i] == std::numeric_limits<uint>::max())
			strongconnect(i);
	}

	std::vector<std::set<uint>> sscs(ssc_counter);

	for (uint i = 0; i < v; i++)
		sscs[graph[i]].insert(i);

	std::ranges::sort(sscs, [&sscs](std::set<uint>& a, std::set<uint>& b) {
			return *a.begin() < *b.begin();
			});

	std::cout << sscs.size() << '\n';
	for (auto& ssc: sscs) {
		for (uint val: ssc)
			std::cout << val + 1 << ' ';
		std::cout << "-1\n";
	}
}
