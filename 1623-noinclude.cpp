#include <algorithm>
#include <array>
#include <cstdio>
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

	Node add(T value) {
		values.push_back(value);
		children.emplace_back();
		return *this[values.size() - 1];
	}

	void link(Node parent, Node child) {
		children[parent.index].insert(child.index);
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

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

std::vector<std::array<int, 2>> cache;

int solution(Graph<int>::Node root);

int solution(Graph<int>::Node root, bool used) {
	if (cache[root.index][used] != -1) {
		return cache[root.index][used];
	}
	int value;
	if (used) {
		value = root.value;
		root.forEachChild([&value](Graph<int>::Node child) {
				auto result = solution(child, false);
				if (result > 0) {
					value += result;
				}
				});
	}
	else {
		value = 0;
		root.forEachChild([&value](Graph<int>::Node child) {
				auto result = solution(child);
				if (result > 0) {
					value += result;
				}
				});
	}

	cache[root.index][used] = value;

	return value;
}

int solution(Graph<int>::Node root) {
	int used = solution(root, true);
	int not_used = solution(root, false);

	if (used > not_used) {
		return used;	
	}
	else {
		return not_used;
	}
}

void output(Graph<int>::Node root, bool used, std::vector<int>& out) {
	if (used) {
		out.push_back(root.index);
	}
	root.forEachChild([&used, &out](Graph<int>::Node child) {
			if (used) {
				output(child, false, out);
			}
			else {
				if (solution(child, true) > solution(child, false)) {
					output(child, true, out);
				}
				else {
					output(child, false, out);
				}
			}
			});
}

int main() {
	FastIO();
	std::vector<int> values;

	int n;
	std::cin >> n;

	cache.resize(n);

	for (int i = 0; i < n; i++) {
		cache[i] = {-1, -1};
	}

	values.resize(n);

	for (int i = 0; i < n; i++) {
		std::cin >> values[i];
	}

	Graph<int> tree(values);

	for (int i = 0; i < n - 1; i++) {
		int parent;
		std::cin >> parent;
		tree.link(parent - 1, i + 1);
	}
	tree.toTree(tree[0]);
	
	std::cout << solution(tree[0], true) << " " << solution(tree[0], false) << std::endl;

	std::vector<int> used;
	output(tree[0], true, used);
	std::sort(used.begin(), used.end());

	for (int i: used) {
		std::cout << i + 1 << " ";
	}
	std::cout << -1 << std::endl;

	std::vector<int> not_used;
	output(tree[0], false, not_used);
	std::sort(not_used.begin(), not_used.end());

	for (int i: not_used) {
		std::cout << i + 1 << " ";
	}
	std::cout << -1 << std::endl;
}
