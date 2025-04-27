#include "../modules/UnionFind.h"
#include "../modules/Input.h"
#include "../modules/ListGraph.h"
#include "../modules/TreeWrapper.h"
#include "../modules/SegmentTree.h"
#include "../modules/FastIO.h"
#include <iostream>
#include <istream>
#include <string>
#include <variant>

std::vector<int> subtree_sizes;
std::vector<int> heavy_edges;

template <Graph G>
int calculate_subtree_sizes(G& tree, int parent = 0) {
	int sum = 1;
	int max = 0;
	for (auto child: tree.children(parent)) {
		int size = calculate_subtree_sizes(tree, child);

		if (size > max) {
			max = size;
			heavy_edges[parent] = child;
		}

		sum += size;
	}

	subtree_sizes[parent] = sum;
	return sum;
}

std::vector<int> chain_heads;
std::vector<int> chain_parents;
std::vector<int> chain_indices;
std::vector<int> chain_depths;

template <Graph G>
void decompose(G& tree, int parent = 0, int depth = 0, int chain_head = 0, int chain_parent = 0) {
	static int index = 0;
	chain_heads[parent] = chain_head;
	chain_parents[parent] = chain_parent;

	chain_indices[parent] = index;
	index++;

	chain_depths[parent] = depth;

	if (heavy_edges[parent] == -1) return;

	decompose(tree, heavy_edges[parent], depth, chain_head, chain_parent);

	for (auto child: tree.children(parent)) {
		if (child != heavy_edges[parent]) {
			decompose(tree, child, depth + 1, child, parent);
		}
	}
}

struct Instruction {
	enum type {
		BRIDGE,
		PENGUINS,
		EXCURSION
	};
	type type;
	int a, b;
};

std::istream& operator>>(std::istream& is, Instruction& instruction) {
	static std::string type;
	is >> type >> instruction.a >> instruction.b;

	if (type[0] == 'b') {
		instruction.type = Instruction::BRIDGE;
		instruction.a--;
		instruction.b--;
	}
	else if (type[0] == 'p') {
		instruction.type = Instruction::PENGUINS; 
		instruction.a--;
	}
	else {
	 	instruction.type = Instruction::EXCURSION;
		instruction.a--;
		instruction.b--;
	}
	return is;
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	auto const penguins = Input::toVec<int>(n);

	int q;
	std::cin >> q;
	auto const instructions = Input::toVec<Instruction>(q);
	std::vector<int> results(q);

	int const root = n;
	int const size = n + 1;
	UnionFind uf(size);
	ListGraph<std::monostate, std::monostate> graph(size);

	for (int i = 0; i < q; i++) {
		Instruction instruction = instructions[i];

		switch(instruction.type) {
			case Instruction::BRIDGE:
				if (uf.connected(instruction.a, instruction.b)) {
					results[i] = false;
				}
				else {
					uf.connect(instruction.a, instruction.b);
					graph.connect(instruction.a, instruction.b);
					graph.connect(instruction.b, instruction.a);
					results[i] = true;
				}
				break;
			case Instruction::PENGUINS:
				break;
			case Instruction::EXCURSION:
				if (!uf.connected(instruction.a, instruction.b))
					results[i] = -1;
		}
	}

	for (int i = 0; i < n; i++) {
		if (!uf.connected(i, root)) {
			uf.connect(i, root);
			graph.connect(i, root);
			graph.connect(root, i);
		}
	}

	TreeWrapper tree(graph, root);

	subtree_sizes.resize(size);
	heavy_edges.resize(size, -1);
	calculate_subtree_sizes(tree, root);

	chain_heads.resize(size);
	chain_parents.resize(size);
	chain_indices.resize(size);
	chain_depths.resize(size);
	decompose(tree, root);

	SegmentTree<int> segtree(size);
	for (int i = 0; i < n; i++) {
		segtree.update(chain_indices[i], [i, &penguins](int& val) {
				val = penguins[i];
				});
	}

	for (int i = 0; i < q; i++) {
		Instruction instruction = instructions[i];

		switch(instruction.type) {
			case Instruction::BRIDGE:
				if (results[i])
					std::cout << "yes\n";
				else
				 	std::cout << "no\n";
				break;
			case Instruction::PENGUINS:
				segtree.update(chain_indices[instruction.a], [&instruction](int& val) {
						val = instruction.b;
						});
				break;
			case Instruction::EXCURSION:
				if (results[i] == -1) {
					std::cout << "impossible\n";
					break;
				}

				int mut_a = instruction.a;
				int mut_b = instruction.b;

				int result = 0;

				while (chain_heads[mut_a] != chain_heads[mut_b]) {
					if (chain_depths[mut_a] < chain_depths[mut_b]) std::swap(mut_a, mut_b);
					result += segtree.sum(
								chain_indices[chain_heads[mut_a]],
								chain_indices[mut_a] + 1
								);
					mut_a = chain_parents[mut_a];
				}

				if (chain_indices[mut_a] > chain_indices[mut_b])
					std::swap(mut_a, mut_b);

				result += segtree.sum(
						chain_indices[mut_a],
						chain_indices[mut_b] + 1
						);

				std::cout << result << '\n';
		}
	}
}
