#include <array>
#include <forward_list>
#include <functional>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

template <typename T>
class Graph {
	std::vector<T> values;
	std::vector<std::unordered_set<size_t>> children;

public:
	struct Node {
	private:
		Graph& graph;
	public:
		size_t index;
		T& value;

	private:

		Node(Graph& graph, int index, T& value): graph(graph), index(index), value(value) {}
	
	public:

		void forEachChild(std::function<void(Node)> func) {
			for (size_t childIndex: graph.children[index]) {
				func(Node(graph, childIndex, graph.values[childIndex]));
			}
		}

		friend class Graph;
	};

private:

	void forEachChild(size_t index, std::function<void(size_t)> func) {
		for (size_t childIndex: children[index]) {
			func(childIndex);
		}
	}

public:
	Graph();

	Graph(std::vector<T>& values) {
		this->values = values;
		children.resize(values.size());
	}

	Graph(size_t size) {
		values.resize(size);
		children.resize(size);
	}

	Node operator[](size_t index) {
		return Node(*this, index, values[index]);
	}

	T& value(size_t index) {
		return values[index];
	}

	void add(T& value) {
		values.push_back(value);
		children.emplace_back();
	}

	void link(size_t parent, size_t child) {
		children[parent].insert(child);
	}

	void toTree(size_t root) {
		forEachChild(root, [this, root](size_t child) {
				children[child].erase(root);
				toTree(child);
				});
	}

	inline void toTree(Node root) {
		toTree(root.index);
	}
};

std::vector<std::array<std::pair<std::forward_list<int>, int>, 2>> cache;

std::pair<std::forward_list<int>, int> solution(Graph<int>::Node root);

std::pair<std::forward_list<int>, int> solution(Graph<int>::Node root, bool used) {
	if (cache[root.index][used].second != -1) {
		return cache[root.index][used];
	}
	std::forward_list<int> out;
	int value;
	if (used) {
		out.push_front(root.index);
		value = root.value;
		root.forEachChild([&out, &value](Graph<int>::Node child) {
				auto result = solution(child, false);
				value += result.second;
				out.merge(result.first);
				});
	}
	else {
		value = 0;
		root.forEachChild([&out, &value](Graph<int>::Node child) {
				auto result = solution(child);
				value += result.second;
				out.merge(result.first);
				});
	}

	cache[root.index][used] = {out, value};

	return {out, value};
}

std::pair<std::forward_list<int>, int> solution(Graph<int>::Node root) {
	auto used = solution(root, true);
	auto not_used = solution(root, false);

	if (used.second > not_used.second) {
		return used;	
	}
	else {
		return not_used;
	}
}

int main() {
	std::vector<int> values;

	int n;
	std::cin >> n;

	cache.resize(n);

	for (int i = 0; i < n; i++) {
		cache[i][0] = {std::forward_list<int>(), -1};
		cache[i][1] = {std::forward_list<int>(), -1};
	}

	values.resize(n);

	for (int i = 0; i < n; i++) {
		std::cin >> values[i];
	}

	Graph<int> tree(values);

	for (int i = 0; i < n - 1; i++) {
		int a, b;
		std::cin >> a >> b;
		tree.link(a - 1, b - 1);
		tree.link(b - 1, a - 1);
	}
	tree.toTree(tree[0]);

	auto result = solution(tree[0]);

	result.first.sort();

	std::cout << result.second << std::endl;

	for (auto value: result.first) {
		std::cout << value + 1 << " ";
	}
}
