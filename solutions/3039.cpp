#include "SegmentTree2.h"
#include "FastIO2.h"
#include "Compress.h"
#include "OverloadedPlus.h"
#include <iostream>
#include <limits>
#include <set>
#include <variant>

struct Query {
	char type;
	int a, b;
	uint i;
};

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<std::pair<int, int>> additions;
	std::vector<Query> queries(n);

	std::vector<int> knowledges;

	uint index = 0;
	for (auto& query: queries) {
		Fast::cin >> query.type;
		if (query.type == 'D') {
			Fast::cin >> query.a >> query.b;
			query.i = index;
			index ++;
			additions.emplace_back(query.a, query.b);
			knowledges.push_back(query.b);
		}
		else
		 	Fast::cin >> query.i;
	}

	std::ranges::sort(knowledges);
	Compress<int> knowledge_compress(knowledges);

	auto set_compare = [&additions](uint l, uint r) {
		return additions[l].first < additions[r].first;
	};

	std::vector<std::set<uint, decltype(set_compare)>> students;

	for (size_t i = 0; i < knowledge_compress.size(); i++)
		students.emplace_back(set_compare);

	SegmentTree<maxxer<int>> max_tree(knowledge_compress.size(), std::numeric_limits<int>::min());

	for (auto& query: queries) {
		if (query.type == 'D') {
			int temp = query.a;

			uint knowledge_index = knowledge_compress.compress(query.b);

			max_tree.update(knowledge_index, [temp](maxxer<int>& x) {x = x + temp;});
			students[knowledge_index].insert(query.i);
			continue;
		}

		auto [understanding, knowledge] = additions[query.i - 1];

		uint start = knowledge_compress.compress(knowledge);

		auto at_start = students[start].upper_bound(query.i - 1);
		if (at_start != students[start].end()) {
			std::cout << *at_start + 1 << '\n';
			continue;
		}

		uint l_ptr = start + 1;
		uint r_ptr = max_tree.size() - 1;

		if (l_ptr > r_ptr) {
			std::cout << "NE\n";
			continue;
		}

		while (l_ptr != r_ptr) {
			uint center = (l_ptr + r_ptr) / 2;
			int val = max_tree.sum(start + 1, center + 1).val();
			
			if (val >= understanding)
				r_ptr = center;
			else
				l_ptr = center + 1;
		}

		auto answer = students[l_ptr].lower_bound(query.i - 1);
		if (answer == students[l_ptr].end())
			std::cout << "NE\n";
		else
		 	std::cout << *answer + 1 << '\n';
	}
}
