#include <cstdio>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <utility>
#include <vector>

template <typename Vertex, typename Edge>
class ListGraph {
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<int, Edge>>> connections;
	int _size;

public:
	ListGraph(int size): data(size), connections(size), _size(size) {}
	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	void connect(int parent, int child, Edge edge = Edge()) {
		connections[parent].push_back(std::make_pair(child, edge));
	}

	template <typename Callable>
	void forEachChild(int parent, Callable&& func) {
		for (std::pair<int, Edge>& child: connections[parent]) {
			func(child.first, data[child.first], child.second);
		}
	}

	size_t size() {
		return _size;
	}

	Vertex& operator[](size_t index) {
		return data[index];
	}
};

template <typename T>
struct Matrix {

	Matrix(int columns, int rows, int defaultValue = T()):
		columnSize(columns), rowSize(rows), values(columns * rows, defaultValue) {}

	Matrix(): columnSize(0), rowSize(0), values(std::vector<T>()) {}

	T& at(int column, int row) {
		return this->values[column * rowSize + row];
	}

	struct Column {
		Matrix& matrix;
		int column;

		Column(Matrix& matrix, int column): matrix(matrix), column(column) {}

		T& operator[](int row) {
			return matrix.at(column, row);
		}
	};

	Column operator[](int column) {
		return Column(*this, column);
	}

	std::vector<T> values;

	int columnSize, rowSize;
};

template<typename T>
std::istream& operator>>(std::istream& input, Matrix<T>& matrix) {
	for (int j = 0; j < matrix.rowSize; j++) {
		for (int i = 0; i < matrix.columnSize; i++) {
			std::cin >> matrix[i][j];
		}
	}

	return input;
}

struct None {};

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

struct Data {
	int length;
	int cost;

	bool operator<(const Data& other) const {
		if (this->length > other.length) return true; // Reverse

		return this->cost > other.cost;
	}
};

int main() {
	FastIO();
	int k;
	std::cin >> k;

	int n, r;
	std::cin >> n >> r;

	ListGraph<None, Data> graph(n);

	for(int i = 0; i < r; i++) {
		int s, d, l, t;
		std::cin >> s >> d >> l >> t;

		graph.connect(s - 1, d - 1, {.length = l, .cost = t});
	}

	Matrix<int> length(n, k + 1, std::numeric_limits<int>::max());

	for (int i = 0; i <= k; i++)
		length[0][i] = 0;

	std::priority_queue<std::pair<int, Data>> queue;

	queue.push({0, {0, 0}});

	while (!queue.empty()) {
		std::pair<int, Data> temp = queue.top();
		queue.pop();

		int parent = temp.first;
		Data edge = temp.second;

		if (length[parent][edge.cost] < edge.length) continue;

		graph.forEachChild(parent, [parent, edge, k, &length, &queue](int child, None& vertex, Data& data) {
				if (edge.cost + data.cost > k) return;

				if (length[child][edge.cost + data.cost] <= edge.length + data.length) return;

				length[child][edge.cost + data.cost] = edge.length + data.length;
				queue.emplace(child, Data {edge.length + data.length, edge.cost + data.cost});
				});
	}

	int min = std::numeric_limits<int>::max();

	for (int cost = 0; cost <= k; cost++)
		min = std::min(min, length[n - 1][cost]);

	if (min == std::numeric_limits<int>::max())
		std::cout << -1;
	else
		std::cout << min;
}
