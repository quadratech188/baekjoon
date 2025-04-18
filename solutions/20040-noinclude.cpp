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

int main() {
	FastIO();
	int n, m;
	std::cin >> n >> m;

	UnionFind uf(n);

	int result = 0;

	for (int i = 1; i <= m; i++) {
		int a, b;
		std::cin >> a >> b;
		if (result != 0) continue;
		
		if (uf.connected(a, b)) {
			result = i;
			continue;
		}

		uf.connect(a, b);
	}

	std::cout << result;
}
