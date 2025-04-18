#include <cstdio>
#include <iostream>
#include <istream>
#include <limits>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

template <typename Vertex, typename Edge>
class ListGraph {
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<int, Edge>>> connections;
	int _size;

public:
	ListGraph(int size, Vertex defaultV): data(size, defaultV), connections(size), _size(size) {}
	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	int add(Vertex data) {
		data.push_back(data);
		connections.emplace_back();
		_size += 1;
		return _size - 1;
	}

	void reserve(int size) {
		data.reserve(size);
		connections.reserve(size);
	}

	void connect(int parent, int child, Edge edge = Edge()) {
		connections[parent].push_back(std::make_pair(child, edge));
	}

	size_t size() {
		return _size;
	}

	class Children {
	public:
		Children(ListGraph& graph, int parent): graph(graph), parent(parent) {}

		class iterator {
		public:
			iterator(ListGraph& graph, const int parent, int index = 0): graph(graph), parent(parent), _index(index) {}

			iterator& operator*() {
				return *this;
			}

			int index() const {
				return graph.connections[parent][_index].first;
			}

			Edge& edge() {
				return graph.connections[parent][_index].second;
			}

			operator int() {
				return index();
			}

			iterator& operator++() {
				_index++;
				return *this;
			}

			bool operator!=(const iterator& other) const {
				return _index != other._index;
			}

		private:
			ListGraph& graph;
			const int parent;
			int _index;
		};

		iterator begin() {
			return iterator(graph, parent);
		}

		iterator end() {
			return iterator(graph, parent, graph.connections[parent].size());
		}

	private:
		ListGraph& graph;
		int parent;
	};

	Children children(int parent) {
		return Children(*this, parent);
	}

	Vertex& operator[](size_t index) {
		return data[index];
	}
};

template<typename T1, typename T2>
std::istream& operator>>(std::istream& is, std::pair<T1, T2>& data) {
	is >> data.first >> data.second;
	return is;
}

namespace Input {
	template<typename T>
	inline std::vector<T> inlineToVec(size_t n) {
		std::vector<T> result;
		result.reserve(n);
		for (int i = 0; i < n; i++) {
			T value;
			std::cin >> value;
			result.push_back(value);
		}
		return result;
	}
}

struct None {};

template<typename T, typename T2 = T>
struct Vec2 {
	T x, y;

	Vec2(T x, T y): x(x), y(y) {}
	Vec2(): x(T()), y(T()) {}

	Vec2 operator+(const Vec2& other) const {
		return Vec2(this->x + other.x, this->y + other.y);
	}

	Vec2 operator-(const Vec2& other) const {
		return Vec2(this->x - other.x, this->y - other.y);
	}

	template<typename D>
	Vec2<D> operator/(const D other) const {
		return Vec2<D>(this->x / other, this->y / other);
	}

	Vec2<T2> operator*(const T other) const {
		return Vec2<T2>((T2)this->x * other, (T2)this->y * other);
	}

	bool operator<(const Vec2& other) const {
		if (this->x != other.x) return this->x < other.x;
		return this->y < other.y;
	}

	bool operator==(const Vec2& other) const {
		return this->x == other.x && this->y == other.y;
	}

	bool operator!=(const Vec2& other) const {
		return this->x != other.x || this->y != other.y;
	}

	T2 dot(const Vec2& other) const {
		return static_cast<T2>(this->x) * other.x + static_cast<T2>(this->y) * other.y;
	}

	T2 cross(const Vec2& other) const {
		return static_cast<T2>(this->x) * other.y - static_cast<T2>(this->y) * other.x;
	}

	T2 size2() const {
		return static_cast<T2>(this->x) * this->x + static_cast<T2>(this->y) * this->y;
	}

	static Vec2 zero() {
		return Vec2(0, 0);
	}

	static Vec2 one() {
		return Vec2(1, 1);
	}
};

template <typename T, typename T2>
std::istream& operator>>(std::istream& is, Vec2<T, T2>& vec2) {
	is >> vec2.x >> vec2.y;
	return is;
}

typedef Vec2<int, long long int> Int2;

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

void updateDistances(ListGraph<int, None>& graph, int root) {
	std::queue<int> queue;
	queue.push(root);

	while (!queue.empty()) {
		int parent = queue.front();
		queue.pop();

		for (int child: graph.children(parent)) {
			if (graph[child] > graph[parent] + 1) {
				graph[child] = graph[parent] + 1;
				queue.push(child);
			}
		}
	}
}

int main() {
	FastIO();
	int n, m, q;
	std::cin >> n >> m >> q;

	ListGraph<int, None> graph(n, std::numeric_limits<int>::max() - 10);
	graph[0] = 0;

	std::vector<Int2> edges = Input::inlineToVec<Int2>(m);

	std::vector<std::pair<char, int>> queries = Input::inlineToVec<std::pair<char, int>>(q);

	std::vector<bool> deleted(m);

	for (auto query: queries) {
		if (query.first == 'U')
			deleted[query.second - 1] = true;
	}

	for (int i = 0; i < m; i++) {
		if (!deleted[i])
			graph.connect(edges[i].x - 1, edges[i].y - 1);
	}

	updateDistances(graph, 0);

	std::stack<int> results;

	for (int i = q - 1; i >= 0; i--) {
		char type = queries[i].first;
		int data = queries[i].second;

		if (type == 'U') {
			graph.connect(edges[data - 1].x - 1, edges[data - 1].y - 1);
			updateDistances(graph, edges[data - 1].x - 1);
		}
		else {
			if (graph[data - 1] == std::numeric_limits<int>::max() - 10)
				results.push(-1);
			else
				results.push(graph[data - 1]);
		}
	}

	while (!results.empty()) {
		std::cout << results.top() << '\n';
		results.pop();
	}
}
