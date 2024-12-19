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
