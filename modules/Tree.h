#include "Graph.h"
#include <optional>

template <typename T>
struct Node {
	T value;
	Node* left;
	Node* right;
};
