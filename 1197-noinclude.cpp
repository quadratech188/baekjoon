#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

class UnionFind {
public:
	UnionFind(int size) {
		this->size = size;
		data.reserve(size);
		for (int i = 0; i < size; i++)
			data.push_back(i);
	}

	int find(int x) {
		if (data[x] == x)
			return x;
		data[x] = find(data[x]);
		return data[x];
	}

	void connect(int x, int y) {
		data[find(x)] = find(y);
	}

	bool connected(int x, int y) {
		return find(x) == find(y);
	}

private:
	std::vector<int> data;
	int size;
};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Edge {
	Edge(int a, int b, int c): a(a), b(b), c(c) {}

	int c;
	int a;
	int b;

	bool operator<(const Edge& other) const {
		if (c != other.c) return c < other.c;
		if (a != other.a) return a < other.a;
		return b < other.b;
	}
};

int main() {
	FastIO();
	int v, e;
	std::cin >> v >> e;

	UnionFind unionFind(v);

	std::vector<Edge> edges;
	edges.reserve(e);
	for (int i = 0; i < e; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;
		edges.emplace_back(a - 1, b - 1, c);
	}
	std::sort(edges.begin(), edges.end());

	int sum = 0;

	for(Edge& edge: edges) {
		if (unionFind.connected(edge.a, edge.b)) continue;

		sum += edge.c;
		unionFind.connect(edge.a, edge.b);
	}
	std::cout << sum;
}
