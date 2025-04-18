#include "modules/Graph.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

enum FileType {
	file,
	folder
};

struct Data {
	std::string name;
	FileType type;
};

struct Node {
	std::unordered_map<std::string, Node*> children;
	Data value;
};

static std::unordered_map<std::string, Node*> lookup;

Node* parse(std::string path) {
	std::istringstream stream(path);
	std::string buffer;
	Node* current = lookup["main"];

	while (getline(stream, buffer, '/')) {
		current = current->children[buffer];
	}

	return current;
}

int main() {
	int n, m;
	std::cin >> n >> m;

	lookup["main"] = new Node {.value = {"main", folder}};

	for (int i = 0; i < n + m + 1; i++) {
		std::string parent, name;
		int isFolder;
		std::cin >> parent >> name >> isFolder;

		FileType type = isFolder? folder: file;
		
		Node* childNode = new Node {.value = {name, type}};

		lookup[parent]->children[name] = childNode;

		lookup[name] = childNode;
	}

	int k;
	std::cin >> k;

	for (int i = 0; i < k; i++) {
		std::string from, to;
		std::cin >> from >> to;
		Node* parent, child;
	}
}
