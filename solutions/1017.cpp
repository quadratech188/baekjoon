#include "../modules/PrimeLookup.h"
#include "../modules/InputRange.h"
#include "../modules/ListGraph.h"
#include "../modules/Output.h"
#include <algorithm>
#include <iostream>
#include <variant>
#include <vector>

constexpr PrimeLookup<2000> lookup;

std::vector<int> parents;
std::vector<int> children;
std::vector<int> considered;
ListGraph<std::monostate, std::monostate> graph;
int iterations;

bool match(int parent) {
	if (parent == 0) return false;
	for (int child: graph.children(parent)) {
		if (considered[child] == iterations) continue;
		considered[child] = iterations;

		if (parents[child] == -1 || match(parents[child])) {
			parents[child] = parent;
			children[parent] = child;
			return true;
		}
	}
	return false;
}

int main() {
	int n;
	std::cin >> n;

	std::vector<int> a, b;

	int first;
	std::cin >> first;

	a.push_back(first);

	for (int value: InputRange<int>(n - 1)) {
		if ((value - first) % 2 == 0)
			a.push_back(value);
		else
		 	b.push_back(value);
	}

	if (a.size() != b.size()) {
		std::cout << "-1";
		return 0;
	}

	parents.resize(b.size(), -1);
	children.resize(a.size());
	considered.resize(b.size());
	graph.resize(a.size());

	for (size_t parent = 0; parent < a.size(); parent++) {
		for (size_t child = 0; child < b.size(); child++) {
			if (lookup(a[parent] + b[child])) {
				graph.connect(parent, child);
			}
		}
	}

	std::vector<int> results;

	for (size_t child: graph.children(0)) {

		parents.assign(b.size(), -1);
		children.assign(a.size(), -1);
		considered.assign(b.size(), -1);
		size_t successes = 1;
		iterations = 0;

		children[0] = child;
		parents[child] = 0;
		considered[child] = iterations;

		for (size_t i = 1; i < a.size(); i++) {
			if (match(i))
				successes++;
			iterations++;
		}

		if (successes == b.size())
			results.push_back(b[child]);
	}

	std::sort(results.begin(), results.end());

	if (!results.empty())
		std::cout << results;
	else
		std::cout << "-1";
}
