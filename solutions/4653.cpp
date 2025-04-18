#include "modules/Tree.h"
#include <iostream>
#include <unordered_map>

enum Split {
	HORIZONTAL,
	VERTICAL
};

struct Window {
	char label;
	Split split;
};

void preorderInput(Node<int>& root) {
	char value;
	std::cin >> value;

	if (value == '|') {
		Node<int>& left;
		Node<int>& right;
		Node.left = left;
		Node.right = right;

		preorderInput(graph, left);
		preorderInput(graph, right);
	}
	else if (value == '-') {
		Graph<Window>::Node& left = graph.add({.split = HORIZONTAL});
		Graph<Window>::Node& right = graph.add({.split = HORIZONTAL});
		graph.link(root, left);
		graph.link(root, right);

		preorderInput(graph, left);
		preorderInput(graph, right);
	}
	else {
		Graph<Window>::Node& end = graph.add({.label = value});
		graph.link(root, end);
	}
}
std::pair<int, int> dimensions(Graph<Window>::Node& root) {
	static std::unordered_map<int, std::pair<int, int>> cache;
	if (cache.find(root.index) == cache.end()) {
		return cache[root.index];
	}

	if(root.value.label == '|') {

	}
	else if (root.value.label == '-') {

	}
	else {
		cache[root.index] = {1, 1};
	}

	return cache[root.index];
}

int main() {

}
