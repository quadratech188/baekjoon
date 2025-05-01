#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <variant>
#include <vector>

template <int size>
class PrimeLookup {
public:
	constexpr PrimeLookup() {
		primes.fill(true);

		primes[0] = false;
		primes[1] = false;

		for (int i = 2; i * i <= size; i++) {
			if (primes[i]) {
				for (int j = i * i; j <= size; j += i)
					primes[j] = false;
			}
		}
	};

	bool operator()(int n) const {
		return primes[n];
	}

private:
	std::array<bool, size+1> primes;

};

template <typename T>
inline auto InputRange(size_t n, std::istream& is = std::cin) {
	return std::views::iota(static_cast<size_t>(0), n)
		| std::views::transform([&is](size_t) {
				T temp;
				std::cin >> temp;
				return temp;
				});
}

template <typename Vertex, typename Edge>
class ListGraph {
public:
	using index_t = std::size_t;
	using vertex_t = Vertex;
	using edge_t = Edge;
	template <typename T>
	using storage_t = std::vector<T>;
	using size_t = std::size_t;

private:
	std::vector<Vertex> data;
	std::vector<std::vector<std::pair<index_t, edge_t>>> connections;
	size_t _size;

public:
	ListGraph(size_t size = 0, const Vertex& defaultV = Vertex()):
		data(size, defaultV), connections(size), _size(size) {}

	ListGraph(std::vector<Vertex>&& values):
		data(std::move(values)),
		connections(data.size()),
		_size(data.size()) {}

	index_t add(Vertex data) {
		data.push_back(data);
		connections.emplace_back();
		_size += 1;
		return _size - 1;
	}

	void reserve(int size) {
		data.reserve(size);
		connections.reserve(size);
	}

	void resize(size_t size) {
		data.resize(size);
		connections.resize(size);
		_size = size;
	}

	void connect(index_t parent, index_t child, edge_t edge = edge_t()) {
		connections[parent].emplace_back(child, edge);
	}

	size_t size() const {
		return _size;
	}

	vertex_t& operator[](index_t index) {
		return data[index];
	}

	class child {
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = child;

		child(ListGraph* graph, index_t parent, int list_index):
			graph(graph), parent(parent), list_index(list_index) {}

		child():
			graph(nullptr), list_index(0) {}

		index_t index() const {
			return graph->connections[parent][list_index].first;
		}
		edge_t& edge() {
			return graph->connections[parent][list_index].second;
		}
		edge_t const& edge() const {
			return graph->connections[parent][list_index].second;
		}

		vertex_t& value() {
			return graph->data[index()];
		}
		operator index_t() const {
			return index();
		}

		child operator*() const {
			return *this;
		}

		child& operator++() {
			++list_index;
			return *this;
		}
		
		void operator++(int) {
			++list_index;
		}

		bool operator!=(child const& other) const {
			return list_index != other.list_index;
		}

		bool operator==(child const& other) const {
			return list_index == other.list_index;
		}

	private:
		ListGraph* graph;
		index_t parent;
		int list_index;
	};

	auto children(index_t parent) {
		return std::ranges::subrange(
				child(this, parent, 0),
				child(this, parent, connections[parent].size())
				);
	}

	int degree(index_t parent) {
		return connections[parent].size();
	}

	void connect_both(index_t parent, index_t child, edge_t edge1 = edge_t(), edge_t edge2 = edge_t())
	requires std::same_as<int, decltype(edge_t().rev)> {
		edge1.rev = connections[child].size();
		edge2.rev = connections[parent].size();

		connect(parent, child, edge1);
		connect(child, parent, edge2);
	}

	child reverse(child original)
	requires std::same_as<int, decltype(edge_t().rev)> {
		return child(this, original.index(), original.edge().rev);
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& values) {
	if (values.size() == 0) return os;

	for (size_t i = 0; i < values.size() - 1; i++) {
		std::cout << values[i] << ' ';
	}
	std::cout << values[values.size() - 1];

	return os;
}

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
