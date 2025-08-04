#include "../modules/ListGraph2.h"
#include "../modules/FastIO2.h"
#include "../modules/Bins.h"
#include "../modules/SSC.h"

int main() {
	ListGraph<uint, std::monostate> graph;
	uint v, e;
	Fast::cin >> v >> e;

	graph.resize(v);

	for (uint i = 0; i < e; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		graph.connect(a - 1, b - 1);
	}

	SetBins<uint> bins;

	SSC(graph, bins)();

	std::ranges::sort(bins.bins, [](std::set<uint>& a, std::set<uint>& b) {
			return *a.begin() < *b.begin();
			});

	std::cout << bins.bins.size() << '\n';
	for (auto& ssc: bins.bins) {
		for (uint val: ssc)
			std::cout << val + 1 << ' ';
		std::cout << "-1\n";
	}
}
