#include <iterator>
#include <unordered_set>
#include <vector>
#include <functional>

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
