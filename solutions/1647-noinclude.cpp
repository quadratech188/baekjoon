#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <type_traits>
#include <unistd.h>
#include <vector>

class UnionFind {
public:
	UnionFind(int size) {
		this->size = size;
		data.reserve(size);
		for (int i = 0; i < size; i++)
			data.push_back(i);
	}

	int find(int x) {
		if (data[x] == x)
			return x;
		data[x] = find(data[x]);
		return data[x];
	}

	void connect(int x, int y) {
		data[find(x)] = find(y);
	}

	bool connected(int x, int y) {
		return find(x) == find(y);
	}

private:
	std::vector<int> data;
	int size;
};

#ifndef FASTISTREAM_BUFFER_SIZE
#define FASTISTREAM_BUFFER_SIZE 1 << 20
#endif

#ifndef FASTOSTREAM_BUFFER_SIZE
#define FASTOSTREAM_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		inline char getchar() {
			static char buffer[FASTISTREAM_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (size <= 0) return EOF;
				ptr = buffer;
				end = buffer + size;
			}
			return *(ptr++);
		}
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar();
			} while (std::isspace(ch));

			// Optimized away for non-signed types
			bool negative = false;
			if constexpr (std::is_signed_v<T>) {
				if (ch == '-') {
					negative = true;
					ch = getchar();
				}
			}

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			if constexpr (std::is_signed_v<T>)
				if (negative) val = -val;

			return *this;
		}

		inline istream& operator>>(char& val) {
			do {
				val = getchar();
			} while (std::isspace(val));
			return *this;
		}
	};

	istream cin;

	/*
	class ostream {
		private:
			inline void putchar(char const& ch) {
				static char buffer[FASTOSTREAM_BUFFER_SIZE];
				static char* ptr = buffer;
				static char* end = buffer + (FASTOSTREAM_BUFFER_SIZE);

				if (ptr == end) {
					write(STDOUT_FILENO, buffer, FASTOSTREAM_BUFFER_SIZE);
					ptr = buffer;
				}
				*(ptr++) = ch;
			}
		public:
			template <typename T>
				inline ostream& operator<<(T& val)
				requires std::is_integral_v<T> {
					if (val < 0)
						putchar('-');
				}
	};
	*/
}

struct Edge {
	uint parent;
	uint child;
	uint cost;

	bool operator<(Edge const& other) const {
		return cost < other.cost;
	}
};

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	std::vector<Edge> edges(m);
	for (auto& val: edges) {
		Fast::cin >> val.parent >> val.child >> val.cost;
		val.parent --;
		val.child --;
	}

	if (n == 2) {
		std::cout << 0;
		return 0;
	}

	UnionFind uf(n);
	std::sort(edges.begin(), edges.end());

	uint total_cost = 0;

	uint counter = 0;
	for (auto [parent, child, cost]: edges) {
		if (uf.connected(parent, child)) continue;

		uf.connect(parent, child);
		total_cost += cost;
		counter++;
		if (counter == n - 2) break;
	}

	std::cout << total_cost;
}
